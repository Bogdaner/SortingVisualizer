add_executable(SortingVisualizer 
    main.cpp
    Visualizer.hpp
    Visualizer.cpp
    algorithms/SortingAlgorithm.hpp
    algorithms/SortingAlgorithm.cpp
    algorithms/BubbleSort.hpp
    algorithms/QuickSort.hpp
)

target_link_libraries(SortingVisualizer PRIVATE ImGui-SFML::ImGui-SFML)