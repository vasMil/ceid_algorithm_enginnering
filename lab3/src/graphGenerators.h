#include "aliases.h"
#include <boost/graph/random.hpp>

std::vector<std::tuple<Vertex, Vertex, int> > createRandomEdges(
    unsigned int num_vertices, 
    unsigned int num_edges) {

    std::vector<std::tuple<Vertex, Vertex, int> > random_edges;
    std::set<std::pair<Vertex, Vertex> > random_vpairs;
    Vertex in, out;

    boost::random::mt19937 rng;
    boost::random::uniform_int_distribution<> random_Vertex(0,num_vertices-1);
    boost::random::uniform_int_distribution<> random_cost(1,MAX_C);

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

    boost::random::mt19937 rng;

    // Get a random edge and create a decrese cost (delta: Δ)
    Edge e;
    for(unsigned int i = 0; i < num_queries; i++) {
        e = boost::random_edge(G, rng);
        boost::random::uniform_int_distribution<> random_cost(0, edgeCost[e] - 1);
        queries.push_back(std::make_tuple(boost::source(e, G), boost::target(e, G), random_cost(rng)));
    }

    return queries;
}

std::vector<std::pair<Vertex, Vertex> > getAllPairQueries(const Graph G) {
    std::vector<std::pair<Vertex, Vertex> > queries;
    VertexIter iit, iend, jit, jend;
    for(boost::tie(iit, iend) = boost::vertices(G); iit != iend; iit++) {
        for(boost::tie(jit, jend) = boost::vertices(G); jit != jend; jit++) {
            if(iit != jit) queries.push_back(std::make_pair(*iit, *jit));
        }
    }
    return queries;
}
