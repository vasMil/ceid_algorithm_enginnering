#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;
typedef boost::graph_traits<Graph>::out_edge_iterator OutEdgeIter;
typedef boost::graph_traits<Graph>::in_edge_iterator InEdgeIter;

struct GraphOper {
    Vertex operator() (Edge& e, Graph& G) const {
        return boost::target(e, G); 
    }
    std::pair<OutEdgeIter, OutEdgeIter> operator() (Vertex& v, Graph& G) const {
        return boost::out_edges(v, G);
        
    }
};

struct RevGraphOper {
    Vertex operator() (Edge& e, Graph& G) const {
        return boost::source(e, G); 
    }
    std::pair<InEdgeIter, InEdgeIter> operator() (Vertex& v, Graph& G) const {
        return boost::in_edges(v, G);
    }
};

template <class GraphOperations>
void printVert(GraphOperations go, Edge& e, Vertex& v, Graph& G){
    std::cout << "printVert with argument Edge: " << go(e, G) << std::endl;
    std::cout << "printVert with argument Vertex: " << *(go(v, G).first) << std::endl;
}

int main() {
    // Init
    Graph G;
    Edge e12, e23;
    Vertex v1, v2, v3;
    bool succ_e;

    // Construct trivial graph
    v1 = boost::add_vertex(G);
    v2 = boost::add_vertex(G);
    v3 = boost::add_vertex(G);
    boost::tie(e12, succ_e) = boost::add_edge(v1, v2, G);
    boost::tie(e23, succ_e) = boost::add_edge(v2, v3, G);

    // Use functor
    auto f = GraphOper();
    auto rf = RevGraphOper();

    printVert(f, e23, v2, G);
    std::cout << "Reverse graph operations" << std::endl;
    printVert(rf, e23, v2, G);
}