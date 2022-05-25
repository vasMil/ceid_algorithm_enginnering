#pragma once
#include <stdio.h>
#include <string>
#include <vector>
#include <limits>
#include <utility> /* std::pair */
#include <algorithm> /* std::max */
#include <fstream>
#include <chrono>
#include <stdexcept>

/* Used by most header files */
#include <boost/tuple/tuple.hpp>
#include <boost/graph/graph_traits.hpp>

/* Used by graphGenerators.h */
#include <boost/random/mersenne_twister.hpp>
#include <boost/graph/random.hpp>

/* Used in Dijkstra_SP */
#include <boost/heap/binomial_heap.hpp>

/* Used in test.h */
#include <boost/graph/graphviz.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#define DEBUG true
#define POSTPROCESS_EDGE_COSTS false

#define INIT_DIST std::numeric_limits<int>::max()/2
#define INIT_LOWER_BOUND 0

struct NodeInfo;
struct EdgeInfo;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS, NodeInfo, EdgeInfo> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;

typedef boost::graph_traits<Graph>::edge_iterator EdgeIter;
typedef boost::graph_traits<Graph>::vertex_iterator VertexIter;
typedef boost::graph_traits<Graph>::out_edge_iterator OutEdgeIter;
typedef boost::graph_traits<Graph>::in_edge_iterator InEdgeIter;

// In boost there is no null_edge in boost (in contrast to null_vertex(), which is a thing)
// https://stackoverflow.com/questions/69972568/returning-a-null-edge-descriptor-in-an-adjacency-list-c-bgl
// Neither std::optional or boost::optional is an option since they are not supported by the system I am
// compiling on.
const Graph NULL_GRAPH;
const Edge NULL_EDGE = *(boost::edges(NULL_GRAPH).second);

struct NodeInfo {
    Edge pred;
    int dist;
    int lowerBound; // Will be used in A*
    NodeInfo() {
        this->dist = INIT_DIST;
        this->lowerBound = INIT_LOWER_BOUND;
        this->pred = NULL_EDGE;
    }
};

struct EdgeInfo {
    int cost;
};

typedef boost::property_map<Graph, int EdgeInfo::*>::type CostPMap;
typedef boost::property_map<Graph, int EdgeInfo::*>::type WeightPMap;
typedef boost::property_map<Graph, int NodeInfo::*>::type DistPMap;
typedef boost::property_map<Graph, Edge NodeInfo::*>::type PredPMap;
typedef boost::property_map<Graph, int NodeInfo::*>::type LowerBoundPMap;

// This functor converts the max-heap to a min-heap
// More about value_compare:
// https://en.cppreference.com/w/cpp/container/map/value_compare
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

// SINGLETON PATTERN - Functors that will virtually perform edge reversing
struct GraphOper {
    static OutEdgeIter it;
    static GraphOper& getInstance() {
        static GraphOper instance;
        return instance;
    }
    Vertex opposite(Edge& e, const Graph& G) {
        return boost::target(e, G); 
    }
    std::pair<OutEdgeIter, OutEdgeIter> adj_edges(Vertex& v, const Graph& G) const {
        return boost::out_edges(v, G);   
    }
    private:
        GraphOper() {}
        GraphOper(GraphOper const&);
        void operator=(GraphOper const&);
};
struct RevGraphOper {
    static InEdgeIter it;
    static RevGraphOper& getInstance() {
        static RevGraphOper instance;
        return instance;
    }
    Vertex opposite(Edge& e, const Graph& G) {
        return boost::source(e, G); 
    }
    std::pair<InEdgeIter, InEdgeIter> adj_edges(Vertex& v, const Graph& G) const {
        return boost::in_edges(v, G);
    }
    private:
        RevGraphOper() {}
        RevGraphOper(RevGraphOper const&);
        void operator=(RevGraphOper const&);
};

Vertex NULL_VERTEX = boost::graph_traits<Graph>::null_vertex();

template <typename PMap_t, typename Iter_t>
std::vector<int*> extractIntPMap(Graph& G, PMap_t& pmap, Iter_t it, Iter_t it_end) {
    std::vector<int*> extractMap;
    for(; it != it_end; ++it) {
        extractMap.push_back(new int(pmap[*it]));
    }
    return extractMap;
}

void clearNodeInfo(Graph& G, DistPMap& dist, LowerBoundPMap& lowerBound, PredPMap& pred) {
    VertexIter vit, vit_end;
    for(boost::tie(vit, vit_end) = boost::vertices(G); vit != vit_end; ++vit) {
        dist[*vit] = INIT_DIST;
        lowerBound[*vit] = INIT_LOWER_BOUND;
        pred[*vit] = NULL_EDGE;
    }
}