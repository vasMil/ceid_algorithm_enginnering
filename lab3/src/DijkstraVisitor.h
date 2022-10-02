#pragma once
#include "aliases.h"
#include <boost/graph/dijkstra_shortest_paths.hpp>

struct DijkstraVisitor : public boost::dijkstra_visitor<> {
    Vertex target;

    void examine_vertex(const Vertex &u, const Graph &g) const {
        if(u == target) {
            throw(2);
        }
    }
};