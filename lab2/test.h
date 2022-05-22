#include "utils.h"
#include <limits>

int test_Dijkstra(int n_vertices, int m_edges) {
    std::cout << "\n...Running test_Dijkstra...\n" << std::endl;
    bool predFlag = false, distFlag = false;
    // Create a random graph G
    Graph G;
    // Get the property maps
    CostPMap cost = boost::get(&EdgeInfo::cost, G);
    PredPMap pred = boost::get(&NodeInfo::pred, G);
    DistPMap dist = boost::get(&NodeInfo::dist, G);

    randomGraph(G, cost, n_vertices, m_edges, 0, 1000);
    
    // Env for Dijkstra_SP
    int cnt = 0, i = 0, cnt_same_pred = 0, cnt_same_dist = 0, cnt_difInit_pred = 0, cnt_difInit_dist = 0;
    VertexIter first, last;
    boost::tie(first, last) = boost::vertices(G);
    // t is intentionally pointing to an edge outside of the Graph,
    // so Dijkstra_SP does not return prematurely.
    Vertex s = *first, t = *last;

    // Setup env for boost::dijkstra_shortest_paths
    WeightPMap wemap = get(&EdgeInfo::cost, G);
    std::vector<Vertex> boost_pred(boost::num_vertices(G));
    std::vector<int> boost_dist(boost::num_vertices(G));

    // Run the two
    bool isPath = Dijkstra_SP(G, s, t, cost, pred, dist, cnt, GraphOper::getInstance());
    boost::dijkstra_shortest_paths(G, s, boost::predecessor_map(&boost_pred[0]).distance_map(
        // Get an iterator type out of the vector.
        // TODO: Take a deeper look at what boost::make_iterator_property_map() does, 
        // source: https://stackoverflow.com/questions/28973152/boostgraph-dijkstra-and-custom-objects-and-properties/28975489#28975489
        boost::make_iterator_property_map(
            boost_dist.begin(), get(boost::vertex_index, G)
            )).weight_map(wemap));
    
    // Check if the output of the two match
    for(i = 0; first != last; ++first) {
        Vertex myPredVert = boost::source(pred[*first], G), boostPredVert =  boost_pred[i];
        int myDist = dist[*first], boostDist = boost_dist[i];
        // Boost initializes the predecessor property map to be equal to vertex_index
        // Thus, if a node is not accessible from s, the boost_pred == i
        // In my impl the predecessor Edge will have an unspecified value
        if (myPredVert != boostPredVert && boostPredVert != i) {
            std::cout << "i = " << i << " Boost: " << boostPredVert << ", myImpl: " << myPredVert << std::endl;
            predFlag = true;
        }
        else if (myPredVert == boostPredVert) cnt_same_pred++;
        else if (boostPredVert == i) cnt_difInit_pred++;

        if (myDist != boostDist && boostDist != std::numeric_limits<int>::max()) {
            std::cout << "i = " << i << " Boost_dist: " << boostDist << ", myImpl_dist: " << myDist << std::endl;
            distFlag = true;
        }
        else if (myDist == boostDist) cnt_same_dist++;
        else if (boostDist == std::numeric_limits<int>::max()) cnt_difInit_dist++;

        ++i;
    }
    if(predFlag  || distFlag) {
        std::cout << "Dijkstra_test: FAILED" << std::endl;
        return false;
    }
    std::cout << "Dijkstra_test succeeded on random graph with " << n_vertices << " Vertices and " << m_edges << " Edges" << std::endl;
    std::cout << "Vertices with same predecessor: " << cnt_same_pred << std::endl;
    std::cout << "Vertices with different predecessor due to initialization difference: " << cnt_difInit_pred << std::endl;
    std::cout << "Vertices with same distance: " << cnt_same_dist << std::endl;
    std::cout << "Vertices with different distance due to initialization difference: " << cnt_difInit_dist << std::endl;
    std::cout << "\nNote: Vertices with different distance/predecessor due to initialization difference"
        "implies that the vertices are not accessible from the starting Vertex s" << std::endl;
    std::cout << "\n...test_Dijkstra... Done" << std::endl;
    return true;
}

void test_printSmallGraph_DijkstraSP(int n_vertices, int m_edges) {
    std::cout << "\n...Running test_printSmallGraph_DijkstraSP...\n" << std::endl;

    // Declaire graph G
    Graph G;
    // Get the property maps
    CostPMap cost = boost::get(&EdgeInfo::cost, G);
    PredPMap pred = boost::get(&NodeInfo::pred, G);
    DistPMap dist = boost::get(&NodeInfo::dist, G);

    // Create the random graph
    randomGraph(G, cost, n_vertices, m_edges, 0, 1000);

    // Print the graph
    boost::write_graphviz(std::cout, G, boost::default_writer(), boost::make_label_writer(cost));
    std::cout << std::endl;

    // Test by printing the SP from first Vertex to last Vertex of the graph
    int cnt = 0, totSPcost = 0;
    VertexIter first, last;
    boost::tie(first, last) = boost::vertices(G);
    Vertex s = *first, t = *(--last);
    
    bool isPath = Dijkstra_SP(G, s, t, cost, pred, dist, cnt, GraphOper::getInstance());

    if (!isPath) {
        std::cout << "There is no path from s to t" << std::endl;
        std::cout << "\n...test_printSmallGraph_DijkstraSP... Done" << std::endl;
        return;
    }
    std::cout << "Visited " << cnt << " vertices" << std::endl;
    std::cout << "The shortest path found: ";
    while (s != t) {
        std::cout << t << " <- ";
        totSPcost += cost[pred[t]];
        t = boost::source(pred[t], G);
    }
    std::cout << s << std::endl;
    std::cout << "Total cost of the path calculated in test: " << totSPcost << std::endl;
    std::cout << "Total cost of the path using dist bundled property: " << dist[*(--boost::vertices(G).second)] << std::endl;
    std::cout << "\n...test_printSmallGraph_DijkstraSP... Done" << std::endl;
    return;
}