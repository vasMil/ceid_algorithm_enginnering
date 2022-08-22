#include "aliases.h"

class AIMN91_DataStructure {
    public:
        int num_vertices;
        int num_edges;
        int upperCostBound;
        static const int lowerCostBound = MIN_C;
        
        // You are better off using arrays instead of vectors.
        // Arrays use less memory and vectors are better with frequent
        // insertions and deletions which I am not going to be using
        unsigned int** D; // The distance matrix
        
        // The FORWARD Matrix - initially containing all NULL
        Node<Vertex>* ** FORWARD;
        // The BACKWARD Matrix - initially containing all NULL
        Node<Vertex>* ** BACKWARD;
        // Initialize the directed Graph
        Graph G;
        // Property maps
        Vertex_desc_pmap DESC;
        Vertex_anc_pmap ANC;

        // merging updateForward and updateBackward
        void updateForward(Vertex x, Vertex i, Vertex j, DLTree<Vertex>& T, Node<Vertex>* T_id_guide[]) {
            Vertex y, w;
            std::queue<Vertex> Q;
            DLTree<Vertex> N;
            Node<Vertex>* N_id_guide[num_vertices];

            Q.push(j);
            while (!Q.empty()) {
                y = Q.front();
                Q.pop();
                if(D[x][i] + 1 + D[j][y] < D[x][y]) {
                    if (y == j) {
                        FORWARD[x][j] = DESC[x].addChild(FORWARD[x][i], j);
                        N_id_guide[j] = N.addChild(NULL, j);
                    }
                    else {
                        // The parent of y in the tree DESC(j) is a different Node from the Node in DESC(x).
                        // FORWARD[j][y] is a pointer to a Node containing y in tree DESC(j)
                        // thus FORWARD[j][y]->parent->content returns the id of the parent of y
                        // I may use that id to locate the Node in DESC(x) that has as content the same value (id)
                        FORWARD[x][y] = DESC[x].addChild(FORWARD[x][FORWARD[j][y]->parent->content], y);
                        N_id_guide[y] = N.addChild(N_id_guide[FORWARD[j][y]->parent->content], y);
                    }
                }
                D[x][y] = D[x][i] + 1 + D[j][y];
                auto ch_it = T_id_guide[y]->children.begin(); auto ch_end = T_id_guide[y]->children.end();
                for ( ; ch_it != ch_end; ch_it++) {
                    Q.push((*ch_it)->content);
                }
            }
            if (!N.empty()) {
                auto eit = ANC[i].edges().first; auto eend = ANC[i].edges().second;
                for( ; eit != eend; eit++) {
                    if((*eit).first->content == x) {
                        updateForward((*eit).second->content, i, j, N, N_id_guide);
                    }
                }
            }
        }
        
        void updateBackward(Vertex x, Vertex i, Vertex j, DLTree<Vertex>& T, Node<Vertex>* T_id_guide[]);
    public:        
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
            for(int i=0; i < num_vertices; i++) {
                boost::add_vertex(G);
            }


            // Allocate - Initialize arrays
            FORWARD = new Node<Vertex>** [num_vertices];
            BACKWARD = new Node<Vertex>** [num_vertices];
            D = new unsigned int* [num_vertices];
            for(int i=0; i < num_vertices; i++) {
                FORWARD[i] = new Node<Vertex>* [num_vertices];
                BACKWARD[i] = new Node<Vertex>* [num_vertices];
                D[i] = new unsigned int[num_vertices];
                for(int j=0; j < num_vertices; j++) {
                    if (i == j) {
                        D[i][j] = 0;
                        continue;
                    }
                    FORWARD[i][j] = NULL;
                    BACKWARD[i][j] = NULL;
                    D[i][j] = INF;
                }
            }

            // Initialize property maps
            DESC = boost::get(&VertexInfo::desc, G);
            ANC = boost::get(&VertexInfo::anc, G);
            VertexIter vit, vend;
            for(boost::tie(vit, vend) = boost::vertices(G); vit != vend; vit++) {
                FORWARD[*vit][*vit] = ANC[*vit].addChild(NULL, *vit);
                BACKWARD[*vit][*vit] = DESC[*vit].addChild(NULL, *vit);
            }
        }

        ~AIMN91_DataStructure() {
            // Deallocate arrays
            for(int i=0; i < num_vertices; i++) {
                delete[] FORWARD[i];
                delete[] BACKWARD[i];
                delete[] D[i];
            }
            delete[] FORWARD;
            delete[] BACKWARD;
            delete[] D;
        }

        // Get vertices from graph G - adding new vertices should not be allowed
        Vertex get_vertex(int vertex_id) {
            return boost::vertex(vertex_id, this->G);
        }

        // length(x,y) operation simply returns the record in matrix D at position (x,y)
        unsigned int length(Vertex x, Vertex y) {
                return D[x][y];
        }

        // minimal_path(x,y) operation returns the shortest path from x to y 
        std::vector<Vertex> minimal_path(Vertex x, Vertex y);

        // add(i,j,w) - adds the edge i->j with the cost of w
        // Need to check if the edge already exists, given it doesn't update the datastructure
        void add(Vertex i, Vertex j, int w) {
            boost::add_edge(i, j, G);
            updateBackward(j, j, i, ANC[j], BACKWARD[j]);
            updateForward(i, i, j, DESC[j], FORWARD[j]);
        }

        // decrease(i,j,delta)
        // Check if edge exists, if it does check if D(i,j) - delta > 1
        // update the datastructure
        bool decrease(Vertex i, Vertex j, int w);
};


void AIMN91_DataStructure::updateBackward(Vertex x, Vertex i, Vertex j, DLTree<Vertex>& T, Node<Vertex>* T_id_guide[]) {
    Vertex y, w;
    std::queue<Vertex> Q;
    DLTree<Vertex> N;
    Node<Vertex>* N_id_guide[num_vertices];

    Q.push(j);
    while (!Q.empty()) {
        y = Q.front();
        Q.pop();
        if(D[x][i] + 1 + D[j][y] < D[x][y]) {
            if (y == j) {
                BACKWARD[x][j] = ANC[x].addChild(BACKWARD[x][i], j);
                N_id_guide[x] = N.addChild(NULL, x);
            }
            else {
                BACKWARD[x][y] = ANC[x].addChild(BACKWARD[x][BACKWARD[j][y]->parent->content], y);
                N_id_guide[y] = N.addChild(N_id_guide[BACKWARD[j][y]->parent->content], y);
            }
        }
        auto ch_it = T_id_guide[y]->children.begin(); auto ch_end = T_id_guide[y]->children.end();
        for ( ; ch_it != ch_end; ch_it++) {
            Q.push((*ch_it)->content);
        }
    }
    if (!N.empty()) {
        auto eit = DESC[i].edges().first; auto eend = DESC[i].edges().second;
        for( ; eit != eend; eit++) {
            if((*eit).first->content == x) {
                updateBackward((*eit).second->content, j, i, N, N_id_guide);
            }
        }
    }
}