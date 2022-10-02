#pragma once
#include "aliases.h"
#include <boost/graph/random.hpp>
#include <ctime>

std::vector<std::tuple<Vertex, Vertex, int> > createRandomEdges(
    unsigned int num_vertices, 
    unsigned int num_edges) {

    std::vector<std::tuple<Vertex, Vertex, int> > random_edges;
    std::set<std::pair<Vertex, Vertex> > random_vpairs;
    Vertex in, out;

    boost::random::mt19937 rng;
    boost::random::uniform_int_distribution<> random_Vertex(0,num_vertices-1);
    boost::random::uniform_int_distribution<> random_cost(1, MAX_C);

    // Create a set of random pairs of vertices
    while(random_vpairs.size() != num_edges) {
        in = random_Vertex(rng);
        out = random_Vertex(rng);
        while(in == out) out = random_Vertex(rng);
        random_vpairs.insert(std::make_pair(in, out));
    }
    for(auto it = random_vpairs.begin(); it != random_vpairs.end(); ++it)
        random_edges.push_back(std::make_tuple(it->first, it->second, random_cost(rng)));
    return random_edges;
}

std::vector<std::tuple<Vertex, Vertex, int> > createRandomQueries(
    Graph G,
    unsigned int num_queries) {

    int num_vertices = boost::num_vertices(G);
    int num_edges = boost::num_edges(G);

    auto edgeCost = boost::get(&EdgeInfo::cost, G);

    std::vector<std::tuple<Vertex, Vertex, int> > queries;
    std::set<std::pair<Vertex, Vertex> > qs;

    boost::random::mt19937 rng(std::time(0));

    // Get a random edge and create a decrese cost (delta: Δ)
    Edge e;
    while(qs.size() != num_queries) {
        e = boost::random_edge(G, rng);
        boost::random::uniform_int_distribution<> random_cost(0, edgeCost[e] - 1);
        auto delta = random_cost(rng);
        if(delta >= edgeCost[e] || qs.find(std::make_pair(boost::source(e, G), boost::target(e, G))) != qs.end()) {
            continue;
        }
        qs.insert(std::make_pair(boost::source(e, G), boost::target(e, G)));
        queries.push_back(std::make_tuple(boost::source(e, G), boost::target(e, G), random_cost(rng)));
    }

    return queries;
}

std::vector<std::tuple<Vertex, Vertex, int> > createCompleteGraph(const Graph G) {
    std::vector<std::tuple<Vertex, Vertex, int> > queries;
    // Init utils for random cost
    boost::random::mt19937 rng(std::time(0));
    boost::random::uniform_int_distribution<> random_cost(1, MAX_C);
    // Create the tuples
    VertexIter iit, iend, jit, jend;
    for(boost::tie(iit, iend) = boost::vertices(G); iit != iend; iit++) {
        for(boost::tie(jit, jend) = boost::vertices(G); jit != jend; jit++) {
            if(iit != jit) queries.push_back(std::make_tuple(*iit, *jit, random_cost(rng)));
        }
    }
    return queries;
}

std::vector<std::tuple<Vertex, Vertex, int> > getMissingEdgesAsQueries(Graph G, unsigned int num_queries) {
    int cnt = 0;
    int num_vertices = boost::num_vertices(G);
    int num_edges = boost::num_edges(G);
    std::vector<std::tuple<Vertex, Vertex, int> > edges;
    boost::random::mt19937 rng(std::time(0));
    boost::random::uniform_int_distribution<> random_vertex_dist(0, num_vertices-1);
    boost::random::uniform_int_distribution<> random_cost(0, MAX_C);
    Vertex v, u;

    if (num_edges + num_queries > num_vertices*(num_vertices-1)) {
        throw std::invalid_argument("Too many edges for a Graph with no parallel edges");
    }

    while (num_queries != cnt) {
        // Get a random vertex
        v = random_vertex_dist(rng);
        u = random_vertex_dist(rng);
        if(!boost::edge(v,u,G).second) {
            edges.push_back(std::make_tuple(v,u, random_cost(rng)));
            cnt++;
        }
    }

    return edges;
}

// Return a pair of vectors. The first vector will contain the edges that
// set up the graph and bring it to the desired state, so when the edges of the second vector
// are added Ω(n^3) operations are forced to the distance matrix D.
std::pair<std::vector<std::tuple<Vertex, Vertex, int> >, std::vector<std::tuple<Vertex, Vertex, int> > >  
    aimn91_synthetic_graph(unsigned int num_vertices) 
{
    std::vector<std::tuple<Vertex, Vertex, int> > fedges;
    std::vector<std::tuple<Vertex, Vertex, int> > sedges;
    if(num_vertices % 3 != 0) {
        std::cout << "num_vertices must be a multiple of 3" << std::endl;
        return std::make_pair(fedges, sedges);
    }
    if(num_vertices == 0) {
        return std::make_pair(fedges, sedges);
    }

    // Find the limits of each set of vertices (s, x, t) each of size n/3
    int set_size = num_vertices/3;
    Vertex s_first = 0;
    Vertex s_last = s_first + set_size - 1;
    Vertex x_first = s_last + 1;
    Vertex x_last = s_last + set_size;
    Vertex t_first = x_last + 1;
    Vertex t_last = x_last + set_size;

    // Get the first set of vertices
    for (auto i = 0; i < set_size; i++) {
        fedges.push_back(std::make_tuple(s_first + i, x_first, 2));
        fedges.push_back(std::make_tuple(x_last, t_first + i, 2));
        if(i > 0 && i < set_size-1) {
            fedges.push_back(std::make_tuple(x_first+i,x_first+i+1, 2));
        }
    }

    // Get the second set of vertices
    for (auto i = 1; i < set_size-2; i++) {
        sedges.push_back(std::make_tuple(x_first, x_first+i, 2));
    }

    return std::make_pair(fedges, sedges);
}
