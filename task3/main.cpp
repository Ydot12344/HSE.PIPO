#include "arg_parser.h"

#include <atomic>
#include <thread>
#include <iostream>

using namespace NCommon;

int main (int argc, char** argv) {
    int maxCommands;

    TArgParser parser;
    parser
        .AddOption()
        .Name("max-commands", 'm', "Max number of running commands")
        .StoreResult(&maxCommands)
        .Required()
        .Type(EType::INT);

    parser.ParseArgs(argc, argv);


    std::atomic<int> cntRunningProcess = 0;
    std::vector<std::thread> threads;

    while(std::cin) {
        std::string cmd;
        if (!std::getline(std::cin, cmd)) {
            std::cout << "End of input" << std::endl;
            exit(0);
        }
        
        if (cntRunningProcess == maxCommands) {
            std::cout << "More than " << maxCommands << " started" << std::endl;
            std::cout << "End of runsim" << std::endl;
            exit(0);
        }

        threads.push_back(
            std::thread([&cntRunningProcess, cmd]() {
                cntRunningProcess++;
                system(cmd.c_str());
                std::cout << ")";
                cntRunningProcess--;
            })
        );
    }

    return 0;
}