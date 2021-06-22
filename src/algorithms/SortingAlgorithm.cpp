#include "SortingAlgorithm.hpp"

using namespace std::chrono;

void SortingAlgorithm::doSort(std::vector<unsigned int> &inputVec, std::mutex &dataMutex)
{
    if (isSorting()) { return; }
    startSorting(inputVec.size());
    algorithm(inputVec, dataMutex);
    stopSorting();
}

// Static stuff below
void SortingAlgorithm::waitForNextStep(const std::pair<unsigned int, unsigned int> accessedElements)
{
    ++stats.swaps;
    stats.duration = duration_cast<milliseconds>(high_resolution_clock::now() - clock).count();
    std::this_thread::sleep_for(milliseconds(stepDelay));

    const std::lock_guard<std::mutex> gurad{ lastAccessedMutex };
    lastAccessed.push_back(accessedElements.first);
    lastAccessed.push_back(accessedElements.second);
    while (lastAccessed.size() > maxLastAccessedStored) { lastAccessed.pop_front(); }
}

int SortingAlgorithm::isLastAccessed(unsigned int v)
{
    const std::lock_guard<std::mutex> gurad{ lastAccessedMutex };

    // Get first occurance of the v in the lastAccessed queue starting from the end of the queue
    auto rit = std::find(lastAccessed.rbegin(), lastAccessed.rend(), v);
    if (rit != lastAccessed.rend()) {
        return std::distance(std::begin(lastAccessed), rit.base()) - 1;
    } else {
        return -1;
    }
}


void SortingAlgorithm::startSorting(const size_t elems)
{
    sorting = true;
    stats.swaps = 0;
    stats.elements = elems;
    clock = high_resolution_clock::now();
}

void SortingAlgorithm::stopSorting()
{
    sorting = false;
    stats = SortingStatistics{};
    const std::lock_guard<std::mutex> gurad{ lastAccessedMutex };
    lastAccessed.clear();
}

unsigned int SortingAlgorithm::stepDelay = 1U;
unsigned int SortingAlgorithm::maxLastAccessedStored = 10U;

SortingStatistics SortingAlgorithm::stats{};
time_point<steady_clock> SortingAlgorithm::clock{};
std::deque<unsigned int> SortingAlgorithm::lastAccessed{};
std::mutex SortingAlgorithm::lastAccessedMutex{};
std::atomic_bool SortingAlgorithm::sorting = false;
