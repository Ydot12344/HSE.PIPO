#pragma once

#include <string>
#include <vector>
#include <unordered_map>

namespace NCommon {

enum class EType{
    STRING = 0,
    INT = 1,
};

class TOpt {
friend class TArgParser;
public:
    TOpt& Name(std::string longName, char shortName = ' ', std::string descr = "");

    TOpt& StoreResult(void* ptr);

    TOpt& Type(EType type);

    TOpt& Required();

    void Fill(std::string);

    bool IsOk() const;

private:
    std::string Description_;
    std::string LongName_;
    char ShortName_;
    EType Type_ = EType::STRING;
    void* Storage_ = nullptr;
    bool IsRequired_ = false;
    bool IsFilled_ = false;
};

class TArgParser {

public:
    TOpt& AddOption();

    void ParseArgs(int argc, char** argv);

private:
    std::string Help() const;
    void FillOpt(const std::string& name, const std::string& val);

private:
    std::vector<TOpt> Opts_;
};

} // namespace NCommon