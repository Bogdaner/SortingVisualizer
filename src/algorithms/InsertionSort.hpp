#ifndef INSERTION_SORT_HPP
#define INSERTION_SORT_HPP

#include "SortingAlgorithm.hpp"

class InsertionSort: public SortingAlgorithm
{
  public:
    std::string getAlgortihmName() override { return "Insertion Sort"; }

  private:
    void algorithm(std::vector<unsigned int> &inputVec, std::mutex &dataMutex) override
    {
        for (size_t i = 1; i < inputVec.size(); i++) {
            const auto key = inputVec.at(i);
            int j = i - 1;

            /* Move elements of inputVec[0..i-1], that are
            greater than key, to one position ahead
            of their current position */
            while (j >= 0 && inputVec.at(j) > key) {
                const std::lock_guard<std::mutex> guard(dataMutex);
                inputVec.at(j + 1) = inputVec.at(j);
                waitForNextStep({ inputVec.at(j + 1), inputVec.at(j) });
                j--;
            }

            const std::lock_guard<std::mutex> guard(dataMutex);
            inputVec.at(j + 1) = key;
            waitForNextStep({ inputVec.at(j + 1), key });
        }
    }
};

#endif// INSERTION_SORT_HPP