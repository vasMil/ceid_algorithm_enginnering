#include "aliases.h"
#include "AIMN91_DataStructure.h"

void test_directed_graph_no_weight() {
    AIMN91_DataStructure AIMN91(7);

    Vertex v0 = AIMN91.get_vertex(0);
    Vertex v1 = AIMN91.get_vertex(1);
    Vertex v2 = AIMN91.get_vertex(2);
    Vertex v3 = AIMN91.get_vertex(3);
    Vertex v4 = AIMN91.get_vertex(4);
    Vertex v5 = AIMN91.get_vertex(5);
    Vertex v6 = AIMN91.get_vertex(6);
    
    AIMN91.add(v0, v1, 1);
    AIMN91.add(v0, v2, 1);
    AIMN91.add(v0, v3, 1);

    AIMN91.add(v1, v4, 1);

    AIMN91.add(v2, v4, 1);

    AIMN91.add(v3, v5, 1);

    AIMN91.add(v4, v1, 1);
    AIMN91.add(v4, v2, 1);
    AIMN91.add(v4, v5, 1);

    AIMN91.add(v5, v6, 1);

    AIMN91.add(v6, v4, 1);
    AIMN91.add(v6, v0, 1);

    for(int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            std::cout << "d[" << i << "," << j << "] = " << AIMN91.length(i, j) << std::endl;
        }
    }

    AIMN91.minimal_path(0,6);
}