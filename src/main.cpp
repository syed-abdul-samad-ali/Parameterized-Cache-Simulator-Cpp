#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <stdio.h>
#include <GLFW/glfw3.h>

#include <iostream> 
#include <vector>   
#include <string>
#include <sstream>
#include <cmath>
#include <fstream> 
#include <random> // Added for modern C++ random number generation

using ull = unsigned long long;

enum Policy { 
    LRU = 0, 
    FIFO = 1, 
    RANDOM = 2 
};

struct CacheLine {
    bool valid;
    ull tag;
    ull lru_timestamp;
    ull fifo_timestamp;

    CacheLine() : valid(false), tag(0), lru_timestamp(0), fifo_timestamp(0) {}
};

class CacheSimulator {
public: 
    int total_size;
    int block_size;
    int associativity;
    int policy;
    bool is_configured;

    int hits;
    int misses;
    int evictions;
    ull global_time; 
    
    // Variables to track the last accessed cell for GUI Highlighting
    int last_accessed_set;
    int last_accessed_way;

    std::vector<std::vector<CacheLine>> sets;
    std::vector<std::string> execution_log;

    CacheSimulator() : total_size(1024), block_size(16), associativity(4), 
                       policy(LRU), is_configured(false), hits(0), 
                       misses(0), evictions(0), global_time(0),
                       last_accessed_set(-1), last_accessed_way(-1) {}

    // Using bitwise AND operator to validate powers of 2 efficiently
    bool is_power_of_two(int n) {
        return (n > 0) && ((n & (n - 1)) == 0);
    }

    void log_message(const std::string& msg) { 
        execution_log.push_back(msg); 
    }

    void reset() {
        hits = 0; 
        misses = 0; 
        evictions = 0;
        global_time = 0;
        last_accessed_set = -1;
        last_accessed_way = -1;
        execution_log.clear(); 
        if (is_configured) {
            configure();
        }
    }

    bool configure() {
        // Validation Check #1: Power of two validation
        if (!is_power_of_two(total_size) || !is_power_of_two(block_size) || !is_power_of_two(associativity)) {
            log_message("Error: Sizes and Associativity must be powers of 2.");
            is_configured = false;
            return false;
        }

        int num_sets = total_size / (block_size * associativity);
        if (num_sets <= 0) {
            log_message("Error: Invalid configuration parameters.");
            is_configured = false;
            return false;
        }

        sets.clear();
        for (int i = 0; i < num_sets; ++i) {
            std::vector<CacheLine> way_vector;
            for (int j = 0; j < associativity; ++j) {
                way_vector.push_back(CacheLine());
            }
            sets.push_back(way_vector);
        }
        
        is_configured = true;
        log_message("System Configured Successfully.");
        return true;
    }

    void access(char op, ull addr) {
        global_time++; 
        
        int offset_bits = log2(block_size);
        int num_sets = total_size / (block_size * associativity);
        int index_bits = log2(num_sets);

        ull offset_mask = (1ULL << offset_bits) - 1;
        ull index_mask = (1ULL << index_bits) - 1;
        
        ull index = (addr >> offset_bits) & index_mask;
        ull tag = addr >> (offset_bits + index_bits);

        std::stringstream ss;
        ss << "Addr 0x" << std::hex << addr << std::dec 
           << " -> Set " << index << ", Tag 0x" << std::hex << tag << std::dec;

        bool hit = false;
        int hit_way = -1;

        for (int i = 0; i < associativity; ++i) {
            if (sets[index][i].valid && sets[index][i].tag == tag) {
                hit = true;
                hit_way = i;
                break; 
            }
        }

        if (hit) {
            hits++;
            ss << " -> HIT";
            if (policy == LRU) {
                sets[index][hit_way].lru_timestamp = global_time;
            }
            // Update tracking for UI highlighting
            last_accessed_set = index;
            last_accessed_way = hit_way;

        } else {
            misses++;
            int target_way = -1;
            
            for (int i = 0; i < associativity; ++i) {
                if (!sets[index][i].valid) {
                    target_way = i;
                    break;
                }
            }

            if (target_way == -1) {
                evictions++;
                target_way = findEvictionTarget(index);
                ss << " -> MISS (Evicted Tag 0x" << std::hex << sets[index][target_way].tag << std::dec << ")";
            } else {
                ss << " -> MISS (Empty Line Filled)";
            }

            sets[index][target_way].valid = true;
            sets[index][target_way].tag = tag;
            sets[index][target_way].fifo_timestamp = global_time; 
            sets[index][target_way].lru_timestamp = global_time;
            
            // Update tracking for UI highlighting
            last_accessed_set = index;
            last_accessed_way = target_way;
        }
        log_message(ss.str());
    }

private: 
    int findEvictionTarget(int set_idx) {
        switch (policy) {
            case RANDOM: {
                // Using modern C++ <random> header
                std::random_device rd; 
                std::default_random_engine engine(rd()); 
                std::uniform_int_distribution<int> dist(0, associativity - 1);
                return dist(engine); 
            }
                
            case FIFO:
            case LRU: {
                int target = 0;
                ull min_time = 0xFFFFFFFFFFFFFFFFULL; 
                
                for (int i = 0; i < associativity; ++i) {
                    ull compare_time = (policy == LRU) ? sets[set_idx][i].lru_timestamp : sets[set_idx][i].fifo_timestamp;
                    if (compare_time < min_time) {
                        min_time = compare_time;
                        target = i;
                    }
                }
                return target;
            }
            default:
                return 0; 
        }
    }
};

// --- GUI FRONTEND ---
static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

int main(int, char**) {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) return 1;

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow* window = glfwCreateWindow(1000, 650, "Interactive Cache Simulator", nullptr, nullptr);
    if (window == nullptr) return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    CacheSimulator sim;
    char trace_buffer[8192] = "R 0x1000\nW 0x1004\nR 0x2000\nR 0x1000\n";
    static char filename_buffer[256] = "trace.txt";
    
    std::vector<std::string> trace_lines;
    int current_step = 0;

    auto parseTrace = [&]() {
        trace_lines.clear();
        std::string raw_trace(trace_buffer);
        std::stringstream ss(raw_trace);
        std::string line;
        while (std::getline(ss, line)) {
            if (!line.empty() && line.find_first_not_of(" \r\n\t") != std::string::npos) {
                trace_lines.push_back(line);
            }
        }
    };

    // Helper to check if trace buffer is completely empty
    auto isTraceEmpty = [&]() {
        std::string trace_str(trace_buffer);
        return trace_str.find_first_not_of(" \t\n\r") == std::string::npos;
    };

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        ImGui::Begin("Simulator", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

        // 1. Configuration Panel
        ImGui::Text("1. Configuration & Control Panel");
        ImGui::Separator();
        ImGui::InputInt("Total Cache Size (B)", &sim.total_size);
        ImGui::InputInt("Block Size (B)", &sim.block_size);
        ImGui::InputInt("Associativity (Ways)", &sim.associativity);
        
        ImGui::Text("Replacement Policy:");
        ImGui::RadioButton("LRU", &sim.policy, LRU); ImGui::SameLine();
        ImGui::RadioButton("FIFO", &sim.policy, FIFO); ImGui::SameLine();
        ImGui::RadioButton("Random", &sim.policy, RANDOM);

        if (ImGui::Button("Configure Cache")) {
            sim.configure();
            current_step = 0;
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
        
        // Validation Check #2: Load Trace Button functionality
        ImGui::InputText("Trace File Name", filename_buffer, sizeof(filename_buffer));
        ImGui::SameLine();
        if (ImGui::Button("Load Trace...")) {
            std::ifstream infile(filename_buffer);
            if (infile.is_open()) {
                std::string file_contents((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());
                snprintf(trace_buffer, sizeof(trace_buffer), "%s", file_contents.c_str());
                sim.log_message("Trace loaded from file: " + std::string(filename_buffer));
            } else {
                sim.log_message("Error: Could not open trace file. Check filename.");
            }
        }

        ImGui::Spacing();

        if (ImGui::Button("Run Simulation")) {
            if (!sim.is_configured) {
                sim.log_message("Error: Please configure cache parameters first.");
            } 
            // Validation Check #4: Empty Trace Error
            else if (isTraceEmpty()) {
                sim.log_message("Error: Please load a trace file first.");
            } 
            else {
                sim.reset();
                parseTrace();
                
                for (std::vector<std::string>::const_iterator iter = trace_lines.begin(); iter != trace_lines.end(); ++iter) {
                    std::stringstream lss(*iter);
                    char op; 
                    std::string addr_str;
                    if (lss >> op >> addr_str) {
                        try {
                            ull addr = std::stoull(addr_str, nullptr, 16);
                            sim.access(op, addr);
                        } catch (...) { 
                            sim.log_message("Format error on line."); 
                        }
                    }
                }
            }
        }
        ImGui::SameLine();
        
        if (ImGui::Button("Step")) {
            if (!sim.is_configured) {
                sim.log_message("Error: Please configure cache parameters first.");
            } 
            // Validation Check #4: Empty Trace Error
            else if (isTraceEmpty()) {
                sim.log_message("Error: Please load a trace file first.");
            }
            else {
                if (current_step == 0) { 
                    sim.reset(); 
                    parseTrace(); 
                }
                
                if (current_step < (int)trace_lines.size()) {
                    std::stringstream lss(trace_lines[current_step]);
                    char op; 
                    std::string addr_str;
                    if (lss >> op >> addr_str) {
                        try {
                            ull addr = std::stoull(addr_str, nullptr, 16);
                            sim.access(op, addr);
                        } catch (...) { 
                            sim.log_message("Format error on step."); 
                        }
                    }
                    current_step++;
                } else {
                    sim.log_message("End of trace reached.");
                }
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Reset")) {
            sim.reset();
            current_step = 0;
        }

        ImGui::Spacing(); ImGui::Spacing();

        // 2. Trace Editor
        ImGui::Text("2. Trace Editor Panel");
        ImGui::Separator();
        ImGui::InputTextMultiline("##Trace", trace_buffer, IM_ARRAYSIZE(trace_buffer), ImVec2(-FLT_MIN, 150));

        ImGui::Spacing(); ImGui::Spacing();

        // 3. Results Dashboard
        ImGui::Text("3. Statistics Dashboard");
        ImGui::Separator();
        ImGui::TextColored(ImVec4(0, 1, 0, 1), "Hits: %d", sim.hits); ImGui::SameLine();
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "Misses: %d", sim.misses); ImGui::SameLine();
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "Evictions: %d", sim.evictions);

        ImGui::Text("Execution Log:");
        ImGui::BeginChild("LogRegion", ImVec2(-FLT_MIN, 120), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
        
        for (std::vector<std::string>::const_iterator log_iter = sim.execution_log.begin(); log_iter != sim.execution_log.end(); ++log_iter) {
            ImGui::TextUnformatted((*log_iter).c_str());
        }
        
        if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
            ImGui::SetScrollHereY(1.0f); 
        }
        ImGui::EndChild();

        ImGui::Spacing(); ImGui::Spacing();

        // 4. Cache Visualizer
        ImGui::Text("4. Cache Visualizer");
        ImGui::Separator();
        if (sim.is_configured && ImGui::BeginTable("CacheGrid", sim.associativity + 1, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY, ImVec2(0, 200))) {
            
            ImGui::TableSetupColumn("Set Index");
            for (int i = 0; i < sim.associativity; ++i) {
                ImGui::TableSetupColumn(("Way " + std::to_string(i)).c_str());
            }
            ImGui::TableHeadersRow();

            int num_sets = sim.total_size / (sim.block_size * sim.associativity);
            for (int s = 0; s < num_sets; ++s) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("Set %d", s);
                for (int w = 0; w < sim.associativity; ++w) {
                    ImGui::TableSetColumnIndex(w + 1);
                    
                    // Validation Check #3: Highlight Accessed Cell
                    if (s == sim.last_accessed_set && w == sim.last_accessed_way) {
                        // Apply a green highlight background to the active cell
                        ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, ImGui::GetColorU32(ImVec4(0.0f, 0.5f, 0.0f, 0.6f))); 
                    }

                    if (sim.sets[s][w].valid) {
                        ImGui::Text("V | Tag: %llX", (unsigned long long)sim.sets[s][w].tag);
                    } else {
                        ImGui::TextDisabled("Empty");
                    }
                }
            }
            ImGui::EndTable();
        }

        ImGui::End();

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
