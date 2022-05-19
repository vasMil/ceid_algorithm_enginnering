#include "utils.h"
#include "graphGenerators.h"

bool Dijkstra_SP(Graph& G, Vertex& s, Vertex& t, CostPMap& cost, PredPMap& pred, int& iter_cnt) {
    // Setup CompareNode struct for the priority queue
    CompareNodes::pmap_dist = get(&NodeInfo::dist, G);

    // Initialize the priority queue
    VertexBH PQ;

    // Parallel helper vectors
    std::vector<VBH_handle_t> handles(num_vertices(G));
    std::vector<bool> visited(num_vertices(G), false);

    // Helper variables
    Vertex u, v;
    OutEdgeIter out_eit, out_eit_end;
    int cur_vert_dist = 0;
    int adj_vert_dist = 0;

    // Update distance of the starting Vertex to 0 and push it in PQ.
    CompareNodes::pmap_dist[s] = 0;
    handles[s] = PQ.push(s);

    while(!PQ.empty()) {
        u = PQ.top(); PQ.pop();
        iter_cnt++;
        if (u == t) return true;
        
        cur_vert_dist = CompareNodes::pmap_dist[u];
        for (boost::tie(out_eit, out_eit_end) = boost::out_edges(u, G); out_eit != out_eit_end; ++out_eit) {
            v = boost::target(*out_eit, G);
            adj_vert_dist = cur_vert_dist + cost[*out_eit];
            if (!visited[v] && v != s) {
                visited[v] = true;
                CompareNodes::pmap_dist[v] = adj_vert_dist;
                handles[v] = PQ.push(v);
            }
            else if (adj_vert_dist < CompareNodes::pmap_dist[v]) {
                CompareNodes::pmap_dist[v] = adj_vert_dist;
                PQ.decrease(handles[v]);
            }
            else {
                continue;
            }
            pred[v] = *out_eit;
        }
    }
    
    return false;
}