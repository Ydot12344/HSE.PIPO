#include "sort.h"

#include <algorithm>
#include <vector>
#include <random>
#include <cassert>

bool Cmp(const int& a, const int& b) {
    return a < b;
}

int main() {

    size_t N = 10000;
    std::vector<int> arr1(N);
    for (size_t i = 0; i < N; i++) {
        arr1[i] = rand() % 100000;
    }

    std::vector<int> arr2 = arr1;

    std::function<bool(const int&, const int&)> cmp = Cmp;
    NMergeSort::Sort(arr1, cmp, 8);
    std::sort(arr2.begin(), arr2.end());

    assert(arr1.size() == arr2.size());
    for (size_t i = 0; i < arr1.size(); i++) {
        assert(arr1[i] == arr2[i]);
    }
    std::cout << "OK" << std::endl;
}