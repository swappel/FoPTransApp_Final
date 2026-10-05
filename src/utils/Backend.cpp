#include "../../include/utils/Backend.h"

#include "files/FileManager.h"

void Backend::loadFiles(const std::string& locpackPath, const std::string& locpackbinPath)
{
    locpackFile = LocPackFile(locpackPath);
    locpackbinFile = LocPackBinFile(locpackbinPath);
}

unsigned int Backend::countLines() const
{
    return locpackFile.getEntryCount();
}

std::vector<int> Backend::verify(std::function<void(int)> onProgress)
{
    return verifyFiles(locpackFile, locpackbinFile, onProgress);
}
