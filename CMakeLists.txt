cmake_minimum_required(VERSION 3.16)
project(hotlappers CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(cmake/CPM.cmake)
add_subdirectory(external)

if(CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
    add_compile_options(-Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options(/WX /W4 /permissive-)
endif()

add_library(hotlappers-core STATIC src/Vehicle.cpp)
target_include_directories(hotlappers-core PUBLIC include)
target_link_libraries(hotlappers-core PUBLIC SFML::Graphics)

add_executable(hotlappers src/Main.cpp src/App.cpp)
target_link_libraries(hotlappers PRIVATE hotlappers-core ImGui-SFML::ImGui-SFML)

enable_testing()
add_subdirectory(tests)

add_custom_target(format
    COMMAND clang-format -i `git ls-files *.hpp *.cpp`
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
add_custom_target(tidy
    COMMAND clang-tidy -p ${CMAKE_BINARY_DIR} `git ls-files *.cpp`
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
add_custom_target(run COMMAND hotlappers)
