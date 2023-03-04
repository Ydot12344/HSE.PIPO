#include "factorial.h"

#include <exception>
#include <sys/shm.h>
#include <sys/wait.h>
#include<unistd.h>

#include <iostream>
#include <vector>

namespace NFactorial {

void DeleteShm(int fd, void* ptr) {
    shmdt(ptr);
    shmctl(fd, IPC_RMID, nullptr);
}

std::string ToString(__int128 num) {
    std::string str;
    do {
        int digit = num % 10;
        str = std::to_string(digit) + str;
        num = (num - digit) / 10;
    } while (num != 0);
    return str;
}

TReturnInfo Factorial(size_t number, size_t jobs) {
    if (number == 0) return {1, false};
    
    size_t minSizeWork = number / jobs;
    size_t cntAdditional = number % jobs;

    int fd = shmget(IPC_PRIVATE, sizeof(__uint128_t)*jobs, IPC_CREAT | 0666);

    if (fd < 0) {
        std::cout << "Error while createing shared memory" << std::endl;
        exit(0);
    }

    __uint128_t* results = (__uint128_t*)shmat(fd, nullptr, 0);
    size_t currentIt = 1;

    std::vector<pid_t> pids;

    for (size_t i = 0; i < jobs; i++) {
        size_t start = currentIt;
        size_t end = currentIt + minSizeWork;
        if (cntAdditional > 0) {
            end++;
            cntAdditional--;
        }

        currentIt = end;

        pids.push_back(fork());

        if (pids.back() < 0) {
            std::cout << "Error while fork" << std::endl;
            DeleteShm(fd, results);
            exit(0);
        } else if (pids.back() == 0) {
            __uint128_t res = 1;
            for (size_t j = start; j < end; j++) {
                res *= j;
            }

            results[i] = res;

            return {0, true};
        }
    }

    for(size_t i = 0; i < pids.size(); i++) {
        while (waitpid(pids[i],nullptr, WNOHANG) != pids[i]) {}
    }

    __uint128_t result = 1;
    for (size_t i = 0; i < jobs; i++) {
        result *= results[i];
    }

    DeleteShm(fd, results);

    return {result, false};
}

} // namespace NFactorial