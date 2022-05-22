#include "utils.h"

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

    void save_into_csv(std::fstream& outfile) {
        // Create the output file if it does not exist and format it accordingly
        createFileIfNotExists(outfile);
        // Write the measurment - add a newline at the end
        outfile << typeOfGraph << "," << numOfNodes << "," << numOfEdges << "," 
        << time_Dijkstra_SP.count() << "," << nodesVisited_Dijkstra_SP 
        << "," << time_A_star.count() << "," << nodesVisited_A_star << std::endl;
    }

    private:
    void createFileIfNotExists(std::fstream& outfile) {
        outfile.seekg(0);
        // If file is not empty
        if(outfile.peek() != EOF) {
            // Get back at the end of the file
            outfile.seekg(std::ios::end);
            return;
        }
        // Clear the flags (I only care about eof flag)
        outfile.clear();
        // Get at the beggining of the file
        outfile.seekg(0);
        // Else create and setup the first line as (type, n, leda, myImpl) - add a newline at the end
        outfile << "type_of_graph,number_of_nodes,number_of_edges,time_Dijkstra_SP,nodesVisited_Dijkstra,time_A*,nodesVisited_A*" << std::endl;
    }
};



// typeOfGraph may be "grid" or "random"
void timeMe(
    std::fstream& outfile, std::string typeOfGraph, 
    std::pair<Vertex, Vertex> (*graphGenerator)(Graph&, int, int, int, int), 
    int n_nodes, int m_edges, int min_cost, int max_cost
    ) {
    // Helper variables
    Graph G;
    Durations d(typeOfGraph);
    d.numOfNodes = n_nodes;
    d.numOfEdges = m_edges;
    CostPMap cost = boost::get(&EdgeInfo::cost, G);
    PredPMap pred = boost::get(&NodeInfo::pred, G);
    DistPMap dist = boost::get(&NodeInfo::dist, G);
    int cnt = 0;
    Vertex s, t;
    
    // Create the graph using the provided graph generator
    boost::tie(s, t) = graphGenerator(G, n_nodes, m_edges, min_cost, max_cost);

    // Run Dijkstra
    auto t0 = std::chrono::high_resolution_clock::now();
    Dijkstra_SP(G, s, t, cost, pred, dist, cnt, GraphOper::getInstance());
    auto t1 = std::chrono::high_resolution_clock::now();
    d.nodesVisited_Dijkstra_SP = cnt;
    d.time_Dijkstra_SP = t1-t0;

    // Prepare for A*
    cnt = 0; // Clear counter
    prep_A_star(G, t);

    // Run A*
    auto t2 = std::chrono::high_resolution_clock::now();
    Dijkstra_SP(G, s, t, cost, pred, dist, cnt, GraphOper::getInstance());
    auto t3 = std::chrono::high_resolution_clock::now();
    d.nodesVisited_A_star = cnt;
    d.time_A_star = t3-t2;

    d.save_into_csv(outfile);
    return;
}