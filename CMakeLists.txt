cmake_minimum_required(VERSION 3.22)
project(LoopScheduler VERSION 0.0.1)

# Currently, JUCE supports only C++ 20 cross-platform.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# I like to download the dependencies to the same folder as the project.
# If you want to install them system wide, set CPM_SOURCE_CACHE with the path to the dependencies
# either as an environment variable or pass it to the cmake script with -DCPM_SOURCE_CACHE=<path>.
set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cmake-build-debug/lib)

include(cmake/cpm.cmake)
CPMAddPackage(
        NAME JUCE
        GIT_TAG 8.0.1 #this is done because JUCE versions don't follow the "vX.X.X" convention that cpm has for versions
        VERSION 8.0.1 #(like v8.0.1 instead of just 8.0.1)
        GITHUB_REPOSITORY juce-framework/JUCE
        SOURCE_DIR ${LIB_DIR}/juce
)

# Adds googletest.
CPMAddPackage(
        NAME GOOGLETEST
        GITHUB_REPOSITORY google/googletest
        GIT_TAG v1.15.2 #technically not necessary?
        VERSION 1.15.2
        SOURCE_DIR ${LIB_DIR}/googletest
        OPTIONS
        "INSTALL_GTEST OFF"
        "gtest_force_shared_crt ON"
)

CPMAddPackage(
        NAME melatonin_inspector
        GITHUB_REPOSITORY sudara/melatonin_inspector
        GIT_TAG main
        #VERSION main
        SOURCE_DIR ${LIB_DIR}/melatonin_inspector
)

CPMAddPackage(
        NAME juce-end-to-end
        GITHUB_REPOSITORY FocusriteGroup/juce-end-to-end
        VERSION 1.0.0
        SOURCE_DIR ${LIB_DIR}/je2e
)

CPMAddPackage(
        NAME jest
        GITHUB_REPOSITORY jestjs/jest
        VERSION 29.7.0
        SOURCE_DIR ${LIB_DIR}/jest
)

CPMAddPackage(
  NAME imgui
  GITHUB_REPOSITORY ocornut/imgui
  VERSION 1.91.9b-docking
  SOURCE_DIR ${LIB_DIR}/imgui
)

CPMAddPackage(
  NAME imgui_juce
  GITHUB_REPOSITORY hew02/imgui_juce
  GIT_TAG 1.0.0
  SOURCE_DIR ${LIB_DIR}/imgui_juce
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# This command allows running tests from the "build" folder (the one where CMake generates the project to).
enable_testing()

include_directories(include)

# Adds all the targets configured in the "plugin_test_cmake" folder.
add_subdirectory(src)

# Adds all the targets configured in the "test" folder.
#add_subdirectory(test)
