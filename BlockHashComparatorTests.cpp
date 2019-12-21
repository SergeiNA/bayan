#define BOOST_TEST_MODULE blockHashComparator

#include "BlockHashComparator.h"
#include "Hash.h"
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <string>
#include <iostream>

BOOST_AUTO_TEST_SUITE(blockHashComparator)

std::vector<std::pair<FileList, size_t>> 
groupbySize{{
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

BOOST_AUTO_TEST_CASE(uniqueFilesCheck)
{
    FileList uniqueChech ={
        "TestData/s25tC_0.txt",
        "TestData/s30tC_0.txt",
        "TestData/s30tB_0.txt"
    };
    
    FileList uniqueFiles;

    for(const auto& fgroup:groupbySize){
        BlockHashComparator blockHasher(fgroup, HashType::BOOST, 6);

        while (blockHasher.Process())
            for (auto &&uf : blockHasher.updateFiles())
                uniqueFiles.emplace_back(uf);
    }
    BOOST_CHECK_EQUAL_COLLECTIONS(uniqueChech.begin(),uniqueChech.end(),
                            uniqueFiles.begin(),uniqueFiles.end());
}

BOOST_AUTO_TEST_CASE(dublicatesCheck)
{
    std::vector<FileSet> 
    dubsChech{
            {
                "TestData/s25tA_0.txt",
                "TestData/s25tA_1.txt"

            },
            {
                "TestData/s25tB_0.txt",
                "TestData/s25tB_1.txt"
            },
            {
                "TestData/s30tA_0.txt",
                "TestData/s30tA_1.txt"
            }
    };

    std::vector<FileSet> dubs;

    for(const auto& fgroup:groupbySize){
        BlockHashComparator blockHasher(fgroup, HashType::BOOST, 6);

        while (blockHasher.Process())
            for ([[maybe_unused]]auto &&uf : blockHasher.updateFiles());

        for(auto&& group:blockHasher.dumpDublicates())
            dubs.emplace_back(std::move(group));
    }
    for(size_t i=0;i<dubsChech.size();++i){
        BOOST_CHECK_EQUAL(dubsChech.at(i).size(),dubs.at(i).size());

        BOOST_CHECK_EQUAL_COLLECTIONS(dubsChech.at(i).begin(),
                                        dubsChech.at(i).end(),
                                        dubs.at(i).begin(),
                                        dubs.at(i).end());
    }
}

BOOST_AUTO_TEST_SUITE_END()