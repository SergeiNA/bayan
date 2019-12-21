#include "FileHandler.h"
#include "BlockHashComparator.h"
#include "AppOption.h"
#include "MainHandler.h"

namespace popt = boost::program_options;

int main(int argc, const char *argv[]){
    try {
        auto options = getProgrammOptions(argc, argv);
        if(!options.has_value())
            return 0;
        auto groupedFiles = groupFiles( options.value().fileList,
                                        options.value().hashType,
                                        options.value().blockSize);

        PrintFileNames(groupedFiles.first,groupedFiles.second);
    }
    catch(const popt::error& err){
        std::cerr << err.what() << std::endl;
        return 1;
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}