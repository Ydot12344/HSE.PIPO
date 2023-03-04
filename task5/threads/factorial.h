#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

namespace NFactorial {

std::string ToString(__int128 num);

__uint128_t Factorial(size_t number, size_t jobs);

} // namespace NFactorial