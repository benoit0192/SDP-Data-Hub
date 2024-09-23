#include <iomanip>
#include <iostream>

#include "Utils.hpp"


void printHex(const unsigned char& byte) {
    std::cout << std::hex << std::setfill('0') << std::setw(2)
              << static_cast<unsigned int>(byte);
    std::cout << std::dec;  // Reset to decimal
}
