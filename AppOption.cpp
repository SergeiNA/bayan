#include <iostream>
#include "AppOption.h"
#include "Hash.h"
#include "FileUtilities.h"

namespace popt = boost::program_options;
std::optional<Options> getProgrammOptions(int argc, const char *argv[]){
    Options options;
    popt::options_description desc{"Options"};
    desc.add_options()
        ("help,h", "display help")
        ("files,F", popt::value<std::vector<std::string>>()->multitoken()->
                zero_tokens()->composing(), "files to compare, able to use without command")
        ("dir,D", popt::value<std::string>(),
        "Set searching directory.\nIf no directory or files were selected set dir as current dir")
        ("hash,H", popt::value<std::string>()->default_value("boost"), 
        "hash func for comparation: crc/boost/sha1")
        ("block,B", popt::value<size_t>()->default_value(10), "block size (bytes)");

    popt::positional_options_description pos_descr;
    pos_descr.add("files", -1);

    popt::command_line_parser parser{argc, argv};
    parser.options(desc).positional(pos_descr).allow_unregistered();
    popt::parsed_options parsed_options = parser.run();

    popt::variables_map vm;
    store(parsed_options, vm);
    notify(vm);

    if (vm.count("help")){
        std::cout << desc << '\n';
        return std::nullopt;
    }
    if (vm.count("hash")){
        std::cout << "Hash type: " << vm["hash"].as<std::string>() << std::endl;
        options.hashType = hashTypeSelector[vm["hash"].as<std::string>()];
    }
    if (vm.count("block")){
        std::cout << "block size: " << vm["block"].as<size_t>() << std::endl;
        options.blockSize = vm["block"].as<size_t>();
    }
    if (vm.count("dir"))
        options.fileList = RecursiveFileList(vm["dir"].as<std::string>());

    else if (vm.count("files")){
        options.fileList = vm["files"].as<FileList>();
        ValidateFileNames(options.fileList);
    }
    else
        options.fileList = RecursiveFileList("");
    return options;
}