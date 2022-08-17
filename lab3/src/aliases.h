#pragma once
#include <iostream>
#include <vector>
#include <limits.h>
#include <cstddef>
#include <stdexcept>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>

#define INF INT_MAX
#define MAX_C 100 // Max cost of an edge
#define MIN_C 1 // Min cost of an edge


/********* DLTree - 1 *********/
struct DLTVertex;
typedef boost::adjacency_list<boost::listS, boost::listS, boost::bidirectionalS, DLTVertex> DLTree;

/**************** GRAPH ****************/
struct VertexInfo {
    DLTree desc;
    DLTree anc;
};

struct EdgeInfo {
    int cost;
};

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, VertexInfo, EdgeInfo> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;

/********* DLTree - 2 *********/
// DLTree contains pointers to vertices and edges between those
struct DLTVertex {
    Vertex* v;
};

/* Vertex NULL_VERTEX = boost::graph_traits<Graph>::null_vertex();
Vertex* NULL_VERTEX_PTR = &NULL_VERTEX; */
