#include <iostream>
#include <string>

class Person {
private:
    std::string family_name_;
    std::string given_name_;

public:
    Person(const std::string& family_name, const std::string& given_name)
        : family_name_(family_name), given_name_(given_name) {}

    std::string family_name() const { return family_name_; }
    std::string given_name() const { return given_name_; }
    std::string full_name() const { return family_name_ + ", " + given_name_; }
};

class Student : public Person {
private:
    std::string student_id_;

public:
    Student(const std::string& family_name, const std::string& given_name, const std::string& student_id)
        : Person(family_name, given_name), student_id_(student_id) {}

    std::string student_id() const { return student_id_; }
};

int main() {
    Student s("Doe", "John", "12345");
    std::cout << s.full_name() << " | ID: " << s.student_id() << std::endl;

    return 0;
}