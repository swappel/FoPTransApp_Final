#pragma once

#include <filesystem>
#include <string>

#include "files/LocPackBinFile.h"
#include "files/LocPackFile.h"

bool verifyFiles(const LocPackFile &locPackFile, const LocPackBinFile &locPackBinFile);

void readFiles(const std::filesystem::path& locPackPath, const std::filesystem::path& locPackBinPath);
void writeFiles(LocPackFile locPackFile, LocPackBinFile locPAckBinFile);