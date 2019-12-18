#include <iostream>
#include <fstream>
#include <unordered_map>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include "FileHandler.h"
#include "BlockHashComparator.h"

namespace popt = boost::program_options;
namespace fs = boost::filesystem;

void set_hasher(std::string hash) {
    std::string hash_(std::move(hash));
    std::cout << "set hasher to " << hash_ << std::endl;
} 

void set_block(size_t size) {
    size_t size_(size);
    std::cout << "set block size to " << size_ << std::endl;
}

void set_files(FileList files_){
    if(files_.size()<2)
        std::cerr<<"No data to compare!"<<std::endl;
    for(auto&&f:files_)  
        std::cout<<"FileName: "<<f<<std::endl;
}

void Run(FileList files_, HashType hashType, size_t blockSize){
    if(files_.size()<2)
        std::cerr<<"No data to compare!"<<std::endl;
    FileHandler fileHandler(files_);
    auto groupFiles = fileHandler.groupBySize();
    FileList uniqueFiles(fileHandler.GetUniqueFiles());
    std::vector<FileList> groupDubs;

    for(auto&& fgroup:groupFiles){
        BlockHashComparator blockHasher(fgroup, hashType, blockSize);
        while (blockHasher.Process()){
            for (auto &&uf : blockHasher.updateFiles())
                uniqueFiles.emplace_back(uf);
        }
        for(auto&& group:blockHasher.dumpDublicates())
            groupDubs.emplace_back(std::move(group));
    }

    std::cout << "Unique files: " << std::endl;
    for(const auto& uf: uniqueFiles)
        std::cout << uf << std::endl;
    std::cout << "Dublicate files: " << std::endl;

    int gcount =0;
    for(const auto& group:groupDubs){
        std::cout<<"Group "<<++gcount<<": ";
        for(const auto& file: group)
            std::cout<<file<<' ';
        std::cout<<std::endl;
    }
}


int main(int argc, const char *argv[]){
    FileList fileList;
    std::map<std::string, HashType> hashTypeSelector{
        {"boost",HashType::BOOST},
        {"crc",HashType::CRC},
        {"sha1",HashType::SHA1}
    };

    HashType hashType;
    size_t blockSize;
    try {
        popt::options_description desc{"Options"};
        desc.add_options()
                ("help,h", "show help")
                ("files,F", popt::value<std::vector<std::string>>()->multitoken()->
                        zero_tokens()->composing()->notifier(set_files), "files to compare, able to use without command")
                ("hash,H", popt::value<std::string>()->default_value("boost"), "hash func for comparation: crc/boost") //->notifier(set_hasher)
                ("block,B", popt::value<size_t>()->default_value(1), "block size (bytes)");


        popt::positional_options_description pos_descr;
        pos_descr.add("files",-1);

        popt::command_line_parser parser{argc,argv};
        parser.options(desc).positional(pos_descr).allow_unregistered();
        popt::parsed_options parsed_options = parser.run();

        popt::variables_map vm;
        store(parsed_options, vm);
        notify(vm);

        if (vm.count("help"))
            std::cout << desc << '\n';
        if (vm.count("hash")){
            std::cout<< "Hash type: "<<vm["hash"].as<std::string>()<<std::endl;
            hashType = hashTypeSelector[vm["hash"].as<std::string>()];
        }
        if (vm.count("block")){
            std::cout << "block size: " << vm["block"].as<size_t>() << std::endl;
            blockSize = vm["block"].as<size_t>();
        }
        if(vm.count("files")){
            fileList = vm["files"].as<FileList>();
        }

        Run(fileList,hashType,blockSize);

    }
    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}