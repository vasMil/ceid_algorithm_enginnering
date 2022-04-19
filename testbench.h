#include "utils.h"
#include "my_algorithms.h"

void timeMe(leda::graph& G, int n, void (*graphFunc)(leda::graph&, int)) {
    // Helper nodes, edges, lists etc

    // Create the graph using the input function
    graphFunc(G, n);
    
    // Configure chrono

    // Start timer
    
    // Run my_bipar_checker()

    // Capture time

    // Run LEDAs implementation

    // End timer

    // Return times as a struct (or a pair)
    
}