cmake_minimum_required(VERSION 3.7)

if(NOT DEFINED ENV{TRAVIS_BUILD_NUMBER})
    set(ENV{TRAVIS_BUILD_NUMBER} 10000)
endif()

project(bayan VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})

find_package(Threads REQUIRED)
find_package(Boost COMPONENTS unit_test_framework program_options system REQUIRED)

set(sources bayan.cpp BlockHashComparator.h BlockHashComparator.cpp FileHandler.h FileHandler.cpp Hash.h)

add_executable(bayan ${sources})

set_target_properties(bayan PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra"
)

set_target_properties(bayan PROPERTIES
    COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
    INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)

target_link_libraries(bayan
    ${Boost_LIBRARIES}
)