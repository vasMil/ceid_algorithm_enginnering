#include "utils.h"
#include "my_algorithms.h"
#include <chrono>
#include <stdexcept>

struct Durations {
    std::chrono::duration<double, std::milli> myImpl, ledaImpl;
    int numOfNodes;

    void save_into_csv(std::fstream& outfile, std::string typeOfGraph) {
        // Create the output file if it does not exist and format it accordingly
        createFileIfNotExists(outfile);
        // Write the measurment - add a newline at the end
        outfile << typeOfGraph << "," << numOfNodes << "," << ledaImpl.count() << "," << myImpl.count() << std::endl;
    }

    void print(std::string typeOfGraph) {
        std::cout << "..." << typeOfGraph << " n = " << numOfNodes << "..." << std::endl;
        std::cout << "LEDA time:\t" << ledaImpl.count() << " ms" << std::endl;
        std::cout << "myImpl time:\t" << myImpl.count() << " ms" << std::endl;
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
        outfile << "type_of_graph,number_of_nodes,leda_ms,myImpl_ms" << std::endl;
    }
};

Durations timeMe(void (*graphFunc)(leda::graph&, int), leda::graph& G, int n) {
    leda::list<leda::node> V1, V2, myV1, myV2;
    int x, y;

    // Create the graph using the input function
    G.clear();
    graphFunc(G, n);
    
    // Start timer
    auto t0 = std::chrono::high_resolution_clock::now();
    
    // Run my_bipar_checker()
    x = my_bipar_checker(G, myV1, myV2);

    // Capture time
    auto t1 = std::chrono::high_resolution_clock::now();
    
    // Convert graph so I may run leda
    addComplementaryEdges(G);

    auto t2 = std::chrono::high_resolution_clock::now();
    // Run LEDAs implementation
    y = leda::Is_Bipartite(G, V1, V2);

    // End timer
    auto t3 = std::chrono::high_resolution_clock::now();

    if(x != y) throw std::runtime_error("Your implementation is wrong!");

    // Return durations as a struct
    Durations d;
    d.myImpl = t1 - t0;
    d.ledaImpl = t3 - t2;
    d.numOfNodes = n;
    return d;
}