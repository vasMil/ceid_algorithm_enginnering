#pragma once
#include "aliases.h"
#include "AIMN91_DataStructure.h"

#if true
void verifyUsingDijkstra(AIMN91_DataStructure& AIMN91, std::pair<Vertex, Vertex> query) {
    Graph G = *AIMN91.get_graph();
    auto aimn_path = AIMN91.minpath(query.first, query.second);

    // Run the boost::disjkstra_shortest_paths and get the length of each path aswell,
    // since two minpaths may have different nodes in between but the same length
    // hence AIMN91 has choosen a different path than Dijkstra and has not made a mistake
    std::vector<Vertex> predVec(boost::num_vertices(G));
    std::vector<unsigned int> distVec(boost::num_vertices(G));
    boost::dijkstra_shortest_paths(G, query.first, 
        boost::weight_map(boost::get(&EdgeInfo::cost, G)).
        predecessor_map(&predVec[0]).
        distance_map(
            boost::make_iterator_property_map(
            distVec.begin(), get(boost::vertex_index, G)))
    );

    // Using the predVec to create a vector<Vertex> with the same format as the
    // one returned by AIMN91.minpath()
    std::vector<Vertex> dijkstra_path;
    Vertex s = query.second;
    dijkstra_path.push_back(s);
    while(query.first != s) {
        if(predVec[s] == s) {
            // There is no path from query.first to query.target, since s is
            // a source node
            dijkstra_path.clear();
            break;
        }        
        s = predVec[s];
        dijkstra_path.push_back(s);
    }
    std::reverse(dijkstra_path.begin(), dijkstra_path.end());

    // Check if the two solutions are equivelant
    if(dijkstra_path != aimn_path && 
        distVec[query.second] != AIMN91.length(query.first, query.second))
    {
        std::cout << "Dijkstra and AIMN91 disagree on query: " << 
            query.first << " -> " << query.second << std::endl;

        std::cout << "Dijkstra (length = " << distVec[query.second] << ")..." << std::endl;
        auto itt = dijkstra_path.begin();
        auto eitt = dijkstra_path.end();
        for( ; itt != eitt; itt++) {
            std::cout << (*itt) << std::endl;
        }

        std::cout << "AIMN91 (length = " << AIMN91.length(query.first, query.second) << ")..." << std::endl;
        auto it = aimn_path.begin();
        auto eit = aimn_path.end();
        for( ; it != eit; it++) {
            std::cout << (*it) << std::endl;
        }
    }
}
#endif