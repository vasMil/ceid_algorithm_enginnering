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
        
        timeme_decrease(750, 750, 250, "random", csv);
        timeme_decrease(750, 1500, 250, "random", csv);
        timeme_decrease(750, 2250, 250, "random", csv);
        timeme_decrease(750, 3000, 250, "random", csv);
        timeme_decrease(750, 4000, 250, "random", csv);
        timeme_decrease(750, 5000, 250, "random", csv);
        timeme_decrease(750, 6000, 250, "random", csv);
        timeme_decrease(750, 8000, 250, "random", csv);
        timeme_decrease(750, 10000, 250, "random", csv);
        timeme_decrease(750, 12000, 250, "random", csv);
        timeme_decrease(750, 14000, 250, "random", csv);
        // timeme_length(750, , 250, "random", csv);
        // timeme_length(750, , 250, "random", csv);

        csv.close();
    #endif
    #if INTERACTIVE
        cli();
    #endif
    return 0;
}
