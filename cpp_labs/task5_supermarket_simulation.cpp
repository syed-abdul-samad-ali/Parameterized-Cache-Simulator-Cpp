

#include <iostream>
#include <vector>
#include <queue>

int main() {
    const int N = 3; 
    std::vector<std::queue<int>> lanes(N);

  
    std::vector<std::pair<int, int>> arrivals = {
        {0, 101}, {0, 102}, {1, 103}, {1, 104},
        {2, 105}, {3, 106}, {3, 107}, {4, 108}
    };

    const int SIMULATION_TIME = 6;

    for (int t = 0; t <= SIMULATION_TIME; ++t) {
       
        for (const auto& arrival : arrivals) {
            if (arrival.first == t) {
                int customerID = arrival.second;

              
                int shortestLane = 0;
                for (int i = 1; i < N; ++i) {
                    if (lanes[i].size() < lanes[shortestLane].size()) {
                        shortestLane = i;
                    }
                }

                lanes[shortestLane].push(customerID);
                std::cout << "Time " << t << ": Customer " << customerID
                          << " joins lane " << shortestLane << std::endl;
            }
        }

    
        std::cout << "  State at time " << t << ":" << std::endl;
        for (int i = 0; i < N; ++i) {
            std::cout << "    Lane " << i << ": [";
            std::queue<int> temp = lanes[i]; 
            bool first = true;
            while (!temp.empty()) {
                if (!first) std::cout << ", ";
                std::cout << temp.front();
                temp.pop();
                first = false;
            }
            std::cout << "]" << std::endl;
        }

      
        for (int i = 0; i < N; ++i) {
            if (!lanes[i].empty()) {
                std::cout << "  Lane " << i << " serves customer " << lanes[i].front() << std::endl;
                lanes[i].pop();
            }
        }
        std::cout << std::endl;
    }

    return 0;
}
