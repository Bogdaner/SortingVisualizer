message(STATUS "Fetching SFML...")

set(BUILD_SHARED_LIBS OFF)
set(SFML_BUILD_EXAMPLES TRUE)

FetchContent_MakeAvailable(sfml)