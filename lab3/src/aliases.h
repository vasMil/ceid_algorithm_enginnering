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
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

#include "DLTree.h"

#define INF UINT_MAX
#define MAX_C 100 // Max cost of an edge
#define MIN_C 1 // Min cost of an edge

#define INTERACTIVE true
#define VISUALIZE true
#define TIME false
#define DEBUG false

#define GRAPHVIZ_FILE "out/AIMN91_graph.dot"
#define TIMES_FILE "out/times.csv"
#define TIMES_BATCH "out/times_batch.csv"

#if VISUALIZE
    #include <boost/graph/graphviz.hpp>
    #include <cstdlib>
#endif

#if INTERACTIVE
    #include <regex>
#endif



/**************** GRAPH ****************/
struct VertexInfo;
struct EdgeInfo;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, VertexInfo, EdgeInfo> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;

typedef boost::graph_traits<Graph>::vertex_iterator VertexIter;
typedef boost::graph_traits<Graph>::edge_iterator EdgeIter;

struct VertexInfo {
    DLTree<Vertex> desc;
    DLTree<Vertex> anc;
};

struct EdgeInfo {
    unsigned int cost;

    #if VISUALIZE
        std::string color = "black";
    #endif
};

typedef boost::property_map<Graph, DLTree<Vertex> VertexInfo::*>::type Vertex_desc_pmap;
typedef boost::property_map<Graph, DLTree<Vertex> VertexInfo::*>::type Vertex_anc_pmap;

#if VISUALIZE
    typedef boost::property_map<Graph, std::string EdgeInfo::*>::type Edge_color_pmap;
#endif

Vertex NULL_VERTEX = boost::graph_traits<Graph>::null_vertex();
