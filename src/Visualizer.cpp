#include "Visualizer.hpp"

void Visualizer::drawArray(sf::RenderWindow &window)
{
    sf::RectangleShape sampleRect{};
    sampleRect.setOutlineColor(sf::Color{ 128U, 128U, 128U });
    sampleRect.setOutlineThickness(outlineThickness);

    for (size_t i = 0; i < arrayToVisualize.size(); ++i) {
        sampleRect.setPosition(getRectPos(window, static_cast<float>(arrayToVisualize.at(i)), i));
        sampleRect.setSize(getRectSize(window, static_cast<float>(arrayToVisualize.at(i))));
        sampleRect.setFillColor(sf::Color::White);

        // Colour last accessed elements with diffrent red shades
        const int indexInQueue = SortingAlgorithm::isLastAccessed(arrayToVisualize.at(i));
        if (indexInQueue != -1) {
            const float factor =
              static_cast<float>(indexInQueue + 1)
              / static_cast<float>(SortingAlgorithm::getCurrLastAcccessedSize());// factor is form is <0, 1>
            const auto color = sf::Color(200 * factor + 55, 0, 0);

            sampleRect.setFillColor(color);
        }

        // Alements that are sorted are green
        if (arrayToVisualize.at(i) == i + 1) { sampleRect.setFillColor(sf::Color::Green); }

        window.draw(sampleRect);
    }
}

void Visualizer::drawControlsMenu(sf::RenderWindow &window)
{
    constexpr float slidersWidth = 300.F;
    const ImVec2 buttonsSize = ImVec2{ 120, 0 };

    constexpr int minArrElems = 3;
    constexpr int maxArrElems = 1000;

    constexpr int minDelay = 1;
    constexpr int maxDelay = 1000U;

    constexpr int minLastAccessed = 1;
    constexpr int maxLastAccessed = 1000;

    constexpr float stepOutlineThic = 0.05F;
    constexpr float minOutlineThic = 0.0F;
    constexpr float maxOutlineThic = 10.0F;

    std::string zeroSeparatedAlgorithms{};
    for (const auto &alg : algorithms) { zeroSeparatedAlgorithms += alg->getAlgortihmName() + '\0'; }

    ImGui::SetNextWindowPos({ 0.0F, 0.0F }, ImGuiCond_Always);
    ImGui::SetNextWindowSize({ 800.0F, 450.0F }, ImGuiCond_Once);

    ImGui::Begin("Controls & Stats", nullptr);

    if (ImGui::CollapsingHeader("Controls")) {

        ImGui::SetNextItemWidth(slidersWidth);
        ImGui::Combo("Sorting algorithm", &selectedAlgorithm, zeroSeparatedAlgorithms.c_str());

        ImGui::SetNextItemWidth(slidersWidth);
        ImGui::SliderInt("Array elements", reinterpret_cast<int *>(&arrayElements), minArrElems, maxArrElems);

        ImGui::SetNextItemWidth(slidersWidth);
        ImGui::SliderInt(
          "Step delay", reinterpret_cast<int *>(&SortingAlgorithm::stepDelay), minDelay, maxDelay, "%d ms");

        ImGui::SetNextItemWidth(slidersWidth);
        ImGui::SliderInt("Last accessed elements marked",
          reinterpret_cast<int *>(&SortingAlgorithm::maxLastAccessedStored),
          minLastAccessed,
          maxLastAccessed,
          "%d");

        ImGui::SetNextItemWidth(slidersWidth);
        ImGui::DragFloat(
          "Outline thickness\n", &outlineThickness, stepOutlineThic, minOutlineThic, maxOutlineThic, "%.02f");

        if (ImGui::Button("Sort", buttonsSize) && !SortingAlgorithm::isSorting()) {
            std::thread t1{ &SortingAlgorithm::doSort,
                algorithms.at(selectedAlgorithm).get(),
                std::ref(arrayToVisualize),
                std::ref(arrayMutex) };
            t1.detach();

            tableItems.push_back(
              TableItem{ SortingStatistics{}, algorithms.at(selectedAlgorithm)->getAlgortihmName() });
        }

        ImGui::SameLine();
        if (ImGui::Button("Shuffle", buttonsSize)) { shuffleVisualizationArray(); }
    }

    if (SortingAlgorithm::isSorting()) { tableItems.back().stats = SortingAlgorithm::getCurrentStatistics(); }

    if (ImGui::CollapsingHeader("Stats")) {
        static const ImGuiTableFlags flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter
                                             | ImGuiTableFlags_BordersV | ImGuiTableFlags_NoBordersInBody
                                             | ImGuiTableFlags_ScrollY | ImGuiTableFlags_SizingStretchProp;
        if (ImGui::BeginTable(
              "Statistics_Table", 5, flags, ImVec2(0.0F, ImGui::GetTextLineHeightWithSpacing() * 8), 0.0F)) {
            ImGui::TableSetupColumn("ID", 0.0F);
            ImGui::TableSetupColumn("Sorting Algorithm", 0.0F);
            ImGui::TableSetupColumn("Swaps", 0.0F);
            ImGui::TableSetupColumn("Elements", 0.0F);
            ImGui::TableSetupColumn("Time [ms]", 0.0F);
            ImGui::TableSetupScrollFreeze(0, 1);// Make row always visible
            ImGui::TableHeadersRow();

            ImGuiListClipper clipper;
            clipper.Begin(tableItems.Size);
            while (clipper.Step())
                for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++) {
                    // Display a data item
                    TableItem *item = &tableItems[tableItems.size() - 1 - row_n];
                    ImGui::PushID(row_n);
                    ImGui::TableNextRow();

                    ImGui::TableNextColumn();
                    ImGui::Text("%04d", row_n);
                    ImGui::TableNextColumn();
                    ImGui::TextUnformatted(item->algorithmUsed.c_str());
                    ImGui::TableNextColumn();
                    ImGui::Text("%d", item->stats.swaps);
                    ImGui::TableNextColumn();
                    ImGui::Text("%d", item->stats.elements);
                    ImGui::TableNextColumn();
                    ImGui::Text("%lld", item->stats.duration);
                    ImGui::PopID();
                }

            ImGui::EndTable();
        }
    }

    ImGui::End();
    ImGui::SFML::Render(window);

    // Slider can update array size so this call goes here
    resizeVisualizationArray();
}

sf::Vector2f Visualizer::getRectSize(const sf::RenderWindow &window, const float value)
{
    const auto fnum = static_cast<float>(arrayToVisualize.size());
    const float rectWidth = (window.getSize().x - (outlineThickness * fnum) * 2.0F) / fnum;
    const float rectHeight = (value / fnum * window.getSize().y) - outlineThickness * 2.0F;
    return sf::Vector2f{ rectWidth, rectHeight };
}

sf::Vector2f Visualizer::getRectPos(const sf::RenderWindow &window, const float value, const size_t elemIndex)
{
    const sf::Vector2f rectSize = getRectSize(window, value);
    const auto findex = static_cast<float>(elemIndex);

    const float rectPosX = (2.0F * (findex * outlineThickness) + (findex * rectSize.x)) + outlineThickness;
    const float rectPosY = window.getSize().y - rectSize.y - outlineThickness;

    return sf::Vector2f{ rectPosX, rectPosY };
}

void Visualizer::resizeVisualizationArray()
{
    if (arrayElements != arrayToVisualize.size()) {
        forceAndWaitForSortingToFinish();
        const std::lock_guard<std::mutex> guard(arrayMutex);
        arrayToVisualize.resize(arrayElements);
        constexpr unsigned int startingVal = 1U;
        std::iota(arrayToVisualize.begin(), arrayToVisualize.end(), startingVal);
    }
}

void Visualizer::shuffleVisualizationArray()
{
    forceAndWaitForSortingToFinish();
    const std::lock_guard<std::mutex> guard(arrayMutex);
    std::shuffle(std::begin(arrayToVisualize), std::end(arrayToVisualize), std::mt19937{ std::random_device{}() });
}

void Visualizer::forceAndWaitForSortingToFinish()
{
    // Ugly way to wait until detached thread will finish
    const auto storedDelay = SortingAlgorithm::stepDelay;
    SortingAlgorithm::stepDelay = 0;
    while (SortingAlgorithm::isSorting()) {}// is sorting is atomic to not optimize this loop
    SortingAlgorithm::stepDelay = storedDelay;
}


// Static members init
std::vector<std::unique_ptr<SortingAlgorithm>> Visualizer::algorithms = [] {
    std::vector<std::unique_ptr<SortingAlgorithm>> ret;
    ret.emplace_back(std::make_unique<BubbleSort>(BubbleSort{}));
    ret.emplace_back(std::make_unique<QuickSort>(QuickSort{}));
    return ret;
}();

std::vector<unsigned int> Visualizer::arrayToVisualize{};
std::mutex Visualizer::arrayMutex{};

ImVector<TableItem> Visualizer::tableItems{};

size_t Visualizer::arrayElements = 10U;
float Visualizer::outlineThickness = 0.5F;
int Visualizer::selectedAlgorithm = 0;