#include "aliases.h"
#include "AIMN91_DataStructure.h"

int main() {
    AIMN91_DataStructure AIMN91(20);
    int i = 0, j = 1;
    std::cout << "Cost for path " << i << "->" << j  << " = " << AIMN91.length(i,j) << std::endl;
}