#include "aliases.h"
#include "utils.h"
#include "testbench.h"

int main() {    
    #if TIME
        time_batch_FloydWarshall(800, 1600, "random"); // 50, 100, 200, 300, 400, 500, 600, 800
        time_batch_FloydWarshall(810, 0, "aimn91_synth"); // 30, 60, 90, 120, 150, 210, 300, 390, 510, 630, 810
        time_batch_FloydWarshall(50, 50, "complete");

        time_batch_dijkstra(800, 1600, "random"); // 50, 100, 200, 300, 400, 500, 600, 800
        time_batch_dijkstra(810, 0, "aimn91_synth"); // 30, 60, 90, 120, 150, 210, 300, 390, 510, 630, 810
        time_batch_dijkstra(50, 50, "complete");

        time_batch_AIMN91(800, 1600, "random"); // 50, 100, 200, 300, 400, 500, 600, 800
        time_batch_AIMN91(810, 0, "aimn91_synth"); // 30, 60, 90, 120, 150, 210, 300, 390, 510, 630, 810
        time_batch_AIMN91(50, 50, "complete");

        timeRandom(200, 20); // 50, 100, 200, 300, 400, 500, 600, 800
        timeAIMN91_synthetic_graph(210); // 30, 60, 90, 120, 150, 210, 300, 390, 510, 630, 810
        timeComplete(50, 50); // 50, 100, 200, 300, 400, 500, 600
    #endif
    #if INTERACTIVE
        cli();
    #endif
    return 0;
}
