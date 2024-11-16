#include <iostream>
#include <Ligma/ligma.hpp>
#include <string>

int main(){

    std::cout << "Hello wolrd! " + std::to_string(TestNamespace::ligma()) << std::endl;
    return 0;
}