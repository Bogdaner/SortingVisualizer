#ifndef BUBBLE_SORT_HPP
#define BUBBLE_SORT_HPP

#include "SortingAlgorithm.hpp"

class BubbleSort : public SortingAlgorithm
{
  public:
    std::string getAlgortihmName() override { return "Bubble Sort"; }

  private:
    void algorithm(std::vector<unsigned int> &inputVec, std::mutex &dataMutex) override
    {
        for (size_t i = 0; i < inputVec.size() - 1; i++) {
            // Last i elements are already in place
            for (size_t j = 0; j < inputVec.size() - i - 1; j++) {
                if (inputVec.at(j) > inputVec.at(j + 1)) {
                    const std::lock_guard<std::mutex> guard(dataMutex);
                    std::swap(inputVec.at(j), inputVec.at(j + 1));
                    waitForNextStep({ inputVec.at(j), inputVec.at(j + 1) }); 
                }
            }
        }
    }

  protected:
};

#endif// BUBBLE_SORT_HPP