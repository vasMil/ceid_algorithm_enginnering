#include "utils.h"

template <typename GraphOperations_t>
bool Dijkstra_SP(
    Graph& G, Vertex& s, Vertex& t, CostPMap& cost, PredPMap& pred, DistPMap& dist, int& iter_cnt, 
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
    int cur_vert_dist = 0;
    int adj_vert_dist = 0;
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
std::vector<std::pair<Vertex, int> > getDistance_Landmark(Graph& G, Vertex& L, GraphOperations_t& functor) {
    VertexIter first, last;
    std::vector<std::pair<Vertex, int> > distPairVec(boost::num_vertices(G));
    CostPMap cost = boost::get(&EdgeInfo::cost, G);
    PredPMap pred = boost::get(&NodeInfo::pred, G);
    DistPMap dist = boost::get(&NodeInfo::dist, G);
    int cnt = 0, i = 0;

    // Run Dijkstra with the landmark being the starting point - save dist in NodeInfo::dist
    Dijkstra_SP(G, L, NULL_VERTEX, cost, pred, dist, cnt, functor);

    // Extract info and reset NodeInfo::dist
    // I also reset NodeInfo::pred as a good practice, since it is only used for writing and not reading
    // on the preprocessing state
    for(boost::tie(first, last) = boost::vertices(G); first != last; ++first) {
        distPairVec[i] = std::make_pair(*first, dist[*first]);
        dist[*first] = std::numeric_limits<int>::max();
        pred[*first] = NULL_EDGE;
        ++i;
    }
    return distPairVec;
}

Vertex getFurthestAwayVertex(std::vector<std::pair<Vertex, int> >& distFromL, std::vector<std::pair<Vertex, int> >& distToL) {
    Vertex v = distToL[0].first;
    int max_dist = distFromL[0].second;
    int max_dist_rev = distToL[0].second;

    for (int i=1; i < distFromL.size(); i++) {
        if (max_dist == std::numeric_limits<int>::max() && distFromL[i].second == std::numeric_limits<int>::max()) {
            if (max_dist_rev < distToL[i].second) {
                v = distToL[i].first;
                max_dist_rev = distToL[i].second;
            }
            continue;
        }
        if (max_dist_rev == std::numeric_limits<int>::max() && distToL[i].second == std::numeric_limits<int>::max()) {
            if (max_dist < distFromL[i].second) {
                v = distFromL[i].first;
                max_dist = distFromL[i].second;
            }
            continue;
        }
        // Same as: max_dist + max_dist_rev < distFromL[i].second + distToL[i].second
        // This way I do not need to check for overflows
        if (max_dist - distFromL[i].second < distToL[i].second - max_dist_rev) {
            v = distFromL[i].first;
            max_dist = distFromL[i].second;
            max_dist_rev = distToL[i].second;
        }
    }

    return v;
}


void prep_A_star(Graph& G) {
    // Let the first landmark be the first vertex in the (first dimension of the) adjacency_list;
    Vertex L1 = *(boost::vertices(G).first);
    auto distFromL1 = getDistance_Landmark(G, L1, GraphOper::getInstance());
    auto distToL1 = getDistance_Landmark(G, L1, RevGraphOper::getInstance());
    
    for (int i = 0; i < distFromL1.size(); ++i) {
        std::cout << i << ".distL1 = " << distFromL1[i].second << std::endl;
    }
    for (int i = 0; i < distToL1.size(); ++i) {
        std::cout << i << ".rev_distL1 = " << distToL1[i].second << std::endl;
    }
    
    Vertex L2 = getFurthestAwayVertex(distFromL1, distToL1);
    auto distFromL2 = getDistance_Landmark(G, L2, GraphOper::getInstance());
    auto distToL2 = getDistance_Landmark(G, L2, RevGraphOper::getInstance());
    
    // for (int i = 0; i < distFromL2.size(); ++i) {
    //     std::cout << i << ".distL2 = " << distFromL2[i].second << std::endl;
    // }
    // for (int i = 0; i < distToL2.size(); ++i) {
    //     std::cout << i << ".rev_distL2 = " << distToL2[i].second << std::endl;
    // }
    std::cout << "\n\n" << "L2 = " << L2 << std::endl;
}