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
    fileMask.resize(1);
    for (auto &&file : fileListS.first)
    {
        fsfList.try_emplace(file, std::ifstream());
        fsfList[file].rdbuf()->pubsetbuf(nullptr,0);
        fsfList[file].open(file, std::ios::binary | std::ios_base::in );
        fileMask.at(0).insert(file);
    }
    hasher = HashFactory(hasht).createHash();
}

bool BlockHashComparator::hashBlocksProcess()
{
    if (EoF || fileMask.empty())
        return false;
    Block b(b_size);
    hashFilesPack.clear();
    hashFilesPack.resize(fileMask.size());
    size_t i=0;
    for (const auto &fileGroup : fileMask){
        for(const auto& file:fileGroup){

            if ((nblocks + 1) * b_size < file_pack_size){
                if (!fsfList[file].read(b.data(), b_size).good()){
                    std::cerr << "Fail to read block" << std::endl;
                    return false;
                }
            }
            else{
                EoF = true;
                size_t remaindBytes = file_pack_size - nblocks * b_size;
                if (!fsfList[file].read(b.data(), remaindBytes).good()){
                    std::cerr << "Fail to read remaind" << std::endl;
                    return false;
                }
            }
            hashFilesPack.at(i)[hasher->compute(b)].emplace(file);
        }
        ++i;
    }
    ++nblocks;
    return true;
}

void BlockHashComparator::updateFileMask(){
    fileMask.clear();
    for(auto&& dupGroup:hashFilesPack){
        for (auto &&[hash,hashGroup]:dupGroup){
            (void)hash;
            if (hashGroup.size() == 1){
                uniqueFiles.push_back(*hashGroup.begin());
                fsfList[*hashGroup.begin()].close();
                fsfList.erase(*hashGroup.begin());
            }
            else
                fileMask.emplace_back(hashGroup);
        }
    }
}

void BlockHashComparator::Process(){
    while (hashBlocksProcess())
        updateFileMask();
}

FileList BlockHashComparator::GetUniqueFiles() const {
    return uniqueFiles;
}

std::vector<FileSet> BlockHashComparator::DumpDublicates(){
    std::vector<FileSet> dublicate;
    for(auto&& group:fileMask)
        dublicate.emplace_back(std::move(group));
    return dublicate;
}