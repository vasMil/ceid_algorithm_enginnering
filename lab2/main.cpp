#include "utils.h"
#include "graphGenerators.h"
#include "algorithms.h"


int main() {
    // Create a random graph G
    Graph G, BG;
    // Get the property maps
    CostPMap cost = boost::get(&EdgeInfo::cost, G);
    PredPMap pred = boost::get(&NodeInfo::pred, G);
    DistPMap dist = boost::get(&NodeInfo::dist, G);

    randomGraph(G, cost, 10, 15, 0, 1000);
    BG = G;
    // Print the random graph
    write_graphviz(std::cout, G, boost::default_writer(), boost::make_label_writer(cost));
    
    // Test Dijkstra_SP
    int cnt = 0;
    VertexIter first, last;
    boost::tie(first, last) = vertices(G);
    Vertex s = *first, t = *(--last);
    bool isPath = Dijkstra_SP(G, s, t, cost, pred, cnt);

    if (!isPath) return 0;
    std::cout << "Visited " << cnt << " vertices" << std::endl;
    std::cout << "The path: ";
    while (s != t) {
        std::cout << t << " -> ";
        t = boost::source(pred[t], G);
    }
    std::cout << s << std::endl;
    return 0;
}