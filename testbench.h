#include "utils.h"
#include "my_algorithms.h"
#include <chrono>
#include <stdexcept>

struct Durations {
    std::chrono::duration<double, std::milli> myImpl, ledaImpl;
    int numOfNodes;

    void createFileIfNotExists(std::string filename) {
        // If file already exists, return

        // Else create and setup the first line as (leda, myImpl) - add a newline at the end
    }

    void save_into_csv(std::string filename) {
        // Open file

        // Write the measurment - add a newline at the end
    }

    void print(std::string typeOfGraph) {
        std::cout << "..." << typeOfGraph << " n = " << numOfNodes << "..." << std::endl;
        std::cout << "LEDA time:\t" << ledaImpl.count() << " ms" << std::endl;
        std::cout << "myImpl time:\t" << myImpl.count() << " ms" << std::endl;
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

    // Return durations as a struct (or a pair)
    Durations d;
    d.myImpl = t1 - t0;
    d.ledaImpl = t3 - t2;
    d.numOfNodes = n;
    return d;
}