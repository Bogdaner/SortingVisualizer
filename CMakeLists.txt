cmake_minimum_required(VERSION 3.15)

project(SortingVisualizer CXX)
include(cmake/StandardProjectSettings.cmake)
include(cmake/PreventInSourceBuilds.cmake)

# Link this 'library' to use specified options
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_17)

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)

# enable cache system
include(cmake/Cache.cmake)

# standard compiler warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

# sanitizer options if supported by compiler
include(cmake/Sanitizers.cmake)
enable_sanitizers(project_options)

# Very basic PCH example
option(ENABLE_PCH "Enable Precompiled Headers" OFF)
if(ENABLE_PCH)
  # This sets a global PCH parameter, each project will build its own PCH, which is a good idea if any #define's change
  #
  # consider breaking this out per project as necessary
  target_precompile_headers(
    project_options
    INTERFACE
    <vector>
    <string>
    <map>
    <utility>)
endif()

add_subdirectory(dependencies)

# allow for static analysis options
include(cmake/StaticAnalyzers.cmake)

add_subdirectory(src)

list(APPEND Targets SortingVisualizer)
if(ENABLE_CLANG_TIDY)
  if (CMAKE_GENERATOR MATCHES "Visual Studio")
  # Visual Studio Code Analysis
    foreach(target IN LISTS Targets)
      set_target_properties(${target} PROPERTIES
          
          # Run code analysis on build
          # This is set to false delibarately. We already gave compiler warnings in CompilerWarnings file
          VS_GLOBAL_RunCodeAnalysis false

          # MSVC
          VS_GLOBAL_EnableMicrosoftCodeAnalysis true
          VS_GLOBAL_CodeAnalysisRuleSet AllRules.ruleset  # See for other rulesets: C:\Program Files (x86)\Microsoft Visual Studio\2019\Preview\Team Tools\Static Analysis Tools\Rule Sets\

          # Clang tidy
          VS_GLOBAL_EnableClangTidyCodeAnalysis true
      )
    endforeach()
  endif()
endif()
