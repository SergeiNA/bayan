#include "FileHandler.h"

#include <unordered_set>
#include <fstream>
#include <iostream>

#include <boost/bimap.hpp>
#include <boost/bimap/multiset_of.hpp>
#include <boost/filesystem.hpp>

FileHandler::FileHandler(FileList files) : files_(std::move(files)) {}

std::vector<std::pair<FileList, size_t>>
FileHandler::groupBySize(){
    boost::bimap<
        std::string,
        boost::bimaps::multiset_of<size_t>>
        alias;
    std::unordered_set<size_t> unique_sizes;

    for (auto &f : files_){
        auto fsize = getFileSize(f);
        unique_sizes.insert(fsize);
        alias.insert({f, fsize});
    }

    files_group_by_size.reserve(unique_sizes.size());

    for (auto us : unique_sizes){
        auto j = alias.right.equal_range(us);
        FileList flist;
        for (auto k = j.first; k != j.second; ++k){
            flist.push_back(k->second);
        }
        if (flist.size() == 1){
            uniques_filse_by_size.emplace_back(std::move(flist.back()));
            continue;
        }
        files_group_by_size.emplace_back(std::move(flist), us);
    }
    return files_group_by_size;
}

FileList FileHandler::GetUniqueFiles() const{
        return uniques_filse_by_size;
}

size_t FileHandler::getFileSize(const std::string& file){
    namespace fs = boost::filesystem;
    return fs::file_size(fs::path(file));
}