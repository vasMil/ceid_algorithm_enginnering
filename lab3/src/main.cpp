#include "aliases.h"
#include "AIMN91_DataStructure.h"

int main() {
    AIMN91_DataStructure AIMN91(20);
    int i = 0, j = 1, k = 2, l = 3;
    int cnt = 3;

    Vertex v0 = AIMN91.get_vertex(i);
    Vertex v1 = AIMN91.get_vertex(j);
    Vertex v2 = AIMN91.get_vertex(k);
    Vertex v3 = AIMN91.get_vertex(l);
    AIMN91.add(v0, v1, 1);
    AIMN91.add(v1, v2, 2);
    AIMN91.add(v2, v0, 3);
//    AIMN91.add(v2, v3, 3);
    std::cout << "Cost for path " << i << "->" << j  << " = " << AIMN91.length(v0,v1) << std::endl;
    std::cout << "Cost for path " << i << "->" << k  << " = " << AIMN91.length(v0,v2) << std::endl;
    // std::cout << "Cost for path " << k << "->" << l  << " = " << AIMN91.length(v2,v3) << std::endl;
    std::cout << std::endl;
    for(int l = 0; l < cnt; l++) {
        for(int x = 0; x < cnt; x++) {
            if(AIMN91.FORWARD[l][x] != NULL) {
                std::cout << "FORWARD[" << l << "][" << x << "]->content = " << AIMN91.FORWARD[l][x]->content << std::endl;
            }
            else {
                std::cout << "FORWARD[" << l << "][" << x << "] = " << "NULL" << std::endl;
            }
        }
    }
    std::cout << std::endl;
    for(int l = 0; l < cnt; l++) {
        for(int x = 0; x < cnt; x++) {
            if(AIMN91.BACKWARD[l][x] != NULL) {
                std::cout << "BACKWARD[" << l << "][" << x << "]->content = " << AIMN91.BACKWARD[l][x]->content << std::endl;
            }
            else {
                std::cout << "BACKWARD[" << l << "][" << x << "] = " << "NULL" << std::endl;
            }
        }
    }
    std::cout << std::endl;
    for(int l = 0; l < cnt; l++) {
        for(int x = 0; x < cnt; x++) {
            std::cout << "D[" << l << "][" << x << "] = " << AIMN91.D[l][x] << std::endl;
        }
    }
}