#include "arg_parser.h"
#include "backup.h"

using namespace NCommon;
using namespace NBackUp;

int main(int argc, char** argv) {
    TArgParser parser;

    std::string srcPath;
    std::string dstPath;

    parser.AddOption()
        .Name("src", 's', "source directory")
        .StoreResult(&srcPath)
        .Required();
    
    parser.AddOption()
        .Name("dst", 'd', "destination dir")
        .StoreResult(&dstPath)
        .Required();

    parser.ParseArgs(argc, argv);

    BackUp(srcPath, dstPath);
}