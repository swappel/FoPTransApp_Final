#pragma once
#include "files/LocPackBinFile.h"
#include "files/LocPackFile.h"

struct SearchOptions
{
    std::string query;
    bool searchHash = true;
    bool searchContent = true;
    bool caseSensitive = false;
    bool exactMatch = false;
    int startIndex = 0;
    int count = 100;
};

struct SearchMatch
{
    int rowIndex = -1;
    std::string hash;
    std::vector<int> fields;
    std::string content;
};

class Backend
{
private:
    LocPackFile locpackFile;
    LocPackBinFile locpackbinFile;

    std::vector<LocaleLine> searchableLines;

    static std::string normalizeForSearch(const std::string& value, bool caseSensitive);
    static bool matchesQuery(const std::string& candidate, const std::string& query, bool exactMatch, bool caseSensitive);
public:
    void loadFiles(const std::string &locpackPath, const std::string &locpackbinPath);
    [[nodiscard]] unsigned int countLines() const;
    std::vector<int> verify(std::function<void(int)> onProgress = nullptr);
    std::vector<LocaleLine> getLinesRange(int startIndex, int count);
    void saveChangeToCache(const LocaleLine &newLine);
    void saveToFile(const std::string& newPathString);
    std::vector<SearchMatch> searchLines(const SearchOptions& options);
};
