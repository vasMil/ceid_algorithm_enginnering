#include "aliases.h"
#include "AIMN91_DataStructure.h"

#if VISUALIZE
    void renderGraphFile() {
        int i = std::system("python3 scripts/renderGraphviz.py out/AIMN91_graph.dot");
        if(i != 0) std::cout << "Graph render failed!" << std::endl;
    }

    // Clean color map of edges from previous minimal path request
    void resetEdgeColors(AIMN91_DataStructure& AIMN91) {
        auto G = AIMN91.get_graph();
        Edge_color_pmap color = boost::get(&EdgeInfo::color, *G);
        EdgeIter eit, eend;
        for (boost::tie(eit, eend) = boost::edges(*G); eit != eend; eit++) {
            color[*eit] = "black";
        }
    }
    
    // Save current state of the Graph in a file (using graphviz)
    void save_graph_state(AIMN91_DataStructure& AIMN91) {
        auto G = AIMN91.get_graph();
        // Open an output stream
        std::ofstream dotFile;
        // It will rewrite the current file if it exists
        dotFile.open(GRAPHVIZ_FILE, std::ofstream::out);
        if(!dotFile.is_open()) {
            std::cout << "Could not access " << GRAPHVIZ_FILE << std::endl;
        }

        boost::dynamic_properties dp;
        dp.property("node_id", boost::get(boost::vertex_index, *G));
        dp.property("color", boost::get(&EdgeInfo::color, *G));
        dp.property("fontcolor", boost::get(&EdgeInfo::color, *G));
        dp.property("label", boost::get(&EdgeInfo::cost, *G));
        boost::write_graphviz_dp(dotFile, *G, dp);

        // Close the output stream
        dotFile.close();
    }
#endif