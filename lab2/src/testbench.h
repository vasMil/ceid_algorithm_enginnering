#include "utils.h"
#include "boostToLeda.h"


struct Durations {
    std::chrono::duration<double, std::milli> time_Dijkstra_SP, time_A_star;
    std::string typeOfGraph;
    int numOfNodes;
    int numOfEdges;
    int nodesVisited_Dijkstra_SP;
    int nodesVisited_A_star;

    Durations(std::string typeOfGraph) {
        this->typeOfGraph = typeOfGraph;
    }

    void save_into_csv(std::fstream& boostCsv) {
        // Create the output file if it does not exist and format it accordingly
        createFileIfNotExists(boostCsv);
        // Write the measurment - add a newline at the end
        boostCsv << typeOfGraph << "," << numOfNodes << "," << numOfEdges << "," 
        << time_Dijkstra_SP.count() << "," << nodesVisited_Dijkstra_SP 
        << "," << time_A_star.count() << "," << nodesVisited_A_star << std::endl;
    }

    private:
    void createFileIfNotExists(std::fstream& boostCsv) {
        boostCsv.seekg(0);
        // If file is not empty
        if(boostCsv.peek() != EOF) {
            // Get back at the end of the file
            boostCsv.seekg(std::ios::end);
            return;
        }
        // Clear the flags (I only care about eof flag)
        boostCsv.clear();
        // Get at the beggining of the file
        boostCsv.seekg(0);
        // Else create and setup the first line as (type, n, leda, myImpl) - add a newline at the end
        boostCsv << "type_of_graph,number_of_nodes,number_of_edges,time_Dijkstra_SP,nodesVisited_Dijkstra,time_A*,nodesVisited_A*" << std::endl;
    }
};


// typeOfGraph may be "grid" or "random"
// if grid then n_nodes is the number of rows and m_edges the number of columns in that grid
// else if random then it is the number of vertices and edges respectively
// else throw an error
void timeMe(
    std::fstream& boostCsv, std::fstream& ledaCsv, std::string typeOfGraph, 
    std::pair<Vertex, Vertex> (*graphGenerator)(Graph&, int, int, int, int), 
    int n_nodes, int m_edges, int min_cost, int max_cost
    ) {
    // Helper variables
    Graph G;
    Durations d(typeOfGraph);
    CostPMap cost = boost::get(&EdgeInfo::cost, G);
    PredPMap pred = boost::get(&NodeInfo::pred, G);
    DistPMap dist = boost::get(&NodeInfo::dist, G);
    LowerBoundPMap lowerBound = boost::get(&NodeInfo::lowerBound, G);
    int cnt = 0;
    Vertex s, t;
    VertexIter vit, vit_end;
    Vertex boost_cur_pred, cur_pred;

    if(typeOfGraph == "grid") {
        d.numOfNodes = n_nodes*m_edges;
        d.numOfEdges = 2*(2*d.numOfNodes - n_nodes - m_edges);
    }
    else if (typeOfGraph == "random") {
        d.numOfNodes = n_nodes;
        d.numOfEdges = m_edges;
    }
    else {
        throw std::runtime_error("Invalid typeOfGraph");
    }

    // Create the graph using the provided graph generator
    boost::tie(s, t) = graphGenerator(G, n_nodes, m_edges, min_cost, max_cost);

#if DEBUG
    std::vector<Vertex> predVec(d.numOfNodes);
    std::vector<int> distVec(d.numOfNodes);

    // Run BOOSTs Dijkstra
    boost::dijkstra_shortest_paths(G, s, boost::predecessor_map(&predVec[0]).distance_map(
    boost::make_iterator_property_map(
        distVec.begin(), get(boost::vertex_index, G)
        )).weight_map(cost));
    int boost_isPath = true;
    if(predVec[t] == t) {
        boost_isPath = false;
        std::cout << "\tThere is not path from " << s << " to " << t << std::endl;
    }
    // Clear NodeInfo - Redundant since Dijkstra_SP requires no initialization
    clearNodeInfo(G, dist, lowerBound, pred);
#endif

    // Run Dijkstra
    auto t0 = std::chrono::high_resolution_clock::now();
    bool isPath = Dijkstra_SP(G, s, t, cost, pred, dist, cnt, GraphOper::getInstance());
    auto t1 = std::chrono::high_resolution_clock::now();
    d.nodesVisited_Dijkstra_SP = cnt;
    d.time_Dijkstra_SP = t1-t0;

#if DEBUG /* Ensure the returned path is correct for Dijkstra_SP using BOOSTs Dijkstra*/
    if(isPath != boost_isPath) {
        throw std::runtime_error("In Dijkstra_SP debug: One of the algorithms found the SP, while the other failed to");
    }
    if(dist[t] != distVec[t] && boost_isPath) {
        throw std::runtime_error("Dijkstra_SP returned wrong distance!");
    }
#endif


    // Clear NodeInfo and the iteration counter
    clearNodeInfo(G, dist, lowerBound, pred);
    cnt = 0;

    
    // Prepare for A*
    prep_A_star(G, t);

    // Run A*
    auto t2 = std::chrono::high_resolution_clock::now();
    Dijkstra_SP(G, s, t, cost, pred, dist, cnt, GraphOper::getInstance());
    auto t3 = std::chrono::high_resolution_clock::now();
    d.nodesVisited_A_star = cnt;
    d.time_A_star = t3-t2;


#if DEBUG /* Ensure the returned path is correct for A_star using Dijkstra_SP */
    // Postprocess A* - factor out heuristic
    postp_A_star(G, s);

    if(isPath != boost_isPath) {
        throw std::runtime_error("In A* debug: One of the algorithms found SP, while the other failed to");
    }
    if(dist[t] != distVec[t] && boost_isPath) {
        throw std::runtime_error("A* returned wrond distance!");
    }
#endif

    // LEDA
    timeLeda(ledaCsv, G, s, t, cost, typeOfGraph);

    d.save_into_csv(boostCsv);
    return;
}