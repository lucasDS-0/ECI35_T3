#include <iomanip>
#include <iostream>
#include <cmath>
#include <limits>

using my_float = long double;

auto pi_taylor(size_t steps) -> my_float {
    my_float pi = 0.0;
    int sign = 1;
    for (int n = 0; n < steps; n++) {
        pi+=sign/static_cast<my_float>(2*n+1);
        sign = -sign;
    }
    
    return 4.0f* pi;
}

int main(int argc, const char *argv[]) {

    // read the number of steps from the command line
    if (argc != 2) {
        std::cerr << "Invalid syntax: pi_taylor <steps>" << std::endl;
        exit(1);

    }

    size_t steps = std::stoll(argv[1]);
    auto pi = pi_taylor(steps);

    std::cout << "For " << steps << ", pi value: "
        << std::setprecision(std::numeric_limits<long double>::digits10 + 1)
        << pi << std::endl;
}
