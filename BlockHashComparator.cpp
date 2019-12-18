#include "BlockHashComparator.h"
#include <iostream>

BlockHashComparator::BlockHashComparator(FileList_sized fileListS,
                                         HashType hasht,
                                         size_t block_size) :

                                         file_pack_size(fileListS.second),
                                         b_size(block_size),
                                         nblocks(0),
                                         EoF(false)
{
    for (auto &&file : fileListS.first)
    {
        fsfList.try_emplace(file, std::ifstream(file, std::ios::binary | std::ios_base::in));
    }
    hashPack.reserve(fileListS.first.size());
    hasher = HashFactory(hasht).createHash();
}

bool BlockHashComparator::Process()
{
    if (EoF)
        return false;

    Block b(b_size);
    hashFilesPack.clear();
    for (auto &[file, fst] : fsfList)
    {
        if ((nblocks + 1) * b_size < file_pack_size){

            if (!fst.read(b.data(), b_size).good()){
                std::cerr << "Fail to read block" << std::endl;
                return false;
            }
        }
        else{
            EoF = true;
            size_t remaindBytes = file_pack_size - nblocks * b_size;
            if (!fst.read(b.data(), remaindBytes).good()){
                std::cerr << "Fail to read remaind" << std::endl;
                return false;
            }
        }
        hashPack.push_back(hasher->compute(b));
        hashFilesPack[hasher->compute(b)].push_back(file);
    }
    ++nblocks;
    return true;
}

FileList BlockHashComparator::updateFiles(){
    FileList uniqueFiles;
    for(auto it = begin(hashFilesPack); it!=end(hashFilesPack);){
        if(it->second.size()==1){
            uniqueFiles.push_back(it->second.back());
            fsfList[it->second.back()].close();
            fsfList.erase(it->second.back());
            it = hashFilesPack.erase(it);
        }
        else
            ++it;
    }
    return uniqueFiles;
}

std::vector<FileList> BlockHashComparator::dumpDublicates(){
    std::vector<FileList> dublicate;
    for(auto&& group:hashFilesPack)
        dublicate.emplace_back(std::move(group.second));
    return dublicate;
}