#pragma once

#include <filesystem>
#include <functional>

#include "files/LocPackBinFile.h"
#include "files/LocPackFile.h"

std::vector<int> verifyFiles(
    LocPackFile &locPackFile,
    LocPackBinFile &locPackBinFile,
    std::function<void(int)> onProgress = nullptr
);

void readFiles(const std::filesystem::path& locPackPath, const std::filesystem::path& locPackBinPath);
void writeFiles(LocPackFile& locPackFile, LocPackBinFile& locPackBinFile);