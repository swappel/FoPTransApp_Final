#include "../../include/utils/Backend.h"

#include "files/FileManager.h"

void Backend::loadFiles(const std::string& locpackPath, const std::string& locpackbinPath)
{
    locpackFile = LocPackFile(locpackPath);
    locpackbinFile = LocPackBinFile(locpackbinPath);

    locpackFile.load();
    locpackbinFile.load();
}

unsigned int Backend::countLines() const
{
    try {
        return locpackFile.getEntryCount();
    } catch (...) {
        return 0;
    }
}

std::vector<int> Backend::verify(std::function<void(int)> onProgress)
{
    try {
        return verifyFiles(locpackFile, locpackbinFile, onProgress);
    } catch (const std::exception& e) {
        printf("Error during verification: %s\n", e.what());
        return {};
    } catch (...) {
        printf("Unknown native error during verification.\n");
        return {};
    }
}

std::vector<LocaleLine> Backend::getLinesRange(int startIndex, int count)
{
    std::vector<LocaleLine> result;
    int total = static_cast<int>(locpackFile.getEntryCount());
    if (startIndex < 2) startIndex = 2;

    int endIndex = std::min(startIndex + count, total);
    result.reserve(endIndex - startIndex);

    for (int i = startIndex; i < endIndex; ++i) {
        result.push_back(locpackFile.findFromIndex(i));
    }
    return result;
}

void Backend::saveChangeToCache(const LocaleLine& newLine)
{
    bool success = true;

    if (locpackbinFile.getTextByHash(newLine.getHash(), locpackFile).m_offset == -1 || locpackFile.findHashIndex(newLine.getHash()) == -1)
    {
        throw std::runtime_error("Files have failed to load or hash " + newLine.getHash() + " could not be found.");
    }

    LocaleLine originalLine = LocaleLine(originalLine.getHash(), originalLine.getFields(), originalLine.getContent());

    // TODO: Check if new data has same amount of fields.

    locpackFile.addChanges(newLine.getHash(), originalLine.getFields(), newLine.getContent());

    locpackbinFile.applyEntryUpdate(newLine.getHash(), originalLine.getFields(), newLine.getContent());
}

void Backend::saveToFile()
{
    locpackFile.writeEntry();

    locpackbinFile.save();
}
