#include "aliases.h"
#include "utils.h"
#include "graphGenerators.h"
#include "testbench.h"

#define TIME true
#define TIME_RANDOM false
#define TIME_AIMN91_SYNTH true
#define TIME_FLOYD_WARSHAL false

int main() {
#if TIME
    std::fstream csv;
    csv.open(TIMES_FILE, std::fstream::in | std::fstream::out | std::fstream::app);
    if(!csv.is_open()) {
        std::cout << "Something went wrong with the .csv file!" << std::endl;
        return 1;
    }

    #if TIME_RANDOM
        unsigned int num_vertices = 400;
        unsigned int num_edges = 800;
        unsigned int num_queries = 200;
        std::string typeOfGraph = "random";
    #endif
    #if TIME_AIMN91_SYNTH
        unsigned int num_vertices = 798;
        std::string typeOfGraph = "aimn91_synth";
    #endif

    // Create the AIMN91 Datastructure
    std::cout << "Setting up the AIMN91 data structure (adds edges)..." << std::endl;
    AIMN91_DataStructure AIMN91(num_vertices);

    #if TIME_RANDOM
        // Generate the starting edges for the AIMN91 data structure
        std::cout << "Generating random edges..." << std::endl;
        auto fedges = createRandomEdges(num_vertices, num_edges);
        std::cout << "Edges are ready..." << std::endl;
    #endif
    #if TIME_AIMN91_SYNTH
        // Generate the starting edges for the AIMN91 data structure
        std::cout << "Generating AIM91 synthetic graph edges and queries..." << std::endl;
        std::vector<std::tuple<Vertex, Vertex, int> > fedges, sedges;
        boost::tie(fedges, sedges) = aimn91_synthetic_graph(num_vertices);
        std::cout << "Edges and queries are ready..." << std::endl;
    #endif

    // Add fedges to the graph
    for(auto it = fedges.begin(); it != fedges.end(); ++it) {
        AIMN91.add(std::get<0>(*it), std::get<1>(*it), std::get<2>(*it));
    }
    std::cout << "done..." << std::endl;
    #if TIME_RANDOM
        // Create more random edges to add into the graph as queries
        std::cout << "Generating random queries..." << std::endl;
        auto sedges = createRandomQueries(*AIMN91.get_graph(), num_queries);
        std::cout << "Random queries ready..." << std::endl;

        // Time
        time_me_driver("decrease", AIMN91, sedges, typeOfGraph, csv);
        time_me_driver("minpath", AIMN91, sedges, typeOfGraph, csv);
        time_me_driver("length", AIMN91, sedges, typeOfGraph, csv);
        time_me_driver("add", AIMN91, sedges, typeOfGraph, csv);
        #if TIME_FLOYD_WARSHAL
            std::cout << "Generating random edges..." << std::endl;
            auto edges = createRandomEdges(num_vertices, num_edges);
            std::cout << "Edges are ready..." << std::endl;
            time_with_floyd_warshall_sp(num_vertices, fedges, typeOfGraph);
        #endif
    #endif
    #if TIME_AIMN91_SYNTH
        // Time
        time_me_driver("add", AIMN91, sedges, typeOfGraph, csv);
        for(auto it = sedges.begin(); it != sedges.end(); it++) {
            --std::get<2>(*it);
        }
        time_me_driver("decrease", AIMN91, sedges, typeOfGraph, csv);
        time_me_driver("minpath", AIMN91, sedges, typeOfGraph, csv);
        time_me_driver("length", AIMN91, sedges, typeOfGraph, csv);
    #endif


    csv.close();
#endif

}
