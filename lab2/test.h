#include "utils.h"
#include <limits>

int test_Dijkstra(int n_vertices, int m_edges) {
    std::cout << "Running test_Dijkstra..." << std::endl;

    // Create a random graph G
    Graph G;
    // Get the property maps
    CostPMap cost = boost::get(&EdgeInfo::cost, G);
    PredPMap pred = boost::get(&NodeInfo::pred, G);
    DistPMap dist = boost::get(&NodeInfo::dist, G);

    randomGraph(G, cost, n_vertices, m_edges, 0, 1000);
    
    // Env for Dijkstra_SP
    int cnt = 0;
    VertexIter first, last;
    boost::tie(first, last) = boost::vertices(G);
    // t is intentionally pointing to an edge outside of the Graph,
    // so Dijkstra_SP does not return prematurely.
    Vertex s = *first, t = *last;

    // Setup env for boost::dijkstra_shortest_paths
    WeightPMap wemap = get(&EdgeInfo::cost, G);
    std::vector<Vertex> boost_pred(boost::num_vertices(G));
    std::vector<unsigned int> boost_dist(boost::num_vertices(G));

    // Run the two
    bool isPath = Dijkstra_SP(G, s, t, cost, pred, cnt);
    boost::dijkstra_shortest_paths(G, s, boost::predecessor_map(&boost_pred[0]).distance_map(
        // Get an iterator type out of the vector.
        // TODO: Take a deeper look at what boost::make_iterator_property_map() does, 
        // source: https://stackoverflow.com/questions/28973152/boostgraph-dijkstra-and-custom-objects-and-properties/28975489#28975489
        boost::make_iterator_property_map(
            boost_dist.begin(), get(boost::vertex_index, G)
            )).weight_map(wemap));
    
    // Check if the output of the two match
    for(int i = 0; first != last; ++first) {
        Vertex myPredVert = boost::source(pred[*first], G), boostPredVert =  boost_pred[i];
        unsigned int myDist = dist[*first], boostDist = boost_dist[i];
        // Boost initializes the predecessor property map to be equal to vertex_index
        // Thus, if a node is not accessible from s, the boost_pred == i
        // In my impl the predecessor Edge will have an unspecified value
        if (myPredVert != boostPredVert && boostPredVert != i) {
            std::cout << "i = " << i << " Boost: " << boostPredVert << ", myImpl: " << myPredVert << std::endl;
            return false;
        }
        if (myDist != boostDist && boostDist != std::numeric_limits<unsigned int>::max()) {
            std::cout << "i = " << i << " Boost_dist: " << boostDist << ", myImpl_dist: " << myDist << std::endl;
            return false;
        }
        ++i;
    }

    std::cout << "Dijkstra_test succeeded on random graph with " << n_vertices << " Vertices and " << m_edges << " Edges" << std::endl;
    return true;
}