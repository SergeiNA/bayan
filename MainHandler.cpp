#include "MainHandler.h"
#include "FileHandler.h"
#include "BlockHashComparator.h"

void PrintFileNames( const FileList& uniqueFiles,
                    const std::vector<FileSet>& groupDubs, 
                    std::ostream& os){
    if(!uniqueFiles.empty())
        os << "Unique files: " << std::endl;
    for(const auto& uf: uniqueFiles)
        os <<'\t'<< uf << std::endl;

    if(!groupDubs.empty())
        os << "Dublicate files: " << std::endl;
    int gcount =0;
    for(const auto& group:groupDubs){
        os<<'\t'<<"Group "<<++gcount<<": ";
        for(const auto& file: group)
            os<<file<<' ';
        os<<std::endl;
    }
}

std::pair<FileList,std::vector<FileSet>> 
groupFiles(FileList files_, HashType hashType, size_t blockSize){
    if(files_.size()<2)
        std::cerr<<"Non enough data to compare!"<<std::endl;

    FileHandler fileHandler(files_);
    auto groupFiles = fileHandler.groupBySize();
    FileList uniqueFiles(fileHandler.GetUniqueFiles());
    std::vector<FileSet> groupDubs;

    for(auto&& fgroup:groupFiles){
        BlockHashComparator blockHasher(fgroup, hashType, blockSize);
        
        blockHasher.Process();
        auto uniqueByContent = blockHasher.GetUniqueFiles();
        uniqueFiles.reserve(uniqueFiles.size() + uniqueByContent.size());
        uniqueFiles.insert(end(uniqueFiles),begin(uniqueByContent),end(uniqueByContent) );
        
        for(auto&& group:blockHasher.DumpDublicates())
            groupDubs.emplace_back(std::move(group));

    }
    return std::make_pair(uniqueFiles,groupDubs);
}