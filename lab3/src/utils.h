#include "aliases.h"
#include "visualization_utils.h"
#include "AIMN91_DataStructure.h"


#if INTERACTIVE
// Lazy me: Reference to the code below
// https://stackoverflow.com/questions/4654636/how-to-determine-if-a-string-is-a-number-with-c
bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

int cli(AIMN91_DataStructure& AIMN91) {
    #if VISUALIZE
        save_graph_state(AIMN91);
        auto viewer_to_close = renderGraphFile();
        std::cout << "Please do not close the PNG viewer, I will kill the window on exit!" << std::endl;
    #endif
    /* PROMPT USER TO ADD EDGES - REQUEST FOR MINPATH - LENGTH */
    auto const command_regex = std::regex(
        "(add|decrease|minpath|length)\\(([0-9]+),([0-9]+)(?:,([0-9]+))?\\)|exit"
        );
    std::string command;

    std::cout << "+----------------------------------------------------------+" << std::endl;
    std::cout << "| Valid operations:                                        |" << std::endl;
    std::cout << "| add(x,y,w)          | add the edge x->y to your graph    |" << std::endl;
    std::cout << "| decrease(x,y,delta) | decrease the cost of x->y by delta |" << std::endl;
    std::cout << "| minpath(x,y)        | return the minpath from x to y     |" << std::endl;
    std::cout << "| length(x,y)         | return the length of the minpath   |" << std::endl;
    std::cout << "| exit                | to exit the program                |" << std::endl;
    std::cout << "+----------------------------------------------------------+" << std::endl;

    while(true) {
        std::cout << std::endl;

        std::cout << "Enter your command: ";
        std::cin >> command;
        std::smatch regex_match;
        std::regex_match(command, regex_match, command_regex);

        std::string arg[] = {
            regex_match[2],
            regex_match[3],
            regex_match[4]
        };

        if (regex_match[0] == "exit") {
            #if VISUALIZE
                // Should close the viewer window.
                // TODO: Restructure VISUALIZATION proccess so you only have access to kill that window
                std::string close_viewer_cmd = "xdotool windowkill " + viewer_to_close;
                int i = system(close_viewer_cmd.c_str());
                if (i != 0) {
                    std::cout << "Failed to close the viewer - most likely has already been killed" << std::endl;
                }
            #endif
            return 0;
        }

        if(regex_match[1] == "add") {
            for (auto i = 0; i < 3; i++) {
                if(!is_number(arg[i])) {
                    std::cout << "Invalid arguments" << std::endl;
                    goto game_loop;
                }
            }
            AIMN91.safe_add(std::stoi(arg[0]), std::stoi(arg[1]), std::stoi(arg[2]));
            #if VISUALIZE
                resetEdgeColors(AIMN91);
                save_graph_state(AIMN91);
                viewer_to_close = renderGraphFile();
            #endif
        }
        else if (regex_match[1] == "decrease") {
            for (auto i = 0; i < 3; i++) {
                if(!is_number(arg[i])) {
                    std::cout << "Invalid arguments" << std::endl;
                    goto game_loop;
                }
            }
            AIMN91.safe_decrease(std::stoi(regex_match[2]), std::stoi(regex_match[3]), std::stoi(regex_match[4]));
            #if VISUALIZE
                resetEdgeColors(AIMN91);
                save_graph_state(AIMN91);
                viewer_to_close = renderGraphFile();
            #endif
        }
        else if (regex_match[1] == "minpath") {
            for (auto i = 0; i < 2; i++) {
                if(!is_number(arg[i])) {
                    std::cout << "Invalid arguments" << std::endl;
                    goto game_loop;
                }
            }
            auto path = AIMN91.safe_minpath(std::stoi(regex_match[2]), std::stoi(regex_match[3]));
            if (path.size() == 0) {
                std::cout << "There is no path from " << regex_match[2] << " to " << regex_match[3] << "!" << std::endl;
                continue;;
            }
            std::cout << "minpath(" << regex_match[2] << "," << regex_match[3] << ") = ";
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
                viewer_to_close = renderGraphFile();
            #endif
        }
        else if (regex_match[1] == "length") {
            for (auto i = 0; i < 2; i++) {
                if(!is_number(arg[i])) {
                    std::cout << "Invalid arguments" << std::endl;
                    goto game_loop;
                }
            }
            std::cout << "length(" << regex_match[2] << "," << regex_match[3] << ") = " << 
                AIMN91.safe_length(std::stoi(regex_match[2]), std::stoi(regex_match[3])) << std::endl;
        }
        else {
            std::cout << "Invalid operation - please try again!" << std::endl;
        }

        game_loop:;
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
    cli(AIMN91);

    return 0;
}

#endif
