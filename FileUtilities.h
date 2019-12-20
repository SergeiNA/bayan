#pragma once
#include "common.h"
#include <boost/filesystem.hpp>
#include <algorithm>

namespace fs = boost::filesystem;

void ValidateFileNames(FileList& fList);

FileList RecursiveFileList(const std::string& path_name);