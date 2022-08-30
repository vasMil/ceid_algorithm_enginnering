#include "aliases.h"

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