#pragma once
#include <boost/program_options.hpp>
#include <optional>

#include "common.h"

struct Options{
    FileList fileList;
    HashType hashType;
    size_t blockSize;
};

std::optional<Options> getProgrammOptions(int argc, const char *argv[]);