#include <stdio.h>
#include <string>
#include <vector>
#include <limits>

/* Used by most header files */
#include <boost/tuple/tuple.hpp>
/* Used by graphGenerators.h */
#include <boost/random/mersenne_twister.hpp>
#include <boost/graph/random.hpp>

struct NodeInfo;
struct EdgeInfo;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS, NodeInfo, EdgeInfo> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;

typedef boost::graph_traits<Graph>::edge_iterator EdgeIter;
typedef boost::graph_traits<Graph>::vertex_iterator VertexIter;
typedef boost::graph_traits<Graph>::out_edge_iterator OutEdgeIter;
typedef boost::graph_traits<Graph>::in_edge_iterator InEdgeIter;

Graph nullGraph;

struct NodeInfo {
    EdgeIter pred;
    unsigned int dist;
    unsigned int lowerBound; // Will be used in A*
    NodeInfo() {
        this->dist = std::numeric_limits<unsigned int>::max();
        this->lowerBound = std::numeric_limits<unsigned int>::max();
        this->pred = boost::edges(nullGraph).second;
    }
};

struct EdgeInfo {
    unsigned int cost;
};

typedef boost::property_map<Graph, EdgeIter NodeInfo::*>::type PredPMap;

int main() {
    Graph G;
    Vertex v1, v2, v3, u, v;
    Edge e12, e23;
    bool succ_e;
    EdgeIter first, last;
    VertexIter vfirst, vlast;
    v1 = boost::add_vertex(G);
    v2 = boost::add_vertex(G);
    v3 = boost::add_vertex(G);
    boost::tie(e12, succ_e) = boost::add_edge(v1, v2, G);
    boost::tie(e23, succ_e) = boost::add_edge(v2, v3, G);

    PredPMap pred = boost::get(&NodeInfo::pred, G);
    for(boost::tie(first, last) = boost::edges(G); first != last; ++first) {
        u = boost::source(*first, G);
        v = boost::target(*first, G);
        pred[v] = first;
        // std::cout << v << ".pred = " << *(pred[v]) << std::endl;
    }
    for(boost::tie(vfirst, vlast) = boost::vertices(G); vfirst != vlast; ++vfirst) {
        std::cout << *vfirst << ".pred = " << *(pred[*vfirst]) << std::endl;
    }
}