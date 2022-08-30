#include "aliases.h"
#include "visualization_utils.h"
#include "AIMN91_DataStructure.h"

int cli(AIMN91_DataStructure& AIMN91) {
    #if VISUALIZE
        // save_graph_state(AIMN91);
        renderGraphFile();
    #endif
    /* PROMPT USER TO ADD EDGES - REQUEST FOR MINIMAL PATH - LENGTH */
    auto const command_regex = std::regex(
        "(add|decrease|minimal_path|length)\\(([0-9]+),([0-9]+)(?:,([0-9]+))?\\)|exit"
        );
    std::string command;

    std::cout << "+-------------------------------------------------------------+" << std::endl;
    std::cout << "| Valid operations:                                           |" << std::endl;
    std::cout << "| add(x,y,w)          | add the edge x->y to your graph       |" << std::endl;
    std::cout << "| decrease(x,y,delta) | decrease the cost of x->y by delta    |" << std::endl;
    std::cout << "| minimal_path(x,y)   | return the minimal path from x to y   |" << std::endl;
    std::cout << "| length(x,y)         | return the length of the minimal_path |" << std::endl;
    std::cout << "| exit                | to exit the program                   |" << std::endl;
    std::cout << "+-------------------------------------------------------------+" << std::endl;

    while(true) {
        std::cout << std::endl;

        std::cout << "Enter your command: ";
        std::cin >> command;
        std::smatch regex_match;
        std::regex_match(command, regex_match, command_regex);

        if (regex_match[0] == "exit") {
            return 0;
        }

        // TODO: Add checks for the arguments
        if(regex_match[1] == "add") {
            AIMN91.add(std::stoi(regex_match[2]), std::stoi(regex_match[3]), std::stoi(regex_match[4]));
            #if VISUALIZE
                resetEdgeColors(AIMN91);
                save_graph_state(AIMN91);
                renderGraphFile();
            #endif
        }
        else if (regex_match[1] == "decrease") {
            AIMN91.decrease(std::stoi(regex_match[2]), std::stoi(regex_match[3]), std::stoi(regex_match[4]));
            #if VISUALIZE
                resetEdgeColors(AIMN91);
                save_graph_state(AIMN91);
                renderGraphFile();
            #endif
        }
        else if (regex_match[1] == "minimal_path") {
            auto path = AIMN91.minimal_path(std::stoi(regex_match[2]), std::stoi(regex_match[3]));
            if (path.size() == 0) {
                std::cout << "There is no path from " << regex_match[2] << " to " << regex_match[3] << "!" << std::endl;
                continue;
            }
            std::cout << "minimal_path(" << regex_match[2] << "," << regex_match[3] << ") = ";
            for (auto it = path.begin(); it != std::prev(path.end()); it++) {
                std::cout << *it << ", ";
            }
            std::cout << *std::prev(path.end()) << std::endl;

            #if VISUALIZE
                auto G = AIMN91.get_graph();
                Edge_color_pmap color = boost::get(&EdgeInfo::color, *G);
                resetEdgeColors(AIMN91);
                // Find the edge in the G list and set its color to red
                Edge e;
                auto vit = path.begin(); auto vend = std::prev(path.end());
                for ( ; vit != vend; vit++) {
                    e = boost::edge(*vit, *std::next(vit), *G).first;
                    color[e] = "red";
                }
                save_graph_state(AIMN91);
                renderGraphFile();
            #endif
        }
        else if (regex_match[1] == "length") {
            std::cout << "length(" << regex_match[2] << "," << regex_match[3] << ") = " << 
                AIMN91.length(std::stoi(regex_match[2]), std::stoi(regex_match[3])) << std::endl;
        }
        else {
            std::cout << "Invalid operation - please try again!" << std::endl;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

int cli() {
    unsigned int num_v;
    unsigned int max_cost;

    /* HANDLE USER INPUT */
    // https://stackoverflow.com/questions/19696442/how-to-catch-invalid-input-in-c
    std::cout << "Specify |V|: ";
    std::cin >> num_v;
    while (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Bad entry! Specify |V|: ";
        std::cin >> num_v;
    }

    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    std::cout << "Specify the maximum cost an edge may have: ";
    std::cin >> max_cost;
    while (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Bad entry! Specify the maximum cost an edge may have: ";
        std::cin >> max_cost;
    }
    std::cout << std::endl;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');


    /* SETUP AIMN91_DataStructure */
    AIMN91_DataStructure AIMN91(num_v, max_cost);

    return cli(AIMN91);
}

void test_directed_graph_no_weight() {
    AIMN91_DataStructure AIMN91(7);

    Vertex v0 = AIMN91.get_vertex(0);
    Vertex v1 = AIMN91.get_vertex(1);
    Vertex v2 = AIMN91.get_vertex(2);
    Vertex v3 = AIMN91.get_vertex(3);
    Vertex v4 = AIMN91.get_vertex(4);
    Vertex v5 = AIMN91.get_vertex(5);
    Vertex v6 = AIMN91.get_vertex(6);
    
    AIMN91.add(v0, v1, 1);
    AIMN91.add(v0, v2, 1);
    AIMN91.add(v0, v3, 1);

    AIMN91.add(v1, v4, 1);

    AIMN91.add(v2, v4, 1);

    AIMN91.add(v3, v5, 1);

    AIMN91.add(v4, v1, 1);
    AIMN91.add(v4, v2, 1);
    AIMN91.add(v4, v5, 1);

    AIMN91.add(v5, v6, 1);

    AIMN91.add(v6, v4, 1);
    AIMN91.add(v6, v0, 1);

    for(int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            std::cout << "d[" << i << "," << j << "] = " << AIMN91.length(i, j) << std::endl;
        }
    }

    AIMN91.minimal_path(0,6);
}
