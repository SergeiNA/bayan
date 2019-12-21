#pragma once
#include "Common.h"

class FileHandler{
 
public:
    FileHandler(FileList files);

    std::vector<std::pair<FileList, size_t>>
    groupBySize();

    FileList GetUniqueFiles() const;

private:
    size_t getFileSize(const std::string& file);
private:
    FileList files_;
    std::vector<std::pair<FileList, size_t>> files_group_by_size;
    FileList uniques_filse_by_size;
};