#include <arg_parser.h>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <thread>
#include <chrono>

int main (int argc, char** argv) {
    std::string path_to_file;

    NCommon::TArgParser parser;

    parser.AddOption()
        .Name("file", 'f', "path to file with comands")
        .StoreResult(&path_to_file)
        .Type(NCommon::EType::STRING)
        .Required();

    parser.ParseArgs(argc, argv);

    std::ifstream input(path_to_file);
    std::map<int, std::vector<std::string>> dellayToProc;
    int del;
    std::string tmp;
    while(input) {
        input >> del;
        input >> tmp;
        dellayToProc[del].push_back(std::move(tmp));
    }

    size_t time = 0;
    for (const auto& [dellay, proceses] : dellayToProc) {
        std::this_thread::sleep_for(std::chrono::seconds{dellay - time});
        time = dellay;
        for (const auto& proc : proceses) {
            std::thread([proc](){
                system(proc.c_str());
            }).detach();
        }   
    }

    std::cout << "End of useless" << std::endl;

    return 0;
}