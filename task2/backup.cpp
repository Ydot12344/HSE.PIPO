#include "backup.h"

#include <filesystem>
#include <functional>
#include <cassert>
#include <iostream>

namespace fs = std::filesystem;

namespace NBackUp {

std::string GetZipPath(
    const std::string& path,
    const std::string& src,
    std::string dst) 
{
    assert(path.size() > src.size());

    dst += path.substr(src.size()) + ".gz";
    return dst;
}

void Zip(const std::string& path, const std::string& zipPath) {
    std::string cmd = "tar -P -cvf " +  zipPath + " " + path;
    system(cmd.c_str());
}

bool CheckLastWriteTime(const std::string& path, const std::string& zipPath) {
    return fs::last_write_time(path) < fs::last_write_time(zipPath);
}

void Travers(const std::string& src, std::function<void(const std::string& path)> packFunc) {
    for (const auto& item : fs::directory_iterator(src)) {
        if (item.is_directory()) {
            Travers(item.path().string(), packFunc);
        } else {
            packFunc(item.path().string());
        }
    }
}

void BackUp(const std::string &src, const std::string &dst) {
    if (!fs::exists(src)) {
        std::cout << "No such directory: " << src << std::endl;
        return;
    }

    if (!fs::exists(dst)) {
        std::cout << "No such directory: " << src << std::endl;
        return;
    }

    auto packFunc = [src, dst] (const std::string& path) {
        std::string zipPath = GetZipPath(path, src, dst);
        if (!fs::exists(zipPath) || !CheckLastWriteTime(path, zipPath)) {
            Zip(path, zipPath);
        } else {
            std::cout << path << " skipped by last modification time" << std::endl;
        }
    };
    Travers(src, packFunc);
}

} // namespace NBackUp