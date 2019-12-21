#pragma once

#include <iostream>
#include "Common.h"

void PrintFileNames( const FileList& uniqueFiles,
                    const std::vector<FileSet>& groupDubs, 
                    std::ostream& os =std::cout);

std::pair<FileList,std::vector<FileSet>> 
groupFiles(FileList files_, HashType hashType, size_t blockSize);