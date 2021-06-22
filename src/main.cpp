#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include <imgui-SFML.h>
#include <imgui.h>

#include "Visualizer.hpp"

int main()
{
    const unsigned int windowWidth =
      static_cast<unsigned int>(static_cast<float>(sf::VideoMode::getDesktopMode().width) * 0.75F);

    const unsigned int windowHeight =
      static_cast<unsigned int>(static_cast<float>(sf::VideoMode::getDesktopMode().height) * 0.75F);

    const std::string windowTitle = "SortingVisualizer";

    sf::RenderWindow window;
    window.create(sf::VideoMode{ windowWidth, windowHeight }, windowTitle);
    window.setFramerateLimit(144);
    ImGui::SFML::Init(window);

    constexpr float imGuIScaleFactor = 2.0F;
    ImGui::GetStyle().ScaleAllSizes(imGuIScaleFactor);
    ImGui::GetIO().FontGlobalScale = imGuIScaleFactor;

    sf::Clock deltaClock{};
    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed) {
                window.close();
                Visualizer::forceAndWaitForSortingToFinish();
            }
            if (event.type == sf::Event::Resized) {
                // Update the view to the new size of the window
                const sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        Visualizer::drawArray(window);
        Visualizer::drawControlsMenu(window);

        window.display();
        window.clear();
    }

    ImGui::SFML::Shutdown();

    return 0;
}
