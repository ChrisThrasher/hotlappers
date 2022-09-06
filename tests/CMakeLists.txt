list(APPEND CMAKE_MODULE_PATH ${CPM_PACKAGE_Catch2_SOURCE_DIR}/extras)
include(Catch)

add_executable(test-vehicle Vehicle.cpp)
target_link_libraries(test-vehicle PRIVATE hotlappers-core Catch2::Catch2WithMain)
catch_discover_tests(test-vehicle)

add_executable(test-lowpassfilter LowPassFilter.cpp)
target_link_libraries(test-lowpassfilter PRIVATE hotlappers-core Catch2::Catch2WithMain)
catch_discover_tests(test-lowpassfilter)
