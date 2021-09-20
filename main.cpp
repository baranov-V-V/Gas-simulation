//#include "sphere.h"

#include <iostream>
#include "MathVector2D.h"

int main() {
    MathVector2D<double> v1(1, -3);
    MathVector2D<double> v2(0.845684, 0.533684);

    MathVector2D<double> v11(1, -3);
    MathVector2D<double> v22(0.31241, 0.453124);

    std::cout << (v1 * v2) << std::endl;
    std::cout << (v11 * v22) << std::endl;
    
    return 0;
}