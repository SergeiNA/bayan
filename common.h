#pragma once

#include <string>
#include <vector>
#include <set>

typedef std::string FileName;
typedef std::set<FileName> FileSet;
typedef std::vector<FileName> FileList;

typedef std::string Hash_value;

enum class HashType {
    BOOST, 
    SHA1, 
    CRC };
