#include "aliases.h"
#include "AIMN91_DataStructure.h"
#include "Durations.h"
#include "debug_utils.h"
#include "graphGenerators.h"
#include "DijkstraVisitor.h"

// Given a number of nodes and a number of starting edges create a Graph with those characteristics
// You should also provide the type of the graph "aimn91_synth" or "random"
// This function will execute num_of_queries additions, divide the total time required by num_of_queries
// to get the average and output the result into fstream csv, using a Durations object.
// If the type of graph is aimn91_synth then num_of_edges and num_of_queries have no effect on the time function
// since they are both determined by the number of vertices.
void timeme_add(int num_of_vertices, int num_of_edges, int num_of_queries, std::string typeOfGraph, std::fstream& csv) {
    std::vector<std::tuple<Vertex, Vertex, int> > initEdges;
    std::vector<std::tuple<Vertex, Vertex, int> > queries;
    
    // Create the Graph for Dijkstra and the AIMN91_DataStructure
    Graph G(num_of_vertices);
    AIMN91_DataStructure AIMN91(num_of_vertices);

    // Property map
    Edge_cost_pmap cost;

    // Initialize the Graph
    if(typeOfGraph == "random") {
        initEdges = createRandomEdges(num_of_vertices, num_of_edges);
        for(int i = 0; i < initEdges.size(); i++) {
            AIMN91.add(std::get<0>(initEdges[i]), std::get<1>(initEdges[i]), std::get<2>(initEdges[i]));
            auto e = boost::add_edge(std::get<0>(initEdges[i]), std::get<1>(initEdges[i]), G);
            cost = boost::get(&EdgeInfo::cost, G);
            cost[e.first] = std::get<2>(initEdges[i]);
        }
        queries = getMissingEdgesAsQueries(*AIMN91.get_graph(), num_of_queries);
    }
    else if (typeOfGraph == "aimn91_synth") {
        boost::tie(initEdges, queries) = aimn91_synthetic_graph(num_of_vertices);
        for(int i = 0; i < initEdges.size(); i++) {
            AIMN91.add(std::get<0>(initEdges[i]), std::get<1>(initEdges[i]), std::get<2>(initEdges[i]));
            auto e = boost::add_edge(std::get<0>(initEdges[i]), std::get<1>(initEdges[i]), G);
            cost = boost::get(&EdgeInfo::cost, G);
            cost[e.first] = std::get<2>(initEdges[i]);
        }
        num_of_edges = initEdges.size();
        num_of_queries = queries.size();
    }

    // Durations object
    Durations<std::nano> dur(typeOfGraph, "add", num_of_vertices, num_of_edges + num_of_queries, "AIMN91");

    // Use the queries vector to perform the additions
    auto t0 = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < queries.size(); i++) {
        AIMN91.add(std::get<0>(queries[i]), std::get<1>(queries[i]), std::get<2>(queries[i]));
    }
    auto t1 = std::chrono::high_resolution_clock::now();
    dur.time = (t1 - t0) / num_of_queries;
    dur.save_into_csv(csv);

    dur.updateInfo(typeOfGraph, "add", num_of_vertices, num_of_edges + num_of_queries, "DIJKSTRA");
    auto t2 = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < queries.size(); i++) {
        auto e = boost::add_edge(std::get<0>(queries[i]), std::get<1>(queries[i]), G);
        cost = boost::get(&EdgeInfo::cost, G);
        cost[e.first] = std::get<2>(queries[i]);
    }
    auto t3 = std::chrono::high_resolution_clock::now();
    dur.time = (t3 - t2) / num_of_queries;
    dur.save_into_csv(csv);

    return;
}

// Given a number of nodes and a number of edges create a Graph with those characteristics
// You should also provide the type of the graph "aimn91_synth" or "random"
// This function will execute num_of_queries random length operations and divide the total time required by num_of_queries
// to get the average. Will output the result into fstream csv, using a Durations object.
void timeme_length(int num_of_vertices, int num_of_edges, int num_of_queries, std::string typeOfGraph, std::fstream& csv) {
    std::vector<std::tuple<Vertex, Vertex, int> > initEdges;
    std::vector<std::tuple<Vertex, Vertex, int> > queries;
    
    // Create the Graph for Dijkstra and the AIMN91_DataStructure
    Graph G(num_of_vertices);
    AIMN91_DataStructure AIMN91(num_of_vertices);

    // Property map
    Edge_cost_pmap cost;

    // Visitor for Dijkstra - throws when destination Vertex is popped from the queue and added to S
    DijkstraVisitor visitor;

    // Initialize the Graph
    if(typeOfGraph == "random") {
        initEdges = createRandomEdges(num_of_vertices, num_of_edges);
        for(int i = 0; i < initEdges.size(); i++) {
            AIMN91.add(std::get<0>(initEdges[i]), std::get<1>(initEdges[i]), std::get<2>(initEdges[i]));
            auto e = boost::add_edge(std::get<0>(initEdges[i]), std::get<1>(initEdges[i]), G);
            cost = boost::get(&EdgeInfo::cost, G);
            cost[e.first] = std::get<2>(initEdges[i]);
        }
    }
    else if (typeOfGraph == "aimn91_synth") {
        boost::tie(initEdges, queries) = aimn91_synthetic_graph(num_of_vertices);
        initEdges.insert(initEdges.end(), queries.begin(), queries.end());
        for(int i = 0; i < initEdges.size(); i++) {
            AIMN91.add(std::get<0>(initEdges[i]), std::get<1>(initEdges[i]), std::get<2>(initEdges[i]));
            auto e = boost::add_edge(std::get<0>(initEdges[i]), std::get<1>(initEdges[i]), G);
            cost = boost::get(&EdgeInfo::cost, G);
            cost[e.first] = std::get<2>(initEdges[i]);
        }
        num_of_edges = initEdges.size();
        num_of_queries = queries.size();
    }
    queries = createRandomQueries(*AIMN91.get_graph(), num_of_queries);

    // Durations object
    Durations<std::nano> dur(typeOfGraph, "length", num_of_vertices, num_of_edges, "AIMN91");

    // Use the queries vector to perform the length operations
    auto t0 = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < queries.size(); i++) {
        AIMN91.length(std::get<0>(queries[i]), std::get<1>(queries[i]));
    }
    auto t1 = std::chrono::high_resolution_clock::now();
    dur.time = (t1 - t0) / num_of_queries;
    dur.save_into_csv(csv);

    dur.updateInfo(typeOfGraph, "length", num_of_vertices, num_of_edges, "DIJKSTRA");
    std::vector<unsigned int> distVec(boost::num_vertices(G));
    auto wmap = boost::get(&EdgeInfo::cost, G);
    auto dmap = boost::make_iterator_property_map(distVec.begin(), get(boost::vertex_index, G));
    auto t2 = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < queries.size(); i++) {
        visitor.target = std::get<1>(queries[i]);
        try {
            boost::dijkstra_shortest_paths(G, std::get<0>(queries[i]), 
                boost::weight_map(wmap).distance_map(dmap).visitor(visitor)
            );
        }
        catch(int exception) { }
    }
    auto t3 = std::chrono::high_resolution_clock::now();
    dur.time = (t3 - t2) / num_of_queries;
    dur.save_into_csv(csv);

    return;
}

// Given a number of nodes and a number of edges create a Graph with those characteristics
// You should also provide the type of the graph "aimn91_synth" or "random"
// This function will execute num_of_queries random minpath operations and divide the total time required by num_of_queries
// to get the average. Will output the result into fstream csv, using a Durations object.
void timeme_minpath(int num_of_vertices, int num_of_edges, int num_of_queries, std::string typeOfGraph, std::fstream& csv) {
    std::vector<std::tuple<Vertex, Vertex, int> > initEdges;
    std::vector<std::tuple<Vertex, Vertex, int> > queries;
    
    // Create the Graph for Dijkstra and the AIMN91_DataStructure
    Graph G(num_of_vertices);
    AIMN91_DataStructure AIMN91(num_of_vertices);

    // Property map
    Edge_cost_pmap cost;

    // Visitor for Dijkstra - throws when destination Vertex is popped from the queue and added to S
    DijkstraVisitor visitor;

    // Initialize the Graph
    if(typeOfGraph == "random") {
        initEdges = createRandomEdges(num_of_vertices, num_of_edges);
        for(int i = 0; i < initEdges.size(); i++) {
            AIMN91.add(std::get<0>(initEdges[i]), std::get<1>(initEdges[i]), std::get<2>(initEdges[i]));
            auto e = boost::add_edge(std::get<0>(initEdges[i]), std::get<1>(initEdges[i]), G);
            cost = boost::get(&EdgeInfo::cost, G);
            cost[e.first] = std::get<2>(initEdges[i]);
        }
    }
    else if (typeOfGraph == "aimn91_synth") {
        boost::tie(initEdges, queries) = aimn91_synthetic_graph(num_of_vertices);
        initEdges.insert(initEdges.end(), queries.begin(), queries.end());
        for(int i = 0; i < initEdges.size(); i++) {
            AIMN91.add(std::get<0>(initEdges[i]), std::get<1>(initEdges[i]), std::get<2>(initEdges[i]));
            auto e = boost::add_edge(std::get<0>(initEdges[i]), std::get<1>(initEdges[i]), G);
            cost = boost::get(&EdgeInfo::cost, G);
            cost[e.first] = std::get<2>(initEdges[i]);
        }
        num_of_edges = initEdges.size();
        num_of_queries = queries.size();
    }
    queries = createRandomQueries(*AIMN91.get_graph(), num_of_queries);

    // Durations object
    Durations<std::nano> dur(typeOfGraph, "minpath", num_of_vertices, num_of_edges, "AIMN91");

    // Use the queries vector to perform the minpath operations
    auto t0 = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < queries.size(); i++) {
        AIMN91.minpath(std::get<0>(queries[i]), std::get<1>(queries[i]));
    }
    auto t1 = std::chrono::high_resolution_clock::now();
    dur.time = (t1 - t0) / num_of_queries;
    dur.save_into_csv(csv);

    dur.updateInfo(typeOfGraph, "minpath", num_of_vertices, num_of_edges, "DIJKSTRA");
    std::vector<Vertex> predVec(boost::num_vertices(G));
    auto wmap = boost::get(&EdgeInfo::cost, G);
    auto t2 = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < queries.size(); i++) {
        visitor.target = std::get<1>(queries[i]);
        try {
            boost::dijkstra_shortest_paths(G, std::get<0>(queries[i]), 
                boost::weight_map(wmap).predecessor_map(&predVec[0]).visitor(visitor)
            );
        }
        catch(int exception) { }
    }
    auto t3 = std::chrono::high_resolution_clock::now();
    dur.time = (t3 - t2) / num_of_queries;
    dur.save_into_csv(csv);

    return;
}

// Given a number of nodes and a number of edges create a Graph with those characteristics
// You should also provide the type of the graph "aimn91_synth" or "random"
// This function will execute num_of_queries decrease operations, divide the total time required by num_of_queries
// to get the average and output the result into fstream csv, using a Durations object.
// If the type of graph is aimn91_synth then num_of_edges and num_of_queries have no effect on the time function
// since they are both determined by the number of vertices.
void timeme_decrease(int num_of_vertices, int num_of_edges, int num_of_queries, std::string typeOfGraph, std::fstream& csv) {
    std::vector<std::tuple<Vertex, Vertex, int> > initEdges;
    std::vector<std::tuple<Vertex, Vertex, int> > queries;
    
    // Create the Graph for Dijkstra and the AIMN91_DataStructure
    Graph G(num_of_vertices);
    AIMN91_DataStructure AIMN91(num_of_vertices);

    // Property map
    Edge_cost_pmap cost;

    // Initialize the Graph
    if(typeOfGraph == "random") {
        initEdges = createRandomEdges(num_of_vertices, num_of_edges);
        for(int i = 0; i < initEdges.size(); i++) {
            AIMN91.add(std::get<0>(initEdges[i]), std::get<1>(initEdges[i]), std::get<2>(initEdges[i]));
            auto e = boost::add_edge(std::get<0>(initEdges[i]), std::get<1>(initEdges[i]), G);
            cost = boost::get(&EdgeInfo::cost, G);
            cost[e.first] = std::get<2>(initEdges[i]);
        }
        queries = createRandomQueries(*AIMN91.get_graph(), num_of_queries);
    }
    else if (typeOfGraph == "aimn91_synth") {
        boost::tie(initEdges, queries) = aimn91_synthetic_graph(num_of_vertices);
        initEdges.insert(initEdges.end(), queries.begin(), queries.end());
        for(int i = 0; i < initEdges.size(); i++) {
            AIMN91.add(std::get<0>(initEdges[i]), std::get<1>(initEdges[i]), std::get<2>(initEdges[i]));
            auto e = boost::add_edge(std::get<0>(initEdges[i]), std::get<1>(initEdges[i]), G);
            cost = boost::get(&EdgeInfo::cost, G);
            cost[e.first] = std::get<2>(initEdges[i]);
        }
        num_of_edges = initEdges.size();
        num_of_queries = queries.size();
        // Decrease the cost of each query edge by one
        for(auto it = queries.begin(); it != queries.end(); it++) {
            --std::get<2>(*it);
        }
    }

    // Durations object
    Durations<std::nano> dur(typeOfGraph, "decrease", num_of_vertices, num_of_edges, "AIMN91");

    // Use the queries vector to perform the decrease operations
    auto t0 = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < queries.size(); i++) {
        AIMN91.decrease(std::get<0>(queries[i]), std::get<1>(queries[i]), std::get<2>(queries[i]));
    }
    auto t1 = std::chrono::high_resolution_clock::now();
    dur.time = (t1 - t0) / num_of_queries;
    dur.save_into_csv(csv);

    dur.updateInfo(typeOfGraph, "decrease", num_of_vertices, num_of_edges, "DIJKSTRA");
    cost = boost::get(&EdgeInfo::cost, G);
    auto t2 = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < queries.size(); i++) {
        auto e = boost::edge(std::get<0>(queries[i]), std::get<1>(queries[i]), G);
        cost[e.first] = std::get<2>(queries[i]);
    }
    auto t3 = std::chrono::high_resolution_clock::now();
    dur.time = (t3 - t2) / num_of_queries;
    dur.save_into_csv(csv);

    return;
}

void timeme_add_length_sequence(int num_of_vertices, int num_of_edges, int num_of_queries, std::string typeOfGraph, std::fstream& csv) {
    std::vector<std::tuple<Vertex, Vertex, int> > initEdges;
    std::vector<std::tuple<Vertex, Vertex, int> > queries_add, queries_length;
    
    // Create the Graph for Dijkstra and the AIMN91_DataStructure
    Graph G(num_of_vertices);
    AIMN91_DataStructure AIMN91(num_of_vertices);

    // Property map
    Edge_cost_pmap cost;

    // Initialize AIMN91 and G
    if(typeOfGraph == "random") {
        initEdges = createRandomEdges(num_of_vertices, num_of_edges);
        for(int i = 0; i < initEdges.size(); i++) {
            AIMN91.add(std::get<0>(initEdges[i]), std::get<1>(initEdges[i]), std::get<2>(initEdges[i]));
            auto e = boost::add_edge(std::get<0>(initEdges[i]), std::get<1>(initEdges[i]), G);
            cost = boost::get(&EdgeInfo::cost, G);
            cost[e.first] = std::get<2>(initEdges[i]);
        }
        queries_add = getMissingEdgesAsQueries(*AIMN91.get_graph(), num_of_queries);
    }
    else if (typeOfGraph == "aimn91_synth") {
        boost::tie(initEdges, queries_add) = aimn91_synthetic_graph(num_of_vertices);
        for(int i = 0; i < initEdges.size(); i++) {
            AIMN91.add(std::get<0>(initEdges[i]), std::get<1>(initEdges[i]), std::get<2>(initEdges[i]));
            auto e = boost::add_edge(std::get<0>(initEdges[i]), std::get<1>(initEdges[i]), G);
            cost = boost::get(&EdgeInfo::cost, G);
            cost[e.first] = std::get<2>(initEdges[i]);
        }
        num_of_edges = initEdges.size();
        num_of_queries = queries_add.size();
    }

    // Durations object
    Durations<std::nano> dur(typeOfGraph, "add-length", num_of_vertices, num_of_edges, "AIMN91");

    // Create two separate vectors, one containing queries for length operations (even if path start -> target does not exist)
    // and one containing new edges (that are not in the Graph yet)
    queries_length = createRandomEdges(num_of_vertices, num_of_queries);

    // Associate an iterator with each of the two vectors
    std::vector<std::tuple<Vertex, Vertex, int> >::iterator lit = queries_length.begin(), ait = queries_add.begin();

    // Create a random boolean generator that will produce either true or false (50% each)
    // when the boolean is true a new edge will be added, else a length query will be executed.
    boost::random::mt19937 rng;
    boost::random::uniform_int_distribution<> random_bool(0,1);
    // First create the random booleans, so both algorithms may be executed upon the same sequence of operations
    std::vector<bool> operSeq(num_of_queries);
    for (int i = 0; i < num_of_queries; i++) {
        operSeq[i] = random_bool(rng);
    }

    // Time - AIMN91
    auto t0 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < num_of_queries; i++) {
        if(operSeq[i]) {
            AIMN91.add(std::get<0>(*ait), std::get<1>(*ait), std::get<2>(*ait));
        }
        else {
            AIMN91.length(std::get<0>(*ait), std::get<1>(*ait));
        }
        ++ait;
    }
    auto t1 = std::chrono::high_resolution_clock::now();
    dur.time = t1 - t0;
    dur.save_into_csv(csv);

    // Time - Dijkstra
    dur.updateInfo(typeOfGraph, "add-length", num_of_vertices, num_of_edges, "DIJKSTRA");
    std::vector<unsigned int> distVec(boost::num_vertices(G));
    auto wmap = boost::get(&EdgeInfo::cost, G);
    auto dmap = boost::make_iterator_property_map(distVec.begin(), get(boost::vertex_index, G));

    // Visitor for Dijkstra - throws when destination Vertex is popped from the queue and added to S
    DijkstraVisitor visitor;

    // Reset the iterators
    lit = queries_length.begin(); ait = queries_add.begin();
    auto t2 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < num_of_queries; i++) {
        if(operSeq[i]) {
            auto e = boost::add_edge(std::get<0>(*ait), std::get<1>(*ait), G);
            cost = boost::get(&EdgeInfo::cost, G);
            cost[e.first] = std::get<2>(*ait);
        }
        else {
            visitor.target = std::get<1>(*lit);
            try {
                boost::dijkstra_shortest_paths(G, std::get<0>(*ait), 
                    boost::weight_map(wmap).distance_map(dmap).visitor(visitor)
                );
            }
            catch(int exception) { }
        }
        ++ait;
    }
    auto t3 = std::chrono::high_resolution_clock::now();
    dur.time = t3 - t2;
    dur.save_into_csv(csv);
}