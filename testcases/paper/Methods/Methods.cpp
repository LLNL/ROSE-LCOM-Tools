#include <iostream>

// Outside method
void outsideMethod() {
    std::cout << "Outside method called.\n";
}

// Class definition
class MyClass {
public:
    void methodOne() {
        std::cout << "Method one called.\n";
    }

    void methodTwo() {
        std::cout << "Method two called.\n";
        methodOne(); // Calls methodOne
    }

    void methodThree() {
        std::cout << "Method three called.\n";
        outsideMethod(); // Calls outside method
    }
};
