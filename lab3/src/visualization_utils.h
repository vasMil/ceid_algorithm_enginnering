#pragma once
#include "aliases.h"
#include "AIMN91_DataStructure.h"

#if VISUALIZE

    // Opens a pipe and retrieves the output of cmd
    // pipe_exec IS NOT MY CODE - REFERENCE: 
    // https://stackoverflow.com/questions/478898/how-do-i-execute-a-command-and-get-the-output-of-the-command-within-c-using-po
    std::string pipe_exec(const char* cmd) {
        std::array<char, 128> buffer;
        std::string result;
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
        if (!pipe) {
            throw std::runtime_error("popen() failed!");
        }
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }
        return result;
    }
    
    // Produces a png and opens up a view using a python script
    // Restates the terminal as the active window, after viewer launched
    std::string renderGraphFile() {
        int i = 0, j = 0;
        // Get the id of the window - terminal
        auto terminalWindowId = pipe_exec("xdotool getactivewindow");
        std::string focus_terminal_cmd = "xdotool windowactivate " + terminalWindowId;

        // Run the python script and expect it to open a png viewer
        i = std::system("python3 scripts/renderGraphviz.py out/AIMN91_graph.dot");
        if(i != 0) std::cout << "Graph render failed!" << std::endl;

        // Wait for the viewer to become the active window
        auto curr_window_id = pipe_exec("xdotool getactivewindow");
        while(curr_window_id == terminalWindowId) {
            if(i != 0) break;
            usleep(100);
            curr_window_id = pipe_exec("xdotool getactivewindow");
        }

        // Restore the terminal as the active window
        j = system(focus_terminal_cmd.c_str());

        // Inform the user for any unexpected behaviour
        if(j != 0) {
            std::cout << "Terminal focus failed";
            if(i != 0) {
                std::cout << std::endl;
            }
            else {
                std::cout << " - focus remains on the graph!" << std::endl;
            }
        }
        if (curr_window_id != terminalWindowId) {
            return curr_window_id;
        }
        return "";
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