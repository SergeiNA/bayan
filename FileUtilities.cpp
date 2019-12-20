#include "FileUtilities.h"
#include <iostream>

void expandToFull(FileList& fList) {
    fs::path currentPath = fs::current_path();
    std::transform(fList.begin(), fList.end(), fList.begin(), 
        [currentPath](FileName file){
            return currentPath.string() + '/' + file;
        });
}

void Validate(FileList& fList) {
    fList.erase(
            std::remove_if(fList.begin(), fList.end(), [](const FileName& file){
                return !fs::exists(file);}),
            fList.end()
    );
}

void Unique(FileList& fList) {
    std::sort(fList.begin(), fList.end());
    fList.erase(std::unique(fList.begin(), fList.end()), fList.end());
}

void ValidateFileNames(FileList& fList){
    expandToFull(fList);
    Validate(fList);
    Unique(fList);
}


FileList RecursiveFileList(const std::string& path_name) {
    fs::path parent_path;
    if (!path_name.empty()) {
        if (!fs::exists(path_name)) return {};
        parent_path = fs::path(path_name);
    } else {
        parent_path = fs::current_path();
    }
    FileList fileList;
    try {
        for (const auto& currPath : fs::recursive_directory_iterator(parent_path)) {
            if (fs::is_regular_file(currPath.path())) {
                fileList.push_back(currPath.path().string());
            }
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << " " << fileList.back() << std::endl;
        return fileList;
    }
    return fileList;
}