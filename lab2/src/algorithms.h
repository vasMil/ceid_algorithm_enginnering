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
            adj_vert_dist = cur_vert_dist + cost[e];
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
            pred[v] = e;
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
        dist[*first] = INIT_DIST;
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
        if (max_dist == INIT_DIST && distFromL[i].second == INIT_DIST) {
            if (max_dist_rev < distToL[i].second) {
                v = distToL[i].first;
                max_dist_rev = distToL[i].second;
            }
            continue;
        }
        if (max_dist_rev == INIT_DIST && distToL[i].second == INIT_DIST) {
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


void prep_A_star(Graph& G, Vertex& t) {
    int i = 0;
    int d_L1t, d_tL1, d_L2t, d_tL2;
    LowerBoundPMap lbPMap = boost::get(&NodeInfo::lowerBound, G);
    CostPMap cost = boost::get(&EdgeInfo::cost, G);
    VertexIter vit, vit_end;
    EdgeIter eit, eit_end;
    Vertex u, v;

    // Let the first landmark be the first vertex in the (first dimension of the) adjacency_list;
    Vertex L1 = *(boost::vertices(G).first);
    auto distFromL1 = getDistance_Landmark(G, L1, GraphOper::getInstance());
    auto distToL1 = getDistance_Landmark(G, L1, RevGraphOper::getInstance());
    
    Vertex L2 = getFurthestAwayVertex(distFromL1, distToL1);
    auto distFromL2 = getDistance_Landmark(G, L2, GraphOper::getInstance());
    auto distToL2 = getDistance_Landmark(G, L2, RevGraphOper::getInstance());
 
    // Get d(L, t) and d(t, L) for each of the landmarks
    d_L1t = distFromL1[t].second;
    d_tL1 = distToL1[t].second;
    d_L2t = distFromL2[t].second;
    d_tL2 = distToL2[t].second;

    // Calculate the lower bounds
    for(boost::tie(vit, vit_end) = boost::vertices(G); vit != vit_end; ++vit) {
        lbPMap[*vit] = std::max({
            distToL1[i].second - d_tL1,
            d_L1t - distFromL1[i].second,
            distToL2[i].second - d_tL2,
            d_L2t - distFromL2[i].second
        });
        i++;
    }

    // Update EdgeInfo::cost
    for(boost::tie(eit, eit_end) = boost::edges(G); eit != eit_end; ++eit) {
        u = boost::source(*eit, G);
        v = boost::target(*eit, G);
        // Do not allow negative costs!
        if(cost[*eit] + lbPMap[v] - lbPMap[u] < 0){
            throw std::runtime_error("A* prerpocessing calculated negative cost for an Edge");
        }
        cost[*eit] = cost[*eit] + lbPMap[v] - lbPMap[u];
    }
    return;
}

// Fixes distances and edge costs by factoring out the heuristic
void postp_A_star(Graph& G, Vertex& s) {
    EdgeIter eit, eit_end;
    VertexIter vit, vit_end;
    Vertex v, u;
    LowerBoundPMap lb = boost::get(&NodeInfo::lowerBound, G);
    CostPMap cost = boost::get(&EdgeInfo::cost, G);
    DistPMap dist = boost::get(&NodeInfo::dist, G);
    PredPMap pred = boost::get(&NodeInfo::pred, G);

    // Fix distances
    for(boost::tie(vit, vit_end) = boost::vertices(G); vit != vit_end; ++vit) {
        dist[*vit] = dist[*vit] + lb[s] - lb[*vit];
    }

    // Fix edge costs
#if POSTPROCESS_EDGE_COSTS
    for(boost::tie(eit, eit_end) = boost::edges(G); eit != eit_end; ++eit) {
        u = boost::source(*eit, G);
        v = boost::target(*eit, G);
        cost[*eit] = cost[*eit] - lb[v] + lb[u];
    }
#endif
}