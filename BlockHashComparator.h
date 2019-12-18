#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>

#include "Hash.h"

class BlockHashComparator{
    typedef std::vector<char> Block;
    typedef std::pair<std::vector<std::string>,size_t> FileList_sized;
    typedef std::unordered_map<std::string, std::ifstream> FStreamFileList;
    typedef std::unordered_map<Hash_value, FileList> HashFileListPack;

public:
    BlockHashComparator(FileList_sized fileListS,
        HashType hasht, 
        size_t block_size);

    bool Process();
    
    FileList updateFiles();

    HashPack& GetHashPack(){
        return hashPack;
    }

    std::vector<FileList> dumpDublicates();

private:
    size_t file_pack_size;
    size_t b_size;
    size_t nblocks;
    bool EoF;

    FStreamFileList fsfList;
    std::unique_ptr<Hash> hasher;
    HashFileListPack hashFilesPack;
    HashPack hashPack;
};