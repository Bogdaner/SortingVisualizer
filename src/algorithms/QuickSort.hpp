#ifndef QUICK_SORT_HPP
#define QUICK_SORT_HPP

#include "SortingAlgorithm.hpp"

class QuickSort : public SortingAlgorithm
{
  public:
    std::string getAlgortihmName() override { return "Quick Sort"; }

  private:
    void algorithm(std::vector<unsigned int> &inputVec, std::mutex &dataMutex) override
    {
        quickSort(inputVec, dataMutex, 0, inputVec.size() - 1);
    }

    static void
      quickSort(std::vector<unsigned int> &inputVec, std::mutex &dataMutex, const size_t low, const size_t high)
    {
        if (low < high) {
            const size_t pi = partition(inputVec, low, high, dataMutex);

            // Separately sort elements before
            // partition and after partition
            quickSort(inputVec, dataMutex, low, (pi == 0) ? 0 : pi - 1);
            quickSort(inputVec, dataMutex, pi + 1, high);
        }
    }

    static size_t
      partition(std::vector<unsigned int> &inputVec, const size_t low, const size_t high, std::mutex &dataMutex)
    {
        const size_t pivot = inputVec.at(high);// pivot
        size_t i = (low - 1);// Index of smaller element
        for (size_t j = low; j <= high - 1; j++) {
            // If current element is smaller than or equal to pivot
            if (inputVec.at(j) <= pivot) {
                const std::lock_guard<std::mutex> guard(dataMutex);
                std::swap(inputVec.at(++i), inputVec.at(j));
                if (i != j) { waitForNextStep({ inputVec.at(i), inputVec.at(j) }); }
            }
        }

        const std::lock_guard<std::mutex> guard(dataMutex);
        std::swap(inputVec.at(i + 1), inputVec.at(high));
        waitForNextStep({ inputVec.at(i + 1), inputVec.at(high) });

        return (i + 1);
    }
};

#endif// QUICK_SORT_HPP
