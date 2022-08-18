#include "aliases.h"

class AIMN91_DataStructure {
    private:
        int num_vertices;
        int num_edges;
        int upperCostBound;
        static const int lowerCostBound = MIN_C;
        
        // You are better off using arrays instead of vectors.
        // Arrays use less memory and vectors are better with frequent
        // insertions and deletions which I am not going to be using
        int** D; // The distance matrix

        // The FORWARD Matrix - initially containing all NULL
        // Vertex** FORWARD;
        // The BACKWARD Matrix - initially containing all NULL
        Vertex** BACKWARD;
        // Initialize the directed Graph
        Graph G;
        
        // updateForward() merging both the updates at DESC(x) and ANC(x)

    public:
        Vertex** FORWARD;
        
        // Create an empty graph with num_vertices vertices
        AIMN91_DataStructure(int num_vertices) {
            // Make sure that matrix D has cost values < INF, so INF implies there is no path
            if((num_vertices-1) > INF / MAX_C) {
                throw std::invalid_argument("There might be a path that has a cost > constant INF");
            }

            this->num_vertices = num_vertices;
            this->num_edges = 0;
            this->upperCostBound = MAX_C;

            // Add vertices to the graph
            Vertex temp[num_vertices];
            for(int i=0; i < num_vertices; i++) {
                temp[i] = boost::add_vertex(G);
            }
            DESCpmap DESC = get(&VertexInfo::desc, G);
            ANCpmap ANC = get(&VertexInfo::anc, G);
            for(int i = 0; i  < num_vertices; i++) {
                ANC[temp[i]].root.content = temp[i];
                DESC[temp[i]].root.content = temp[i];
            }

            // Allocate - Initialize arrays
            FORWARD = new Vertex* [num_vertices];
            BACKWARD = new Vertex* [num_vertices];
            D = new int* [num_vertices];
            for(int i=0; i < num_vertices; i++) {
                FORWARD[i] = new Vertex [num_vertices];
                BACKWARD[i] = new Vertex [num_vertices];
                D[i] = new int[num_vertices];
                for(int j=0; j < num_vertices; j++) {
                    FORWARD[i][j] = NULL_VERTEX;
                    BACKWARD[i][j] = NULL_VERTEX;
                    D[i][j] = INF;
                }
            }
        }

        ~AIMN91_DataStructure() {
            // Deallocate arrays
            for(int i=0; i < num_vertices; i++) {
                delete FORWARD[i];
                delete BACKWARD[i];
                delete D[i];
            }
            delete FORWARD;
            delete BACKWARD;
            delete D;
        }

        // Get vertices from graph G - adding new vertices should not be allowed
        Vertex get_vertex(int vertex_id) {
            return boost::vertex(vertex_id, this->G);
        }

        // length(x,y) operation simply returns the record in matrix D at position (x,y)
        int length(Vertex x, Vertex y) {
            // return D[x][y];
            return 0;
        }
        int length(int xid, int yid) {
            return D[xid][yid];
        }

        // minimal_path(x,y) operation returns the shortest path from x to y 
        std::vector<Vertex> minimal_path(Vertex x, Vertex y);

        // add(i,j,w) - adds the edge i->j with the cost of w
        // Need to check if the edge already exists, given it doesn't update the datastructure
        bool add(Vertex i, Vertex j, int w);

        // decrease(i,j,delta)
        // Check if edge exists, if it does check if D(i,j) - delta > 1
        // update the datastructure
        bool decrease(Vertex i, Vertex j, int w);
};

/* template<typename array_t, typename elem_t> 
void initialize_2dMatrix(array_t* arr, elem_t el, int n, int m) {
    for (int i=0; i < n; i++) 
        for(int j=0; j < m; j++)
            arr[i][j] = el;
} */