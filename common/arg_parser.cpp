#include "arg_parser.h"

#include <iostream>
#include <sstream>

namespace {

bool StartsWith(const std::string& str, const std::string& pattern) {
    if (str.size() < pattern.size()) {
        return false;
    }

    return str.find(pattern) == 0;
}

}

namespace NCommon {

TOpt& TOpt::Name(std::string longName, char shortName, std::string descr) {
    LongName_ = std::move(longName);
    ShortName_ = shortName;
    Description_ = std::move(descr);

    return *this;
}

TOpt& TOpt::StoreResult(void* ptr) {
    Storage_ = ptr;
    return *this;
}

TOpt& TOpt::Required() {
    IsRequired_ = true;
    return *this;
}

void TOpt::Fill(std::string val) {
    if (Type_ == EType::INT) {
        *(int*)Storage_ = std::atoi(val.c_str());
    } else {
        *(std::string*)Storage_ = val;
    }

    IsFilled_ = true;
}

bool TOpt::IsOk() const {
    return !IsRequired_ || IsFilled_;
}

TOpt& TOpt::Type(EType type) {
    Type_ = type;

    return *this;
}

TOpt& TArgParser::AddOption() {
    Opts_.push_back({});
    return Opts_.back();
}

std::string TArgParser::Help() const {
    std::stringstream help;

    auto print = [&help](const TOpt& opt) {
        help << "--" << opt.LongName_;
        if (opt.ShortName_ != ' ') {
            help << ", -" << opt.ShortName_;
        }

        help << "\t" << opt.Description_ << std::endl << std::endl;
    };
    
    bool hasRequired = false;
    bool hasOptional = false;

    for (const auto& opt : Opts_) {
        if (opt.IsRequired_) {
            hasRequired = true;
        } else {
            hasOptional = true;
        }
    }

    if (hasRequired) {
        help << std::endl << "Required opts:" << std::endl;
        for (const auto& opt : Opts_) {
            if (opt.IsRequired_) {
                print(opt);
            }
        }
    }

    if (hasOptional) {
        help << std::endl << "Optional opts:" << std::endl;
        for (const auto& opt : Opts_) {
            if (!opt.IsRequired_) {
                print(opt);
            }
        }
    }

    return help.str();
}

void TArgParser::FillOpt(const std::string& name, const std::string& val) {
    for (auto& opt : Opts_) {
        if (name == opt.LongName_ || (name.size() == 1 && name[0] == opt.ShortName_)) {
            if (opt.Type_ == EType::INT) {
                *(int*)opt.Storage_ = std::atoi(val.c_str());
            } else {
                *(std::string*)opt.Storage_ = val;
            }
            opt.IsFilled_ = true;
        }
    }
}

void TArgParser::ParseArgs(int argc, char **argv) {
    for (size_t i = 1; i < argc; i++) {
        auto processArg = [&i, &argv, argc, this](std::string& argName) {
            std::string val;
            if (argName.find("=") == std::string::npos) {
                if (i + 1 == argc) {
                    std::cout << "No value for " << argName << std::endl;
                    std::terminate();
                }

                val = argv[++i];
            } else {
                val = argName.substr(argName.find("=") + 1);
                argName = argName.substr(0, argName.find("="));    
            }
            FillOpt(argName, val);
        };
        
        std::string str = argv[i];
        if (StartsWith(str, "--")) {
            if (str == "--help") {
                std::cout << Help();
                exit(0);
            }

            str = str.substr(2);
            processArg(str);
        } else if (StartsWith(str, "-")) {
            str = str.substr(1);
            processArg(str);
        } else {
            std::cout << "No such parametr: " << str;
            std::cout << Help();
            std::terminate();
        }
    }

    for (const auto& opt : Opts_) {
        if (!opt.IsOk()) {
            std::cout << Help();
            std::cout << "This option is required: " << opt.LongName_ << " option" << std::endl;
            std::terminate();
        }
    }
}

} // namespace NCommon
