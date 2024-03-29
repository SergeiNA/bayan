cmake_minimum_required(VERSION 3.7)

if(NOT DEFINED ENV{TRAVIS_BUILD_NUMBER})
    set(ENV{TRAVIS_BUILD_NUMBER} 10000)
endif()

project(bayan VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})

find_package(Threads REQUIRED)
find_package(Boost COMPONENTS unit_test_framework
                              program_options 
                              filesystem 
                              REQUIRED)

set(sources bayan.cpp
            AppOption.h
            AppOption.cpp
            MainHandler.h
            MainHandler.cpp
            BlockHashComparator.h 
            BlockHashComparator.cpp 
            FileHandler.h 
            FileHandler.cpp
            FileUtilities.h
            FileUtilities.cpp
            common.h
            Hash.h)

set(bayanLibSrc AppOption.h
            AppOption.cpp
            MainHandler.h
            MainHandler.cpp
            BlockHashComparator.h 
            BlockHashComparator.cpp 
            FileHandler.h 
            FileHandler.cpp
            FileUtilities.h
            FileUtilities.cpp
            common.h
            Hash.h)

add_library(bayanLib ${bayanLibSrc})

add_executable(bayan bayan.cpp)
add_executable(FileHandlerTests FileHandlerTests.cpp)
add_executable(BlockHashComparatorTests BlockHashComparatorTests.cpp)

set_target_properties(  bayan 
                        FileHandlerTests 
                        BlockHashComparatorTests 
                        bayanLib 
                        
                PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra"
)

set_target_properties(  bayan 
                        FileHandlerTests 
                        BlockHashComparatorTests 
                        
                PROPERTIES
    COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
    INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)

target_link_libraries(bayan
    bayanLib
    Threads::Threads
    ${Boost_LIBRARIES}
)

target_link_libraries(FileHandlerTests
    bayanLib
    Threads::Threads
    ${Boost_LIBRARIES}
)

target_link_libraries(BlockHashComparatorTests
    bayanLib
    Threads::Threads
    ${Boost_LIBRARIES}
)

install(TARGETS bayan RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT "SergeiNA at github.com")

set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

include(CPack)

enable_testing()

add_test(FileHandlerTest FileHandlerTests)
add_test(FileHandlerTest BlockHashComparatorTests)