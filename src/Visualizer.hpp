#ifndef VISUALIZER_HPP
#define VISUALIZER_HPP

#include <memory>
#include <mutex>
#include <numeric>
#include <random>
#include <thread>

#include <SFML/Graphics.hpp>

#include <imgui-SFML.h>
#include <imgui.h>

#include "algorithms/BubbleSort.hpp"
#include "algorithms/QuickSort.hpp"
#include "algorithms/SortingAlgorithm.hpp"
#include "algorithms/MergeSort.hpp"
#include "algorithms/InsertionSort.hpp"

struct TableItem
{
    SortingStatistics stats{};
    std::string algorithmUsed = "";
};

class Visualizer
{
  public:
    Visualizer() = delete;

    static void drawArray(sf::RenderWindow &window);
    static void drawControlsMenu(sf::RenderWindow &window);
    static void forceAndWaitForSortingToFinish();

  protected:
  private:
    static sf::Vector2f getRectSize(const sf::RenderWindow &window, const float value);
    static sf::Vector2f getRectPos(const sf::RenderWindow &window, const float value, const size_t elemIndex);
    static void resizeVisualizationArray();
    static void shuffleVisualizationArray();

    static std::vector<std::unique_ptr<SortingAlgorithm>> algorithms;
    static std::vector<unsigned int> arrayToVisualize;
    static std::mutex arrayMutex;

    static ImVector<TableItem> tableItems;

    static size_t arrayElements;
    static float outlineThickness;
    static int selectedAlgorithm;
};

#endif// VISUALIZER_HPP
