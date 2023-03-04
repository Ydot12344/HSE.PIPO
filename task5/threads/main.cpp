#include "arg_parser.h"
#include "factorial.h"

#include <unistd.h>

#include <iostream>

using namespace NCommon;
using namespace NFactorial;

int main(int argc, char** argv) {

    TArgParser parser;

    int jobs = sysconf(_SC_NPROCESSORS_ONLN) + 1;

    parser
        .AddOption()
        .Name("num_threads", 'j', "num threads")
        .Type(EType::INT)
        .StoreResult(&jobs);

    parser.ParseArgs(argc, argv);

    if (jobs <= 0) {
        std::cout << "Num jobs must be positive number" << std::endl;
        return 0;
    }

    int num;
    std::cin >> num;
    if (num < 0) {
        std::cout << "Negative numbers not supported" << std::endl;
        return 0;
    }

    std::cout << num << "! = " << ToString(Factorial(num, jobs));
}