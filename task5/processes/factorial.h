#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

namespace NFactorial {

struct TReturnInfo {
    __uint128_t Result;
    bool IsForkedProcess = false;
};

std::string ToString(__int128 num);

TReturnInfo Factorial(size_t number, size_t jobs);

} // namespace NFactorial