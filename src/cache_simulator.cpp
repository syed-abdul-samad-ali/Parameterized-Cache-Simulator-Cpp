#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>

enum ReplacementPolicy { FIFO, LRU, RANDOM };

struct CacheLine {
    bool valid = false;
    unsigned int tag = 0;
    int last_accessed = 0;
};

class CacheSimulator {
private:
    size_t cache_size;
    size_t block_size;
    size_t associativity;
    ReplacementPolicy policy;

    size_t num_sets;
    std::vector<std::vector<CacheLine>> cache;
    int timer = 0;

    size_t hits = 0;
    size_t misses = 0;

public:
    CacheSimulator(size_t c_size, size_t b_size, size_t assoc, ReplacementPolicy p)
        : cache_size(c_size), block_size(b_size), associativity(assoc), policy(p) {
        
        num_sets = cache_size / (block_size * associativity);
        cache.resize(num_sets, std::vector<CacheLine>(associativity));
    }

    void access_memory(unsigned int address) {
        timer++;
        unsigned int set_index = (address / block_size) % num_sets;
        unsigned int tag = address / (block_size * num_sets);

        auto& set = cache[set_index];
        for (auto& line : set) {
            if (line.valid && line.tag == tag) {
                hits++;
                line.last_accessed = timer;
                std::cout << "[HIT] Address: 0x" << std::hex << address << " | Set: " << std::dec << set_index << "\n";
                return;
            }
        }

        // Cache Miss handling
        misses++;
        std::cout << "[MISS] Address: 0x" << std::hex << address << " | Set: " << std::dec << set_index << "\n";

        // Find empty or replace line
        int replace_idx = 0;
        for (size_t i = 0; i < associativity; ++i) {
            if (!set[i].valid) {
                replace_idx = i;
                break;
            }
            if (policy == LRU && set[i].last_accessed < set[replace_idx].last_accessed) {
                replace_idx = i;
            }
        }

        set[replace_idx].valid = true;
        set[replace_idx].tag = tag;
        set[replace_idx].last_accessed = timer;
    }

    void print_stats() const {
        std::cout << "\n--- Cache Statistics ---\n";
        std::cout << "Total Hits  : " << hits << "\n";
        std::cout << "Total Misses: " << misses << "\n";
        std::cout << "Hit Ratio   : " << (float)hits / (hits + misses) * 100.0f << "%\n";
    }
};

int main() {
    // 4KB Cache, 64-byte blocks, 2-way set associative, LRU
    CacheSimulator sim(4096, 64, 2, LRU);

    sim.access_memory(0x1000);
    sim.access_memory(0x1004); // Hit
    sim.access_memory(0x2000); // Miss
    sim.access_memory(0x1000); // Hit

    sim.print_stats();
    return 0;
}
