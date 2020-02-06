#include <iostream>

#include "common.h"
class Base {
private:
    virtual void foo(){}
};
class Drived:public Base {

};
int main() {
    Base b;
    Drived d;
    Base& b1 = d;
    Base* b2 = new Drived;
    Base& b3 = *b2;

    std::cout<<type_name(b)<<std::endl;    // Base
    std::cout<<type_name(d)<<std::endl;    // Drived
    std::cout<<type_name(b1)<<std::endl;   // Drived
    std::cout<<type_name(b2)<<std::endl;   // Base*
    std::cout<<type_name(b3)<<std::endl;   // Drived
}
