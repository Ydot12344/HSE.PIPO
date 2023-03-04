#pragma once

#include <atomic>
#include <functional>
#include <vector>
#include <thread>
#include <iostream>

namespace NMergeSort {

struct Segment {
    size_t Start;
    size_t End;
    size_t Cur;
};

template<class TItem>
void Merge(std::vector<TItem>& array, std::vector<Segment>& segments) {
    std::vector<TItem> tmp;
    size_t N = segments.back().End - segments.front().Start;

    for(size_t i = 0; i < N; i++) {
        TItem min;
        size_t minIdx;
        bool isFirst = true;

        for (size_t j = 0; j < segments.size(); j++) {
            if (segments[j].Cur != segments[j].End) {
                if (isFirst || array[segments[j].Cur] < min) {
                    isFirst = false;
                    min = array[segments[j].Cur];
                    minIdx = j;
                }
            }
        }
        segments[minIdx].Cur++;

        tmp.push_back(min);
    }

    for (size_t i = segments.front().Start; i < segments.back().End; i++) {
        array[i] = tmp[i - segments.front().Start];
    }
}

template<class TItem>
void MergeSort(
    std::vector<TItem>& array,
    size_t start,
    size_t end,
    std::function<bool(const TItem&, const TItem&)> comp
) {
    if (end - start == 1) {
        return;
    }

    size_t middle = (end + start) / 2; 

    MergeSort(array, start, middle, comp);
    MergeSort(array, middle, end, comp);

    std::vector<Segment> segments = {
        {start, middle, start},
        {middle, end, middle}
    };

    Merge(array, segments);
}

template<class TItem>
void Sort(
    std::vector<TItem>& array,
    std::function<bool(const TItem&, const TItem&)> comp,
    size_t numThreads
) {
    size_t minPartSize = array.size() / numThreads;
    size_t cntAdditional = array.size() % numThreads;
    size_t currentIt = 0;

    std::vector<Segment> segments;

    std::vector<std::thread> threads;
    threads.reserve(numThreads);

    for (size_t i = 0; i < numThreads; i++) {
        size_t start = currentIt;
        size_t end = currentIt + minPartSize;
        if (cntAdditional > 0) {
            end++;
            cntAdditional--;
        }

        currentIt = end;

        segments.push_back({start, end, start});
        threads.push_back(
            std::thread(
                [&array, start, end, comp](){
                    MergeSort(array, start, end, comp);
                }
            )
        );
    }

    for (size_t i = 0; i < threads.size(); i++) {
        threads[i].join();
    }

    Merge(array, segments);
}


} // namespace NMergeSort