#include "aliases.h"
#include "AIMN91_DataStructure.h"
#include "tests.h"
#include <regex>

int main() {
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

    /* SETUP AIMN91_DataStructure */
    AIMN91_DataStructure AIMN91(num_v, max_cost);

    /* PROMPT USER TO ADD EDGES - REQUEST FOR MINIMAL PATH - LENGTH */
    auto const command_regex = std::regex("(add|minimal_path|length)\\(([0-9]+),([0-9]+)(?:,([0-9]+))?\\)|exit");
    std::string command;

    std::cout << "+-----------------------------------------------------------+" << std::endl;
    std::cout << "| Valid operations:                                         |" << std::endl;
    std::cout << "| add(x,y,w)        | add the edge x->y to your graph       |" << std::endl;
    std::cout << "| minimal_path(x,y) | return the minimal path from x to y   |" << std::endl;
    std::cout << "| length(x,y)       | return the length of the minimal_path |" << std::endl;
    std::cout << "| exit              | to exit the program                   |" << std::endl;
    std::cout << "+-----------------------------------------------------------+" << std::endl;

    while(true) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << std::endl;

        std::cout << "Enter your command: ";
        std::cin >> command;
        std::smatch regex_match;
        std::regex_match(command, regex_match, command_regex);

        if (regex_match[0] == "exit") {
            return 0;
        }

        if(regex_match[1] == "add") {
            AIMN91.add(std::stoi(regex_match[2]), std::stoi(regex_match[3]), std::stoi(regex_match[4]));
            std::cout << std::endl;
        }
        else if (regex_match[1] == "minimal_path") {
            AIMN91.minimal_path(std::stoi(regex_match[2]), std::stoi(regex_match[3]));
        }
        else if (regex_match[1] == "length") {
            std::cout << "length(" << regex_match[2] << "," << regex_match[3] << ") = " << 
                AIMN91.length(std::stoi(regex_match[2]), std::stoi(regex_match[3])) << std::endl;
        }
        else {
            std::cout << "Invalid operation - please try again!" << std::endl;
        }
    }


    /* Testing */
    // test_directed_graph_no_weight();
}