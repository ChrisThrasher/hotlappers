cmake_minimum_required(VERSION 3.16)
project(hotlappers)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(external)

if(CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
    add_compile_options(-Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options(/WX /W4)
endif()

add_executable(hotlappers src/Main.cpp)
target_link_libraries(hotlappers PRIVATE SFML::Graphics ImGui-SFML::ImGui-SFML)

enable_testing()
add_subdirectory(tests)

add_custom_target(format
    COMMAND clang-format -i `git ls-files *.h *.cpp`
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
add_custom_target(tidy
    COMMAND clang-tidy -p "${CMAKE_BINARY_DIR}" `git ls-files *.cpp`
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
add_custom_target(run COMMAND hotlappers)
