#include "aliases.h"
#include "utils.h"
#include "graphGenerators.h"
#include "testbench.h"

int main() {
    unsigned int num_vertices = 130;
    unsigned int num_edges = 2020;
    unsigned int num_queries = 200;

    std::fstream csv;
    csv.open(TIMES_FILE, std::fstream::in | std::fstream::out | std::fstream::app);
    if(!csv.is_open()) {
        std::cout << "Something went wrong with the .csv file!" << std::endl;
        return 1;
    }

    time_me_driver("add", num_queries, num_vertices, num_edges, csv);
    time_me_driver("decrease", num_queries, num_vertices, num_edges, csv);
    time_me_driver("minpath", num_queries, num_vertices, num_edges, csv);
    time_me_driver("length", num_queries, num_vertices, num_edges, csv);
    
    csv.close();
}
