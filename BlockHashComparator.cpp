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
        fsfList.try_emplace(file, std::ifstream(file, std::ios::binary | std::ios_base::in));
        fileMask.at(0).insert(file);
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
    hashFilesPack.resize(fileMask.size());
    size_t i=0;
    for (const auto &fileGroup : fileMask)
    {
        for(const auto& file:fileGroup){

            if ((nblocks + 1) * b_size < file_pack_size)
            {

                if (!fsfList[file].read(b.data(), b_size).good())
                {
                    std::cerr << "Fail to read block" << std::endl;
                    return false;
                }
            }
            else
            {
                EoF = true;
                size_t remaindBytes = file_pack_size - nblocks * b_size;
                if (!fsfList[file].read(b.data(), remaindBytes).good())
                {
                    std::cerr << "Fail to read remaind" << std::endl;
                    return false;
                }
            }
            hashPack.push_back(hasher->compute(b));
            hashFilesPack.at(i)[hasher->compute(b)].emplace(file);
        }
        ++i;
    }
    ++nblocks;
    return true;
}

FileList BlockHashComparator::updateFiles(){
    FileList uniqueFiles;
    fileMask.clear();
    for(auto&& dupGroup:hashFilesPack){

        for (auto &&[hash,hashGroup]:dupGroup)
        {
            (void)hash;
            // std::cerr << "hashFilesPack: " << std::endl;
            // for (const auto &f : hashGroup)
            //     std::cerr << '\t' << f << std::endl;
            if (hashGroup.size() == 1)
            {
                std::cerr << "Unique: " << *hashGroup.begin() << std::endl;
                uniqueFiles.push_back(*hashGroup.begin());
                fsfList[*hashGroup.begin()].close();
                fsfList.erase(*hashGroup.begin());
            }
            else
                fileMask.emplace_back(hashGroup);
        }
    }
    return uniqueFiles;
}

std::vector<FileSet> BlockHashComparator::dumpDublicates(){
    std::vector<FileSet> dublicate;
    for(auto&& group:fileMask)
        dublicate.emplace_back(std::move(group));
    return dublicate;
}