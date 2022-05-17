#include "utils.h"
#include "graphGenerators.h"

int main() {
    Graph G;
    CostPMap cost = get(&EdgeInfo::cost, G);
    randomGraph(G, cost, 10, 10, 0, 1000);
    
    write_graphviz(std::cout, G, boost::default_writer(), boost::make_label_writer(cost));
    return 0;
}