#define BOOST_TEST_MODULE file_handler_test

#include "FileHandler.h"
#include <boost/test/unit_test.hpp>
#include <string>
#include <iostream>

BOOST_AUTO_TEST_SUITE(FileHandlerTest)

FileList files_ ={  "TestData/s25tA_0.txt",
                    "TestData/s25tA_1.txt",
                    "TestData/s25tB_0.txt",
                    "TestData/s25tB_1.txt",
                    "TestData/s25tC_0.txt",
                    "TestData/s30tA_0.txt",
                    "TestData/s30tA_1.txt",
                    "TestData/s30tB_0.txt",
                    "TestData/s30tC_0.txt",
                    "TestData/s60tA_0.txt"};

BOOST_AUTO_TEST_CASE(uniqueFilesBySize)
{
    FileList uniqueChech{"TestData/s60tA_0.txt"};
    FileHandler fileHandler(files_);
    [[maybe_unused]]auto groupFiles = fileHandler.groupBySize();
    FileList uniqueFiles(fileHandler.GetUniqueFiles());
    BOOST_CHECK_EQUAL(uniqueChech.size(),uniqueFiles.size());
    BOOST_CHECK_EQUAL_COLLECTIONS(uniqueChech.begin(),uniqueChech.end(),
                            uniqueFiles.begin(),uniqueFiles.end());
}

BOOST_AUTO_TEST_CASE(sameFileBySize)
{
    std::vector<std::pair<FileList, size_t>> 
    groupbySizeChech{{
            {
                "TestData/s25tA_0.txt",
                "TestData/s25tA_1.txt",
                "TestData/s25tB_0.txt",
                "TestData/s25tB_1.txt",
                "TestData/s25tC_0.txt"
            },
        25},
        {
            {
                "TestData/s30tA_0.txt",
                "TestData/s30tA_1.txt",
                "TestData/s30tB_0.txt",
                "TestData/s30tC_0.txt"
            },
        30}
    };

    FileHandler fileHandler(files_);
    auto groupFiles = fileHandler.groupBySize();

    for(size_t i=0;i<groupbySizeChech.size();++i){
        BOOST_CHECK_EQUAL(groupbySizeChech.at(i).second,groupFiles.at(i).second);

        BOOST_CHECK_EQUAL_COLLECTIONS(  groupbySizeChech.at(i).first.begin(),
                                        groupbySizeChech.at(i).first.end(),
                                        groupFiles.at(i).first.begin(),
                                        groupFiles.at(i).first.end());
    }
}

BOOST_AUTO_TEST_SUITE_END()