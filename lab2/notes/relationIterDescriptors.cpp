#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <boost/tuple/tuple.hpp>
#include <boost/graph/adjacency_list.hpp>

struct NodeInfo;
struct EdgeInfo;

// Takeaway: When dereferencing a Vertex iterator you get the descriptor of that Vertex
// If you are using a vector to save the Vertices of the graph, then that descriptor is an index
// If you are using a list, the descriptor is an address.

typedef boost::adjacency_list<boost::listS, boost::listS, boost::bidirectionalS, NodeInfo> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;

typedef boost::graph_traits<Graph>::edge_iterator EdgeIter;
typedef boost::graph_traits<Graph>::vertex_iterator VertexIter;

struct NodeInfo {
    int id;
    NodeInfo() {
        this->id = -1;
    }
};

typedef boost::property_map<Graph, int NodeInfo::*>::type IdPMap;

int main() {
    Graph G;
    Vertex v1 = boost::add_vertex(G);
    Vertex v2 = boost::add_vertex(G);
    Vertex v3 = boost::add_vertex(G);
    Edge e12, e23;
    bool succ_e;
    boost::tie(e12, succ_e) = boost::add_edge(v1, v2, G);
    boost::tie(e23, succ_e) = boost::add_edge(v2, v3, G);
    
    // Vertex v, u;
    // Edge e;
    IdPMap idMap = boost::get(&NodeInfo::id, G);
    VertexIter first, last;
    for(boost::tie(first, last) = boost::vertices(G); first != last; ++first) {
        std::cout << "VertexDescriptor: " << *first << std::endl;
        std::cout << "AccessNodeInfo: " << idMap[*first] << std::endl;
    }
}