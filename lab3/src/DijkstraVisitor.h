#pragma once
#include "aliases.h"
#include <boost/graph/dijkstra_shortest_paths.hpp>

struct DijkstraVisitor : public boost::dijkstra_visitor<> {
    Vertex target;

    DijkstraVisitor(Vertex t) {
        this->target = t;
    }

    void examine_vertex(Vertex u, Graph g) {
        if(u == target) {
            throw(2);
        }
    }
};
