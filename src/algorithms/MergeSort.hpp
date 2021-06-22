#ifndef MERGE_SORT_HPP
#define MERGE_SORT_HPP

#include "SortingAlgorithm.hpp"

class MergeSort : public SortingAlgorithm
{
  public:
    std::string getAlgortihmName() override { return "Merge Sort"; }

  private:
    void algorithm(std::vector<unsigned int> &inputVec, std::mutex &dataMutex) override
    {
        mergeSort(inputVec, dataMutex, 0, inputVec.size() - 1);
    }

    static void mergeSort(std::vector<unsigned int> &inputVec, std::mutex &dataMutex, const size_t l, const size_t r)
    {
        if (l >= r) {
            return;// returns recursively
        }
        const size_t m = l + (r - l) / 2;
        mergeSort(inputVec, dataMutex, l, m);
        mergeSort(inputVec, dataMutex, m + 1, r);
        merge(inputVec, dataMutex, l, m, r);
    }

    static void
      merge(std::vector<unsigned int> &inputVec, std::mutex &dataMutex, const size_t l, const size_t m, const size_t r)
    {
        const size_t n1 = m - l + 1;
        const size_t n2 = r - m;

        // Create temp arrays
        std::vector<unsigned int> L{};
        std::vector<unsigned int> R{};
        L.resize(n1);
        R.resize(n2);

        // Copy data to temp arrays L[] and R[]
        for (size_t i = 0; i < n1; i++) { L.at(i) = inputVec.at(l + i); }
        for (size_t j = 0; j < n2; j++) { R.at(j) = inputVec.at(m + 1 + j); }

        // Merge the temp arrays back into

        // Initial index of first subarray
        size_t i = 0;

        // Initial index of second subarray
        size_t j = 0;

        // Initial index of merged subarray
        size_t k = l;

        while (i < n1 && j < n2) {
            const std::lock_guard<std::mutex> guard(dataMutex);
            if (L.at(i) <= R.at(j)) {
                inputVec.at(k) = L.at(i);
                waitForNextStep({ inputVec.at(k), L.at(i++) });
            } else {
                inputVec.at(k) = R.at(j);
                waitForNextStep({ inputVec.at(k), R.at(j++) });
            }
            k++;
        }

        // Copy the remaining elements of
        // L[], if there are any
        while (i < n1) {
            const std::lock_guard<std::mutex> guard(dataMutex);
            inputVec.at(k) = L.at(i);
            waitForNextStep({ inputVec.at(k++), L.at(i++) });
        }

        // Copy the remaining elements of
        // R[], if there are any
        while (j < n2) {
            const std::lock_guard<std::mutex> guard(dataMutex);
            inputVec.at(k) = R.at(j);
            waitForNextStep({ inputVec.at(k++), R.at(j++) });
        }
    }
};

#endif// MERGE_SORT_HPP
