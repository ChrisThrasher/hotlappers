add_executable(test-vehicle Vehicle.cpp)
target_link_libraries(test-vehicle PRIVATE hotlappers-core Catch2::Catch2WithMain)
add_test(NAME test-vehicle COMMAND test-vehicle)

add_executable(test-lowpassfilter LowPassFilter.cpp)
target_link_libraries(test-lowpassfilter PRIVATE hotlappers-core Catch2::Catch2WithMain)
add_test(NAME test-lowpassfilter COMMAND test-lowpassfilter)
