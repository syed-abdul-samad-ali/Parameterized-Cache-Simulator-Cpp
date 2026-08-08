
#include <iostream>
#include <vector>
#include <string>

void printVector(std::vector<std::string>& vec) {
   
    for (std::vector<std::string>::const_iterator it = vec.cbegin(); it != vec.cend(); ++it) {
        std::cout << *it << std::endl;
       
    }
}

int main() {
    std::vector<std::string> names = {"Alice", "Bob", "Charlie", "Diana"};
    printVector(names);
    return 0;
}
