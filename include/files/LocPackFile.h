#ifndef FOPTRANSAPP_FINAL_LOCPACKFILE_H
#define FOPTRANSAPP_FINAL_LOCPACKFILE_H

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

    unsigned int m_fieldNumber;

    void rebuildCache();
    static void convertReadContent(std::string& content);

public:
    LocPackFile();
    explicit LocPackFile(std::filesystem::path path);

    [[nodiscard]] const std::filesystem::path& getPath() const { return m_locPackFilePath; }
    [[nodiscard]] const unsigned int& getFieldNumber() const { return m_fieldNumber; }

    void setPath(const std::filesystem::path& path);

    bool load();
    bool reload();

    // TODO: Decide if necessary, since not remote project anymore
    //[[nodiscard]] std::vector<LocaleLine> parseLocPackRange(int offset, int amount);

    [[nodiscard]] std::vector<LocaleLine> parseLocPackComplete();
    [[nodiscard]] int findHashIndex(const std::string& hash);

    [[nodiscard]] LocaleLine findFromHash(const std::string& hash);

    void writeEntry(const std::string& hash, int character, int unknown, const std::string& content, bool overwrite);
};

#endif //FOPTRANSAPP_FINAL_LOCPACKFILE_H
