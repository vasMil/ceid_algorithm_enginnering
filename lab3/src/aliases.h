#pragma once
#include <iostream>
#include <vector>
#include <limits.h>
#include <cstddef>
#include <stdexcept>
#include <queue>
#include <utility>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>

#include "DLTree.h"

#define INF UINT_MAX
#define MAX_C 100 // Max cost of an edge
#define MIN_C 1 // Min cost of an edge


/**************** GRAPH ****************/
struct VertexInfo;
struct EdgeInfo;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, VertexInfo, EdgeInfo> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;

typedef boost::graph_traits<Graph>::vertex_iterator VertexIter;

struct VertexInfo {
    DLTree<Vertex> desc;
    DLTree<Vertex> anc;
};

struct EdgeInfo {
    int cost;
};

typedef boost::property_map<Graph, DLTree<Vertex> VertexInfo::*>::type Vertex_desc_pmap;
typedef boost::property_map<Graph, DLTree<Vertex> VertexInfo::*>::type Vertex_anc_pmap;

Vertex NULL_VERTEX = boost::graph_traits<Graph>::null_vertex();
