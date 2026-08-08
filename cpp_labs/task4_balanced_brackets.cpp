
#include <iostream>
#include <string>
#include <stack>

bool isMatchingPair(char open, char close) {
    return (open == '(' && close == ')') ||
           (open == '[' && close == ']') ||
           (open == '{' && close == '}');
}

int checkBalanced(const std::string& s) {
    std::stack<char> st;
    std::stack<int> indices; 

    for (size_t i = 0; i < s.size(); ++i) {
        char c = s[i];
        if (c == '(' || c == '[' || c == '{') {
            st.push(c);
            indices.push(static_cast<int>(i));
        } else if (c == ')' || c == ']' || c == '}') {
            if (st.empty() || !isMatchingPair(st.top(), c)) {
                return static_cast<int>(i); 
            }
            st.pop();
            indices.pop();
        }
    }

    if (!st.empty()) {
        return indices.top(); 
    }

    return -1; 
}

int main() {
    std::string input;
    std::cout << "Enter a string with brackets: ";
    std::getline(std::cin, input);

    int result = checkBalanced(input);

    if (result == -1) {
        std::cout << "The string is balanced." << std::endl;
    } else {
        std::cout << "The string is NOT balanced. First unmatched bracket at index: "
                  << result << std::endl;
    }

    return 0;
}
