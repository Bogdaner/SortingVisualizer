#ifndef SORTING_ALGORITHM_HPP
#define SORTING_ALGORITHM_HPP

#include <atomic>
#include <chrono>
#include <deque>
#include <mutex>
#include <string>
#include <thread>
#include <utility>
#include <vector>

struct SortingStatistics
{
    unsigned int swaps = 0U;
    unsigned int elements = 0U;
    long long duration = 0U;
};

class SortingAlgorithm
{
  public:
    void doSort(std::vector<unsigned int> &inputVec, std::mutex &dataMutex);
    virtual std::string getAlgortihmName() = 0;

    static bool isSorting() noexcept { return sorting; }
    static unsigned int getCurrLastAcccessedSize() { return lastAccessed.size(); }
    static int isLastAccessed(unsigned int v);
    static SortingStatistics getCurrentStatistics() { return stats; }

    static unsigned int maxLastAccessedStored;
    static unsigned int stepDelay;

  private:
    virtual void algorithm(std::vector<unsigned int> &inputVec, std::mutex &dataMutex) = 0;
    static void startSorting(const size_t elems);
    static void stopSorting();

    static std::atomic_bool sorting;
    static std::deque<unsigned int> lastAccessed;
    static std::mutex lastAccessedMutex;
    static SortingStatistics stats;
    static std::chrono::time_point<std::chrono::steady_clock> clock;

  protected:
    static void waitForNextStep(const std::pair<unsigned int, unsigned int> accessedElements);
};

#endif// SORTING_ALGORITHM_HPP