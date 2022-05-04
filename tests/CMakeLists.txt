add_executable(test-hotlappers Test.cpp)
target_link_libraries(test-hotlappers PRIVATE hotlappers-core Catch2::Catch2WithMain)
add_test(NAME test-hotlappers COMMAND test-hotlappers)
