#include "aliases.h"
#include "AIMN91_DataStructure.h"
#include <fstream>
#include <chrono>
#include <boost/graph/copy.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/floyd_warshall_shortest.hpp>
#include <boost/graph/exterior_property.hpp>

#if DEBUG
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

// time(ns), typeOfGraph, operation, numberOfNodes, numberOfEdges, algorithm
struct Durations {
    std::chrono::duration<double, std::nano> time;
    std::string typeOfGraph;
    std::string operation;
    int numOfNodes;
    int numOfEdges;
    std::string algorithm;

    Durations(std::string typeOfGraph, std::string operation, int n, int m, std::string algorithm) {
        this->typeOfGraph = typeOfGraph;
        this->operation = operation;
        this->numOfNodes = n;
        this->numOfEdges = m;
        this->algorithm = algorithm;
    }

    void updateInfo(std::string typeOfGraph, std::string operation, int n, int m, std::string algorithm) {
        this->typeOfGraph = typeOfGraph;
        this->operation = operation;
        this->numOfNodes = n;
        this->numOfEdges = m;
        this->algorithm = algorithm;
        this->time = (std::chrono::duration<double, std::nano>)0;
    }

    void save_into_csv(std::fstream& csv) {
        // Create the output file if it does not exist and format it accordingly
        createFileIfNotExists(csv);
        // Write the measurment - add a newline at the end
        csv << time.count() << "," << typeOfGraph << "," << operation << "," << numOfNodes << "," 
            << numOfEdges << "," << algorithm << std::endl;
    }

    private:
    void createFileIfNotExists(std::fstream& csv) {
        csv.seekg(0);
        // If file is not empty
        if(csv.peek() != EOF) {
            // Get back at the end of the file
            csv.seekg(std::ios::end);
            return;
        }
        // Clear the flags (I only care about eof flag)
        csv.clear();
        // Get at the beggining of the file
        csv.seekg(0);
        // Else create and setup the first line - add a newline at the end
        csv << "time,typeOfGraph,operation,numberOfNodes,numberOfEdges,algorithm" << std::endl;
    }
};

int time_batch_FloydWarshall(unsigned int num_vertices, unsigned int num_edges, std::string typeOfGraph) {
    std::cout << "\n\n/******************** BATCH FLOYD_WARSHALL ********************/" << std::endl;

    // Open a stream
    std::fstream csv;
    csv.open(TIMES_BATCH, std::fstream::in | std::fstream::out | std::fstream::app);
    if(!csv.is_open()) {
        std::cout << "Something went wrong with the .csv file!" << std::endl;
        return 1;
    }

    // Prepare Graph G and the queries
    std::cout << "Initializing the Graph for Floyd Warshall algorithm..." << std::endl;
    Graph G;
    for (int i = 0; i < num_vertices; i++) {
        boost::add_vertex(G);
    }

    std::vector<std::tuple<Vertex, Vertex, int> > queries;
    if(typeOfGraph == "random") {
        queries = createRandomEdges(num_vertices, num_edges);
    }
    else if (typeOfGraph == "aimn91_synth") {
        std::vector<std::tuple<Vertex, Vertex, int> > init_edges;
        boost::tie(init_edges, queries) = aimn91_synthetic_graph(num_vertices);
        for(int i = 0; i < init_edges.size(); i++) {
            auto e = boost::add_edge(std::get<0>(init_edges[i]), std::get<1>(init_edges[i]), G).first;
            boost::put(boost::get(&EdgeInfo::cost, G), e, std::get<2>(init_edges[i]));
        }
    }
    else if (typeOfGraph == "complete") {
        auto edges = createCompleteGraph(G);
        auto init_edges = std::vector<std::tuple<Vertex, Vertex, int> >(edges.begin(), edges.end()-num_edges);
        for(int i = 0; i < init_edges.size(); i++) {
            auto e = boost::add_edge(std::get<0>(init_edges[i]), std::get<1>(init_edges[i]), G).first;
            boost::put(boost::get(&EdgeInfo::cost, G), e, std::get<2>(init_edges[i]));
        }
        queries = std::vector<std::tuple<Vertex, Vertex, int> >(edges.end()-num_edges, edges.end());
    }
    std::cout << "Initialization done..." << std::endl;

    std::cout << "Running FLOYD_WARSHALL..." << std::endl;
    boost::exterior_vertex_property<Graph, unsigned int>::matrix_type distMat(num_vertices);
    Durations dur(typeOfGraph, "-", num_vertices, boost::num_edges(G), "FLOYD_WARSHALL");
    for(int i = 0; i < queries.size(); i++) {
        auto e = boost::add_edge(std::get<0>(queries[i]), std::get<1>(queries[i]), G).first;
        boost::put(boost::get(&EdgeInfo::cost, G), e, std::get<2>(queries[i]));
        auto t2 = std::chrono::high_resolution_clock::now();
        boost::floyd_warshall_all_pairs_shortest_paths(G, distMat, boost::weight_map(boost::get(&EdgeInfo::cost, G)));
        auto t3 = std::chrono::high_resolution_clock::now();
        dur.time = t3 - t2;
        dur.save_into_csv(csv);
        dur.updateInfo(typeOfGraph, "-", num_vertices, boost::num_edges(G), "FLOYD_WARSHALL");
    }
    std::cout << "FLOYD_WARSHALL done..." << std::endl;
    
    // Close the output file
    csv.close();
    return 0;
}

int time_batch_dijkstra(int num_vertices, int num_edges, std::string typeOfGraph) {
    std::cout << "\n\n/******************** BATCH DIJKSTRA ********************/" << std::endl;
    std::fstream csv;
    csv.open(TIMES_BATCH, std::fstream::in | std::fstream::out | std::fstream::app);
    if(!csv.is_open()) {
        std::cout << "Something went wrong with the .csv file!" << std::endl;
        return 1;
    }
    // Prepare AIMN91_Datastructure and the queries
    std::cout << "Initializing the Graph for Dijkstra algorithm..." << std::endl;
    Graph G;
    for (int i = 0; i < num_vertices; i++) {
        boost::add_vertex(G);
    }

    std::vector<std::tuple<Vertex, Vertex, int> > queries;
    if(typeOfGraph == "random") {
        queries = createRandomEdges(num_vertices, num_edges);
    }
    else if (typeOfGraph == "aimn91_synth") {
        std::vector<std::tuple<Vertex, Vertex, int> > init_edges;
        boost::tie(init_edges, queries) = aimn91_synthetic_graph(num_vertices);
        for(int i = 0; i < init_edges.size(); i++) {
            auto e = boost::add_edge(std::get<0>(init_edges[i]), std::get<1>(init_edges[i]), G).first;
            boost::put(boost::get(&EdgeInfo::cost, G), e, std::get<2>(init_edges[i]));
        }
    }
    else if (typeOfGraph == "complete") {
        auto edges = createCompleteGraph(G);
        auto init_edges = std::vector<std::tuple<Vertex, Vertex, int> >(edges.begin(), edges.end()-num_edges);
        for(int i = 0; i < init_edges.size(); i++) {
            auto e = boost::add_edge(std::get<0>(init_edges[i]), std::get<1>(init_edges[i]), G).first;
            boost::put(boost::get(&EdgeInfo::cost, G), e, std::get<2>(init_edges[i]));
        }
        queries = std::vector<std::tuple<Vertex, Vertex, int> >(edges.end()-num_edges, edges.end());
    }
    std::cout << "Initialization done..." << std::endl;
    
    Durations dur(typeOfGraph, "-", num_vertices, boost::num_edges(G), "DIJKSTRA");
    std::cout << "Running DIJKSTRA..." << std::endl;
    boost::exterior_vertex_property<Graph, unsigned int>::matrix_type distMat(num_vertices);
    for(int i = 0; i < queries.size(); i++) {
        // Insert the new edge
        auto e = boost::add_edge(std::get<0>(queries[i]), std::get<1>(queries[i]), G).first;
        boost::put(boost::get(&EdgeInfo::cost, G), e, std::get<2>(queries[i]));
        // Extract all the required maps to run Dijkstra, while preserving both the predecessors
        // and the length of each path
        std::vector<Vertex> predVec(boost::num_vertices(G));
        std::vector<unsigned int> distVec(boost::num_vertices(G));
        auto wmap = boost::get(&EdgeInfo::cost, G);
        auto dmap = boost::make_iterator_property_map(distVec.begin(), get(boost::vertex_index, G));
        VertexIter vit, vend;
        auto t2 = std::chrono::high_resolution_clock::now();
        for(boost::tie(vit,vend) = boost::vertices(G); vit != vend; vit++) {
            boost::dijkstra_shortest_paths(G, *vit, 
                boost::weight_map(wmap).predecessor_map(&predVec[0]).distance_map(dmap));
        }
        auto t3 = std::chrono::high_resolution_clock::now();
        dur.time = t3 - t2;
        dur.save_into_csv(csv);
        dur.updateInfo(typeOfGraph, "-", num_vertices, boost::num_edges(G), "DIJKSTRA");
    }
    std::cout << "DIJKSTRA done..." << std::endl;

    csv.close();
    return 0;
}

int time_batch_AIMN91(int num_vertices, int num_edges, std::string typeOfGraph) {
    std::cout << "\n\n/******************** BATCH AIMN91 ********************/" << std::endl;
    std::fstream csv;
    csv.open(TIMES_BATCH, std::fstream::in | std::fstream::out | std::fstream::app);
    if(!csv.is_open()) {
        std::cout << "Something went wrong with the .csv file!" << std::endl;
        return 1;
    }
    // Prepare AIMN91_Datastructure and the queries
    std::cout << "Initializing AIMN91..." << std::endl;
    auto AIMN91 = AIMN91_DataStructure(num_vertices);

    std::vector<std::tuple<Vertex, Vertex, int> > queries;
    if(typeOfGraph == "random") {
        queries = createRandomEdges(num_vertices, num_edges);
    }
    else if (typeOfGraph == "aimn91_synth") {
        std::vector<std::tuple<Vertex, Vertex, int> > init_edges;
        boost::tie(init_edges, queries) = aimn91_synthetic_graph(num_vertices);
        for(int i = 0; i < init_edges.size(); i++) {
            AIMN91.add(std::get<0>(init_edges[i]), std::get<1>(init_edges[i]), std::get<2>(init_edges[i]));
        }
    }
    else if (typeOfGraph == "complete") {
        auto edges = createCompleteGraph(*AIMN91.get_graph());
        auto init_edges = std::vector<std::tuple<Vertex, Vertex, int> >(edges.begin(), edges.end()-num_edges);
        for(int i = 0; i < init_edges.size(); i++) {
            AIMN91.add(std::get<0>(init_edges[i]), std::get<1>(init_edges[i]), std::get<2>(init_edges[i]));
        }
        queries = std::vector<std::tuple<Vertex, Vertex, int> >(edges.end()-num_edges, edges.end());
    }
    std::cout << "Initialization done..." << std::endl;
    
    // Time AIMN91 add for all queries vs running Dijkstra after every edge insertion
    std::cout << "Running AIMN91..." << std::endl;
    Durations dur(typeOfGraph, "-", num_vertices, boost::num_edges(*AIMN91.get_graph()), "AIMN91");
    for(int i = 0; i < queries.size(); i++) {
        auto t0 = std::chrono::high_resolution_clock::now();
        AIMN91.add(std::get<0>(queries[i]), std::get<1>(queries[i]), std::get<2>(queries[i]));
        auto t1 = std::chrono::high_resolution_clock::now();
        dur.time = t1 - t0;
        dur.save_into_csv(csv);
        dur.updateInfo(typeOfGraph, "-", num_vertices, boost::num_edges(*AIMN91.get_graph()), "AIMN91");
    }
    std::cout << "AIMN91 done..." << std::endl;

    csv.close();
    return 0;
}

void time_add(
    AIMN91_DataStructure& AIMN91,
    const std::tuple<Vertex, Vertex, int> edge,
    const std::string typeOfGraph,
    std::fstream& csv)
{
    Graph G;
    boost::copy_graph(*AIMN91.get_graph(), G);
    
    Durations dur(typeOfGraph, "add", boost::num_vertices(G),
        boost::num_edges(G) + 1, "AIMN91");
    auto t0 = std::chrono::high_resolution_clock::now();
    AIMN91.add(std::get<0>(edge), std::get<1>(edge), std::get<2>(edge));
    auto t1 = std::chrono::high_resolution_clock::now();
    dur.time = t1 - t0;
    dur.save_into_csv(csv);

    dur.updateInfo(typeOfGraph, "add", boost::num_vertices(G),
        boost::num_edges(G) + 1, "DIJKSTRA");
    auto t2 = std::chrono::high_resolution_clock::now();
    auto e = boost::add_edge(std::get<0>(edge), std::get<1>(edge), G);
    boost::put(boost::get(&EdgeInfo::cost, G), e.first, std::get<2>(edge));
    auto t3 = std::chrono::high_resolution_clock::now();
    dur.time = t3 - t2;
    dur.save_into_csv(csv);

    dur.updateInfo(typeOfGraph, "add", boost::num_vertices(G),
    boost::num_edges(G) + 1, "DIJKSTRA");
}

void time_minpath(
    AIMN91_DataStructure& AIMN91,
    const std::pair<Vertex, Vertex> query, 
    const std::string typeOfGraph,
    std::fstream& csv)
{
    Graph G = *AIMN91.get_graph();

    // Run AIMN91
    Durations dur(typeOfGraph, "minpath", boost::num_vertices(G),
        boost::num_edges(G), "AIMN91");
    auto t0 = std::chrono::high_resolution_clock::now();
    AIMN91.minpath(query.first, query.second);
    auto t1 = std::chrono::high_resolution_clock::now();
    dur.time = t1 - t0;
    dur.save_into_csv(csv);

    // Run Dijkstra
    dur.updateInfo(typeOfGraph, "minpath", boost::num_vertices(G),
        boost::num_edges(G), "DIJKSTRA");
    // Create a predecessor map
    std::vector<Vertex> predVec(boost::num_vertices(G));
    auto wmap = boost::get(&EdgeInfo::cost, G);
    auto t2 = std::chrono::high_resolution_clock::now();
    boost::dijkstra_shortest_paths(G, query.first, 
        boost::weight_map(wmap).predecessor_map(&predVec[0])
    );
    auto t3 = std::chrono::high_resolution_clock::now();
    dur.time = t3 - t2;
    dur.save_into_csv(csv);
}

void time_length(
    AIMN91_DataStructure& AIMN91,
    const std::pair<Vertex, Vertex> query, 
    std::string typeOfGraph,
    std::fstream& csv)
{
    Graph G = *AIMN91.get_graph();

    // Run AIMN91
    Durations dur(typeOfGraph, "length", boost::num_vertices(G),
        boost::num_edges(G), "AIMN91");
    auto t0 = std::chrono::high_resolution_clock::now();
    AIMN91.length(query.first, query.second);
    auto t1 = std::chrono::high_resolution_clock::now();
    dur.time = t1 - t0;
    dur.save_into_csv(csv);

    // Run Dijkstra
    dur.updateInfo(typeOfGraph, "length", boost::num_vertices(G),
        boost::num_edges(G), "DIJKSTRA");
    // Create a distance map
    std::vector<unsigned int> distVec(boost::num_vertices(G));
    auto wmap = boost::get(&EdgeInfo::cost, G);
    auto dmap = boost::make_iterator_property_map(distVec.begin(), get(boost::vertex_index, G));
    auto t2 = std::chrono::high_resolution_clock::now();
    boost::dijkstra_shortest_paths(G, query.first, 
        boost::weight_map(wmap).distance_map(dmap)
    );
    auto t3 = std::chrono::high_resolution_clock::now();
    dur.time = t3 - t2;
    dur.save_into_csv(csv);
}

void time_decrease(
    AIMN91_DataStructure& AIMN91,
    const std::tuple<Vertex, Vertex, int> query, 
    const std::string typeOfGraph,
    std::fstream& csv)
{
    Graph G;
    boost::copy_graph(*AIMN91.get_graph(), G);
    
    Durations dur(typeOfGraph, "decrease", boost::num_vertices(G),
        boost::num_edges(G), "AIMN91");
    auto t0 = std::chrono::high_resolution_clock::now();
    AIMN91.decrease(std::get<0>(query), std::get<1>(query), std::get<2>(query));
    auto t1 = std::chrono::high_resolution_clock::now();
    dur.time = t1 - t0;
    dur.save_into_csv(csv);

    dur.updateInfo(typeOfGraph, "decrease", boost::num_vertices(G),
        boost::num_edges(G), "DIJKSTRA");
    auto t2 = std::chrono::high_resolution_clock::now();
    auto e = boost::edge(std::get<0>(query), std::get<1>(query), G);
    boost::put(boost::get(&EdgeInfo::cost, G), e.first, std::get<2>(query));
    auto t3 = std::chrono::high_resolution_clock::now();
    dur.time = t3 - t2;
    dur.save_into_csv(csv);
}

// Given the name of the operation and the number of queries
// execute the desired time_* function
// on a graph with num_vertices vertices and num_edges edges.
// Output the results using fstream csv
// It seems to be redundant but I think it keeps the code clean when DEBUG true
void time_me_driver(std::string operation, 
    AIMN91_DataStructure& AIMN91,
    std::vector<std::tuple<Vertex, Vertex, int> > queries,
    std::string typeOfGraph,
    std::fstream& csv) {
    
    std::cout << "\n\n/******** RUNNING TIME ME FUNCTION ********/" << std::endl;

    if(operation == "add") {
        // Add query edges one by one - timing each insertion
        std::cout << "Running time_add()..." << std::endl;
        for(auto it = queries.begin(); it != queries.end(); ++it) {
            time_add(AIMN91, *it, typeOfGraph, csv);
        }
        std::cout << "done..." << std::endl;
    }
    else {
        if(operation == "minpath") {
            std::cout << "Starting minpath queries..." << std::endl;
            for(auto it = queries.begin(); it != queries.end(); ++it) {
                time_minpath(AIMN91, std::make_pair(std::get<0>(*it), std::get<1>(*it)), typeOfGraph, csv);
                #if DEBUG
                    verifyUsingDijkstra(AIMN91, std::make_pair(std::get<0>(*it), std::get<1>(*it)));
                #endif
            }
            std::cout << "done..." << std::endl;
        }
        else if (operation == "length") {
            std::cout << "Starting length queries..." << std::endl;
            for(auto it = queries.begin(); it != queries.end(); ++it) {
                time_length(AIMN91, std::make_pair(std::get<0>(*it), std::get<1>(*it)), typeOfGraph, csv);
                #if DEBUG
                    verifyUsingDijkstra(AIMN91, std::make_pair(std::get<0>(*it), std::get<1>(*it)));
                #endif
            }
            std::cout << "done..." << std::endl;
        }
        else if (operation == "decrease") {
            std::cout << "Starting decrease queries..." << std::endl;
            for(auto it = queries.begin(); it != queries.end(); ++it) {
                time_decrease(AIMN91, *it, typeOfGraph, csv);
                #if DEBUG
                    // std::cout << std::get<0>(*it) << " -> " << std::get<1>(*it) << ", " << std::get<2>(*it) << std::endl;
                    verifyUsingDijkstra(AIMN91, std::make_pair(std::get<0>(*it), std::get<1>(*it)));
                #endif
            }
            std::cout << "done..." << std::endl;
        }
        else {
            std::cout << "Invalid operation..." << std::endl;
        }
    }
}

// 50, 100, 200, 300, 400, 500, 600
int timeRandom(int num_vertices, int num_queries) {
    std::fstream csv;
    csv.open(TIMES_FILE, std::fstream::in | std::fstream::out | std::fstream::app);
    if(!csv.is_open()) {
        std::cout << "Something went wrong with the .csv file!" << std::endl;
        return 1;
    }

    unsigned int num_edges = 2*num_vertices;
    std::string typeOfGraph = "random";

    // Create the AIMN91 Datastructure
    std::cout << "Setting up the AIMN91 data structure (adds edges)..." << std::endl;
    AIMN91_DataStructure AIMN91(num_vertices);

    // Generate the starting edges for the AIMN91 data structure
    std::cout << "Generating random edges..." << std::endl;
    auto fedges = createRandomEdges(num_vertices, num_edges);
    std::cout << "Edges are ready..." << std::endl;

    // Add fedges to the graph
    for(auto it = fedges.begin(); it != fedges.end(); ++it) {
        AIMN91.add(std::get<0>(*it), std::get<1>(*it), std::get<2>(*it));
    }
    std::cout << "done..." << std::endl;

    // Create more random edges to add into the graph as queries
    std::cout << "Generating random queries..." << std::endl;
    auto sedges = createRandomQueries(*AIMN91.get_graph(), num_queries);
    auto missing_edges = getMissingEdgesAsQueries(*AIMN91.get_graph(), num_queries);
    std::cout << "Random queries ready..." << std::endl;

    // Time
    time_me_driver("decrease", AIMN91, sedges, typeOfGraph, csv);
    time_me_driver("minpath", AIMN91, sedges, typeOfGraph, csv);
    time_me_driver("length", AIMN91, sedges, typeOfGraph, csv);
    time_me_driver("add", AIMN91, missing_edges, typeOfGraph, csv);

    csv.close();
    return 0;
}

// 30, 60, 90, 120, 150, 210, 300, 390, 510, 630
int timeAIMN91_synthetic_graph(int num_vertices) {
    std::fstream csv;
    csv.open(TIMES_FILE, std::fstream::in | std::fstream::out | std::fstream::app);
    if(!csv.is_open()) {
        std::cout << "Something went wrong with the .csv file!" << std::endl;
        return 1;
    }

    std::string typeOfGraph = "aimn91_synth";

    // Create the AIMN91 Datastructure
    std::cout << "Setting up the AIMN91 data structure (adds edges)..." << std::endl;
    AIMN91_DataStructure AIMN91(num_vertices);
    
    // Generate the starting edges for the AIMN91 data structure
    std::cout << "Generating AIM91 synthetic graph edges and queries..." << std::endl;
    std::vector<std::tuple<Vertex, Vertex, int> > fedges, sedges;
    boost::tie(fedges, sedges) = aimn91_synthetic_graph(num_vertices);
    std::cout << "Edges and queries are ready..." << std::endl;

    // Add fedges to the graph
    for(auto it = fedges.begin(); it != fedges.end(); ++it) {
        AIMN91.add(std::get<0>(*it), std::get<1>(*it), std::get<2>(*it));
    }
    std::cout << "done..." << std::endl;

    time_me_driver("add", AIMN91, sedges, typeOfGraph, csv);
    for(auto it = sedges.begin(); it != sedges.end(); it++) {
        --std::get<2>(*it);
    }
    time_me_driver("decrease", AIMN91, sedges, typeOfGraph, csv);
    time_me_driver("minpath", AIMN91, sedges, typeOfGraph, csv);
    time_me_driver("length", AIMN91, sedges, typeOfGraph, csv);

    csv.close();
    return 0;
}


// 50, 100, 200, 300, 400, 500, 600
int timeComplete(int num_vertices, int num_queries) {
    std::fstream csv;
    csv.open(TIMES_FILE, std::fstream::in | std::fstream::out | std::fstream::app);
    if(!csv.is_open()) {
        std::cout << "Something went wrong with the .csv file!" << std::endl;
        return 1;
    }

    std::string typeOfGraph = "complete";

    // Create the AIMN91 Datastructure
    std::cout << "Setting up the AIMN91 data structure (adds edges)..." << std::endl;
    AIMN91_DataStructure AIMN91(num_vertices);
    
    // Generate the starting edges for the AIMN91 data structure
    std::cout << "Generating the edges  and queries for a complete graph..." << std::endl;
    auto edges = createCompleteGraph(*AIMN91.get_graph());
    auto init_edges = std::vector<std::tuple<Vertex, Vertex, int> >(edges.begin(), edges.end()-num_queries);
    auto queries = std::vector<std::tuple<Vertex, Vertex, int> >(edges.end()-num_queries, edges.end());
    std::cout << "Edges and queries are ready..." << std::endl;

    // Add init_edges to the graph
    for(auto it = init_edges.begin(); it != init_edges.end(); ++it) {
        AIMN91.add(std::get<0>(*it), std::get<1>(*it), std::get<2>(*it));
    }
    std::cout << "done..." << std::endl;

    time_me_driver("add", AIMN91, queries, typeOfGraph, csv);
    for(auto it = queries.begin(); it != queries.end(); it++) {
        --std::get<2>(*it);
    }
    time_me_driver("decrease", AIMN91, queries, typeOfGraph, csv);
    time_me_driver("minpath", AIMN91, queries, typeOfGraph, csv);
    time_me_driver("length", AIMN91, queries, typeOfGraph, csv);

    csv.close();
    return 0;
}