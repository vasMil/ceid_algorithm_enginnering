#pragma once
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
        void updateForward_and_Backward(
            Vertex x, Vertex i, Vertex j, unsigned int w, 
            Node<Vertex>* T_id_guide[])
        {
            Vertex y;
            std::queue<Vertex> Q;
            DLTree<Vertex> N;
            // A vector that contains pointers to DLTree Nodes
            Node<Vertex>* N_id_guide[num_vertices];
            for (int i = 0; i < num_vertices; i++) {
                N_id_guide[i] = NULL;
            }

            Q.push(j);
            while (!Q.empty()) {
                y = Q.front();
                Q.pop();
                if(D[x][i] + w + D[j][y] < D[x][y]) {
                // A shorter path is found
                    if (y == j) {
                    // Insert y into DESC[x] as the child of i
                        if (FORWARD[x][j] != NULL){
                        // y was already in DESC[x], thus it needs to be removed before inserting it
                        // to its new location
                            DESC[x].removeChild(FORWARD[x][j]);
                            N.removeChild(N_id_guide[j]);
                        }
                        FORWARD[x][j] = DESC[x].addChild(FORWARD[x][i], j);
                        N_id_guide[j] = N.setRoot(j);
                    }
                    else {
                    // Insert y into DESC[x] as a child of the same parent it has in DESC[j]
                        if (FORWARD[x][y] != NULL) {
                            DESC[x].removeChild(FORWARD[x][y]);
                            N.removeChild(N_id_guide[y]);
                        }
                        // The parent of y in the tree DESC(j) is a different Node from the Node in DESC(x).
                        // FORWARD[j][y] is a pointer to a Node containing y in tree DESC(j)
                        // thus FORWARD[j][y]->parent->content returns the id of the parent of y
                        // I may use that id to locate the Node in DESC(x) that has as content the same value (id)
                        FORWARD[x][y] = DESC[x].addChild(FORWARD[x][FORWARD[j][y]->parent->content], y);
                        N_id_guide[y] = N.addChild(N_id_guide[FORWARD[j][y]->parent->content], y);
                    }
                    if (x == i) {
                        if (BACKWARD[y][i] != NULL){
                            ANC[y].removeChild(BACKWARD[y][i]);
                        }
                        BACKWARD[y][i] = ANC[y].addChild(BACKWARD[y][j], i);
                    }
                    else {
                        if (BACKWARD[y][x] != NULL) {
                            ANC[y].removeChild(BACKWARD[y][x]);
                        }
                        BACKWARD[y][x] = ANC[y].addChild(BACKWARD[y][BACKWARD[i][x]->parent->content], x);
                    }
                    D[x][y] = D[x][i] + w + D[j][y];
                    // Examine all the children of y in T in a recursive fashion (pseudocode uses a while loop)
                    auto ch_it = T_id_guide[y]->children.begin(); auto ch_end = T_id_guide[y]->children.end();
                    for ( ; ch_it != ch_end; ch_it++) {
                        Q.push((*ch_it)->content);
                    }
                }
            }
            if (!N.empty()) {
                auto eit = ANC[i].edges().first; auto eend = ANC[i].edges().second;
                for( ; eit != eend; eit++) {
                    if((*eit).first->content == x) {
                        updateForward_and_Backward((*eit).second->content, i, j, w, N_id_guide);
                    }
                }
            }
        }
        
    public: 
        // Create an empty graph with num_vertices vertices
        AIMN91_DataStructure(int num_vertices, int max_cost = MIN_C) {
            // Make sure that matrix D has cost values < INF, so INF implies there is no path
            if((num_vertices-1) > INF / MAX_C) {
                throw std::invalid_argument("There might be a path that has a cost > constant INF");
            }

            this->num_vertices = num_vertices;
            this->num_edges = 0;
            this->upperCostBound = max_cost <= MIN_C ? MAX_C : max_cost;

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
                    D[i][j] = INF;
                    FORWARD[i][j] = NULL;
                    BACKWARD[i][j] = NULL;
                }
            }

            // Initialize property maps
            DESC = boost::get(&VertexInfo::desc, G);
            ANC = boost::get(&VertexInfo::anc, G);
            VertexIter vit, vend;
            for(boost::tie(vit, vend) = boost::vertices(G); vit != vend; vit++) {
                FORWARD[*vit][*vit] = DESC[*vit].setRoot(*vit);
                BACKWARD[*vit][*vit] = ANC[*vit].setRoot(*vit);
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

        // Get G (used when VISUALIZATION == true)
        Graph* get_graph() {
            return &G;
        }

        // length(x,y) operation simply returns the record in matrix D at position (x,y)
        unsigned int safe_length(Vertex x, Vertex y) {
                if (x >= num_vertices || y >= num_vertices || x < 0 || y < 0 ) {
                    return -1;
                }
                return D[x][y];
        }

        unsigned int length(Vertex x, Vertex y) {
            return D[x][y];
        }

        // minpath(x,y) operation returns the shortest path from x to y 
        std::vector<Vertex> safe_minpath(Vertex x, Vertex y) {
            std::vector<Vertex> path;
            if (x >= num_vertices || y >= num_vertices || x < 0 || y < 0 ) {
                std::cout << "Invalid vertices" << std::endl;
                return path;
            }

            Node<Vertex>* t = BACKWARD[y][x];

            if (t == NULL) {
                return path;
            }

            while (t->parent != NULL) {
                path.push_back(t->content);
                t = t->parent;
            }
            path.push_back(t->content);
            
            return path;
        }

        std::vector<Vertex> minpath(Vertex x, Vertex y) {
            Node<Vertex>* t = BACKWARD[y][x];
            std::vector<Vertex> path;
            if (t == NULL) {
                return path;
            }

            while (t->parent != NULL) {
                path.push_back(t->content);
                t = t->parent;
            }
            path.push_back(t->content);
            
            return path;
        }

        // add(i,j,w) - adds the edge i->j with the cost of w
        // Need to check if the edge already exists, given it doesn't update the datastructure
        void safe_add(Vertex i, Vertex j, unsigned int w) {
            // Check if Vertex i and j are valid
            if (i >= num_vertices || j >= num_vertices || i < 0 || j < 0 ) {
                std::cout << "Invalid vertices" << std::endl;
                return;
            }

            // Check if w has a valid value            
            if(w > upperCostBound || w < MIN_C) {
                std::cout << "Add operation ignored!" << std::endl;
                std::cout << "w must be >= " << MIN_C << " and <= " << upperCostBound << std::endl;
                return;
            }

            // Check if edge already exists
            auto e = boost::edge(i, j, G);
            auto edge_cost_pmap = boost::get(&EdgeInfo::cost, G);
            if(e.second) {
                // Edge already exists
                std::cout << "Edge already exists!" << std::endl;
                if(edge_cost_pmap[e.first] > w) {
                    std::cout << "Looks like the new edge you are trying to add has a lower cost than the current one."
                     " Consider using the descrease(x,y,delta) operation." << std::endl;
                }
                return;
            }

            // Insert the edge into G
            this->add(i, j, w);
        }

        void add(Vertex i, Vertex j, unsigned int w) {
            auto e = boost::add_edge(i,j, G);
            boost::put(boost::get(&EdgeInfo::cost, G), e.first, w);
            this->num_edges++;
            updateForward_and_Backward(i, i, j, w, FORWARD[j]);
        }

        // decrease(i,j,delta)
        // Check if edge exists, if it does check if D(i,j) - delta > 1
        // update the datastructure
        void safe_decrease(Vertex i, Vertex j, unsigned int delta) {
            // Check if Vertex i and j are valid
            if (i >= num_vertices || j >= num_vertices || i < 0 || j < 0 ) {
                std::cout << "Invalid vertices" << std::endl;
                return;
            }
            // Check if edge already exists
            auto e = boost::edge(i, j, G);
            auto edge_cost_pmap = boost::get(&EdgeInfo::cost, G);
            if(!e.second) {
                std::cout << "No such edge, please add it using the add(x,y,w) operation!" << std::endl;
                return;
            }

            // Check if decreased cost is valid
            unsigned int decr_cost = edge_cost_pmap[e.first] - delta;
            if(decr_cost < MIN_C) {
                std::cout << "Decrease operation is ignored!" << std::endl;
                std::cout << "Decreasing the edge cost by " << delta << " will result to an edge cost of " <<
                    edge_cost_pmap[e.first] - delta << " which is less than min acceptable cost: " << MIN_C << std::endl;
                return;
            }

            this->decrease(i, j, delta);
        }

        void decrease(Vertex i, Vertex j, unsigned int delta) {
            // Get the edge
            auto e = boost::edge(i, j, G);
            auto edge_cost_pmap = boost::get(&EdgeInfo::cost, G);
            unsigned int decr_cost = edge_cost_pmap[e.first] - delta;
            // Remove the old edge
            boost::remove_edge(e.first, G);
            this->num_edges--;
            // Add the new edge
            this->add(i,j,decr_cost);
        }
};