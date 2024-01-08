cmake_minimum_required(VERSION 3.26)

project(AudioPluginTest)

enable_testing()

add_executable(${PROJECT_NAME}
    source/AudioProcessorTest.cpp
        ../plugin/source/WavetableSynth.cpp
        ../plugin/include/CraneFlyFirstPlugin/WavetableSynth.h
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${GOOGLE_TEST_SOURCE_DIR}/googletest/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../plugin/include
        ${JUCE_SOURCE_DIR}/modules
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        AudioPlugin
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
