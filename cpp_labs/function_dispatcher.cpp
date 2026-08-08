#include <iostream>
#include <string>

class Logger {
public:
    void log(const std::string& msg) {
        std::cout << "[PERMANENT LOG]: " << msg << std::endl;
    }

    void log(std::string&& msg) {
        std::cout << "[TEMPORARY LOG]: " << msg << std::endl;
    }
};

int main() {
    Logger my_logger;

    std::string report = "System OK";

    my_logger.log(report);
    my_logger.log("Low Memory");

    return 0;
}