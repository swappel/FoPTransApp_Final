#pragma once
#include "files/LocPackBinFile.h"
#include "files/LocPackFile.h"

class Backend
{
private:
    LocPackFile locpackFile;
    LocPackBinFile locpackbinFile;

public:
    void loadFiles(const std::string &locpackPath, const std::string &locpackbinPath);
    [[nodiscard]] unsigned int countLines() const;
    std::vector<int> verify(std::function<void(int)> onProgress = nullptr);
};
