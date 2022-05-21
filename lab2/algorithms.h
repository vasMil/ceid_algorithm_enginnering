#include "utils.h"

template <typename GraphOperations_t>
bool Dijkstra_SP(
    Graph& G, Vertex& s, Vertex& t, CostPMap& cost, PredPMap& pred, DistPMap& dist, unsigned int& iter_cnt, 
    GraphOperations_t& functor
    ) {
    // Setup CompareNode struct for the priority queue
    CompareNodes::pmap_dist = dist;

    // Initialize the priority queue
    VertexBH PQ;

    // Parallel helper vectors
    std::vector<VBH_handle_t> handles(boost::num_vertices(G));
    std::vector<bool> visited(boost::num_vertices(G), false);

    // Helper variables
    Vertex u, v;
    Edge e;
    unsigned int cur_vert_dist = 0;
    unsigned int adj_vert_dist = 0;
    // Get the correct iterator type, depending on which operation SINGLETON is passed as an argument
    auto out_eit  = GraphOperations_t::it, out_eit_end = GraphOperations_t::it;

    // Update distance of the starting Vertex to 0 and push it in PQ.
    CompareNodes::pmap_dist[s] = 0;
    visited[s] = true;
    handles[s] = PQ.push(s);

    while(!PQ.empty()) {
        u = PQ.top(); PQ.pop();
        iter_cnt++;
        if (u == t) return true;
        
        cur_vert_dist = CompareNodes::pmap_dist[u];
        for (boost::tie(out_eit, out_eit_end) = functor.adj_edges(u, G); out_eit != out_eit_end; ++out_eit) {
            e = *out_eit;
            v = functor.opposite(e, G);
            adj_vert_dist = cur_vert_dist + cost[*out_eit];
            if (!visited[v]) {
                visited[v] = true;
                CompareNodes::pmap_dist[v] = adj_vert_dist;
                handles[v] = PQ.push(v);
            }
            else if (adj_vert_dist < CompareNodes::pmap_dist[v]) {
                CompareNodes::pmap_dist[v] = adj_vert_dist;
                // As distance decreases -> priority increases
                // trivial example: https://coliru.stacked-crooked.com/a/b7ea797e74d4b0ad
                PQ.increase(handles[v]);
            }
            else {
                continue;
            }
            pred[v] = *out_eit;
        }
    }
    
    return false;
}

template <typename GraphOperations_t>
std::vector<unsigned int> getDistance_Landmark(Graph& G, Vertex& L, GraphOperations_t& functor) {
    VertexIter first, last;
    std::vector<unsigned int> distVec(boost::num_vertices(G));
    CostPMap cost = boost::get(&EdgeInfo::cost, G);
    PredPMap pred = boost::get(&NodeInfo::pred, G);
    DistPMap dist = boost::get(&NodeInfo::dist, G);
    unsigned int cnt = 0, i = 0;

    // Run Dijkstra with the landmark being the starting point - save dist in NodeInfo::dist
    Dijkstra_SP(G, L, NULL_VERTEX, cost, pred, dist, cnt, functor);

    // Extract info and reset NodeInfo::dist
    // I also reset NodeInfo::pred as a good practice, since it is only used for writing and not reading
    // on the preprocessing state
    for(boost::tie(first, last) = boost::vertices(G); first != last; ++first) {
        distVec[i] = dist[*first];
        dist[*first] = std::numeric_limits<unsigned int>::max();
        pred[*first] = NULL_EDGE;
        ++i;
    }
    return distVec;
}


void prep_A_star(Graph& G) {
    Vertex L = *(boost::vertices(G).first);
    auto distFromL = getDistance_Landmark(G, L, GraphOper::getInstance());
    for (int i = 0; i < distFromL.size(); ++i) {
        std::cout << i << ".dist = " << distFromL[i] << std::endl;
    }
    auto distToL = getDistance_Landmark(G, L, RevGraphOper::getInstance());
    for (int i = 0; i < distToL.size(); ++i) {
        std::cout << i << ".rev_dist = " << distToL[i] << std::endl;
    }
}