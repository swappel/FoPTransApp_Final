#pragma once

#include <filesystem>
#include <string>
#include <vector>
#include "rapidcsv.h"
#include "utils/LocaleLine.h"

class LocPackFile
{
private:
    std::filesystem::path m_locPackFilePath;
    std::unique_ptr<rapidcsv::Document> m_document;
    std::filesystem::file_time_type m_lastLoadTime;

    std::unordered_map<std::string, int> m_hashCache;
    std::unordered_map<int, std::vector<std::string>> m_changeCache;

    unsigned int m_fieldNumber;

    void rebuildCache();
    static void convertReadContent(std::string& content);

public:
    LocPackFile();
    explicit LocPackFile(const std::filesystem::path& path);

    [[nodiscard]] const std::filesystem::path& getPath() const { return m_locPackFilePath; }
    [[nodiscard]] unsigned int getFieldCount() const { return m_fieldNumber; }
    [[nodiscard]] size_t getEntryCount() const { return m_document->GetRowCount(); }

    void setPath(const std::filesystem::path& path);

    bool load();
    bool reload();

    [[nodiscard]] std::vector<LocaleLine> parseLocPackComplete();
    [[nodiscard]] int findHashIndex(const std::string& hash);

    [[nodiscard]] LocaleLine findFromHash(const std::string& hash);
    [[nodiscard]] LocaleLine findFromIndex(const int index);

    void addChanges(const std::string& hash, const std::vector<int>& fields, const std::string& content);
    void writeEntry();
};
