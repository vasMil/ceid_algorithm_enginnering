#include "aliases.h"
#include "utils.h"
#include "testbench.h"
#include "timeme.h"

int main() {
    #if TIME
        // Open a stream
        std::fstream csv;
        csv.open(TIMEME_CSV, std::fstream::in | std::fstream::out | std::fstream::app);
        if(!csv.is_open()) {
            std::cout << "Something went wrong with the .csv file!" << std::endl;
            return 1;
        }
        
        // timeme_decrease(900, 0, 250, "aimn91_synth", csv);
        // timeme_length(750, 8000, 250, "random", csv);
        // timeme_add(750, 2000, 250, "random", csv);
        // timeme_minpath(750, 2250, 250, "random", csv);
        timeme_add_length_sequence(750, 750, 250, "random", csv);
        timeme_add_length_sequence(750, 1500, 250, "random", csv);
        timeme_add_length_sequence(750, 2250, 250, "random", csv);
        timeme_add_length_sequence(750, 3000, 250, "random", csv);
        timeme_add_length_sequence(750, 4000, 250, "random", csv);
        timeme_add_length_sequence(750, 5000, 250, "random", csv);
        timeme_add_length_sequence(750, 6000, 250, "random", csv);
        timeme_add_length_sequence(750, 8000, 250, "random", csv);
        timeme_add_length_sequence(750, 10000, 250, "random", csv);
        timeme_add_length_sequence(750, 12000, 250, "random", csv);
        timeme_add_length_sequence(750, 15000, 250, "random", csv);
        timeme_add_length_sequence(750, 20000, 250, "random", csv);
        timeme_add_length_sequence(750, 25000, 250, "random", csv);
        timeme_add_length_sequence(750, 30000, 250, "random", csv);
        timeme_add_length_sequence(750, 40000, 250, "random", csv);
        timeme_add_length_sequence(750, 50000, 250, "random", csv);
        timeme_add_length_sequence(750, 60000, 250, "random", csv);
        timeme_add_length_sequence(750, 70000, 250, "random", csv);
        timeme_add_length_sequence(750, 85000, 250, "random", csv);
        timeme_add_length_sequence(750, 100000, 250, "random", csv);
        timeme_add_length_sequence(750, 120000, 250, "random", csv);
        timeme_add_length_sequence(750, 150000, 250, "random", csv);
        timeme_add_length_sequence(750, 180000, 250, "random", csv);
        timeme_add_length_sequence(750, 200000, 250, "random", csv);
        timeme_add_length_sequence(750, 250000, 250, "random", csv);
        timeme_add_length_sequence(750, 300000, 250, "random", csv);
        timeme_add_length_sequence(750, 350000, 250, "random", csv);
        timeme_add_length_sequence(750, 400000, 250, "random", csv);
        timeme_add_length_sequence(750, 450000, 250, "random", csv);
        timeme_add_length_sequence(750, 550000, 250, "random", csv);
        csv.close();
    #endif
    #if INTERACTIVE
        cli();
    #endif
    return 0;
}
