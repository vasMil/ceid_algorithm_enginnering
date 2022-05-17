#include <stdio.h>
#include <string>
#include <iostream>
#include <boost/random/mersenne_twister.hpp>
#include <boost/graph/random.hpp>
#include <boost/tuple/tuple.hpp>

#include <boost/graph/graphviz.hpp>

struct NodeInfo;
struct EdgeInfo;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, NodeInfo, EdgeInfo> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;

struct NodeInfo {
    int dist_to_t;
};

struct EdgeInfo {
    int cost;
};

typedef boost::property_map<Graph, int EdgeInfo::*>::type CostPMap;


