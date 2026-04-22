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

public:
    LocPackFile();
    explicit LocPackFile(const std::filesystem::path& path);

    void setPath(const std::filesystem::path& path);
    [[nodiscard]] std::filesystem::path getPath() const;

    [[nodiscard]] std::vector<LocaleLine> parseLocPackRange(int offset, int amount) const;
    [[nodiscard]] std::vector<LocaleLine> parseLocPackComplete() const;
    [[nodiscard]] int findHashIndex(const std::string &hash) const;

    [[nodiscard]] LocaleLine findFromHash(const std::string &hash) const;

    void writeEntry(const std::string &hash, int character, int unknown, const std::string &content);
};

#endif //FOPTRANSAPP_FINAL_LOCPACKFILE_H
