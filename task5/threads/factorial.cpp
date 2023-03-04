#include "factorial.h"

#include <thread>
#include <vector>

namespace NFactorial {

std::string ToString(__int128 num) {
    std::string str;
    do {
        int digit = num % 10;
        str = std::to_string(digit) + str;
        num = (num - digit) / 10;
    } while (num != 0);
    return str;
}

__uint128_t Factorial(size_t number, size_t jobs) {
    if (number == 0) return 1;
    
    size_t minSizeWork = number / jobs;
    size_t cntAdditional = number % jobs;

    std::vector<long long> results(jobs);
    std::vector<std::thread> threads;
    threads.reserve(jobs);

    size_t currentIt = 1;

    for (size_t i = 0; i < jobs; i++) {
        size_t start = currentIt;
        size_t end = currentIt + minSizeWork;
        if (cntAdditional > 0) {
            end++;
            cntAdditional--;
        }

        currentIt = end;

        threads.push_back(
            std::thread(
                [&results, start, end, i]() {
                    __uint128_t res = 1;
                    for (size_t j = start; j < end; j++) {
                        res *= j;
                    }

                    results[i] = res;
                }
            )
        );
    }

    for (size_t i = 0; i < jobs; i++) {
        threads[i].join();
    }

    __uint128_t result = 1;
    for (size_t i = 0; i < results.size(); i++) {
        result *= results[i];
    }

    return result;
}

} // namespace NFactorial