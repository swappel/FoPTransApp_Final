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
    std::filesystem::path locPackFilePath;
    std::unique_ptr<rapidcsv::Document> document;
    std::filesystem::file_time_type lastLoadTime;

    std::unordered_map<std::string, int> hashCache;

    void rebuildCache();
public:
    LocPackFile();
    explicit LocPackFile(const std::filesystem::path& path);

    void setPath(const std::filesystem::path& path);
    [[nodiscard]] std::filesystem::path getPath() const;

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
