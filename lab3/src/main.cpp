#include "aliases.h"
#include "AIMN91_DataStructure.h"

int main() {
    AIMN91_DataStructure AIMN91(20);
    int i = 0, j = 1;
    std::cout << "Cost for path " << i << "->" << j  << " = " << AIMN91.length(i,j) << std::endl;
    std::cout << "Vertex 0 descriptor: " << AIMN91.get_vertex(0) << std::endl;
    std::cout << "Vertex 1 descriptor: " << AIMN91.get_vertex(1) << std::endl;
    std::cout << "FORWARD[0,0] = " << AIMN91.FORWARD[0,0] << std::endl;
}