add_executable(test-vehicle Vehicle.cpp)
target_link_libraries(test-vehicle PRIVATE hotlappers-core Catch2::Catch2WithMain)
if(WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET test-vehicle POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:test-vehicle> $<TARGET_FILE_DIR:test-vehicle> COMMAND_EXPAND_LISTS)
endif()
add_test(NAME test-vehicle COMMAND test-vehicle)

add_executable(test-lowpassfilter LowPassFilter.cpp)
target_link_libraries(test-lowpassfilter PRIVATE hotlappers-core Catch2::Catch2WithMain)
if(WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET test-lowpassfilter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:test-lowpassfilter> $<TARGET_FILE_DIR:test-lowpassfilter> COMMAND_EXPAND_LISTS)
endif()
add_test(NAME test-lowpassfilter COMMAND test-lowpassfilter)
