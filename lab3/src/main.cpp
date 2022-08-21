#include "aliases.h"
#include "AIMN91_DataStructure.h"

int main() {
    AIMN91_DataStructure AIMN91(20);
    int i = 0, j = 1, k = 2;

    Vertex v0 = AIMN91.get_vertex(i);
    Vertex v1 = AIMN91.get_vertex(j);
    Vertex v2 = AIMN91.get_vertex(k);
    AIMN91.add(v0, v1, 1);
    AIMN91.add(v1, v2, 1);
    std::cout << "Cost for path " << i << "->" << j  << " = " << AIMN91.length(v0,v1) << std::endl;
    std::cout << "Cost for path " << i << "->" << k  << " = " << AIMN91.length(v1,v2) << std::endl;
}