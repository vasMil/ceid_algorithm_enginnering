#pragma once
#include <stdio.h>
#include <string>
#include <iostream>
#include <boost/random/mersenne_twister.hpp>
#include <boost/graph/random.hpp>
#include <boost/tuple/tuple.hpp>

#include <boost/heap/binomial_heap.hpp>

#include <boost/graph/graphviz.hpp>

#include <boost/graph/dijkstra_shortest_paths.hpp>

struct NodeInfo;
struct EdgeInfo;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, NodeInfo, EdgeInfo> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;

typedef boost::graph_traits<Graph>::edge_iterator EdgeIter;
typedef boost::graph_traits<Graph>::vertex_iterator VertexIter;
typedef boost::graph_traits<Graph>::out_edge_iterator OutEdgeIter;

struct NodeInfo {
    Edge pred;
    unsigned int dist;
    unsigned int dist_to_t; // Will be used in A*
};

struct EdgeInfo {
    unsigned int cost;
};

typedef boost::property_map<Graph, unsigned int EdgeInfo::*>::type CostPMap;
typedef boost::property_map<Graph, unsigned int EdgeInfo::*>::type WeightPMap;
typedef boost::property_map<Graph, unsigned int NodeInfo::*>::type DistPMap;
typedef boost::property_map<Graph, Edge NodeInfo::*>::type PredPMap;

struct CompareNodes {
    static DistPMap pmap_dist;
    // const is necessary, read this:
    // https://stackoverflow.com/questions/5973427/error-passing-xxx-as-this-argument-of-xxx-discards-qualifiers
    bool operator() (Vertex const& lhs, Vertex const& rhs) const {
        return pmap_dist[lhs] > pmap_dist[rhs];
    }
};
DistPMap CompareNodes::pmap_dist;

// binomial_heap options: https://www.boost.org/doc/libs/1_55_0/doc/html/boost/heap/binomial_heap.html#:~:text=The%20container%20supports%20the%20following%20options
typedef boost::heap::binomial_heap<Vertex, boost::heap::compare<CompareNodes> > VertexBH;
typedef VertexBH::handle_type VBH_handle_t;