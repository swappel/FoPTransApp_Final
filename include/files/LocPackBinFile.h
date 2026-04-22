#ifndef FOPTRANSAPP_FINAL_LOCPACKBINFILE_H
#define FOPTRANSAPP_FINAL_LOCPACKBINFILE_H
#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

struct BlockInfo
{
    int offset{};
    uint16_t length{};
    std::string text;

    BlockInfo() = default;

    explicit BlockInfo(int size, uint16_t length, const std::string &text);
};

using byte = uint8_t;

class LocPackBinFile
{
private:
    std::filesystem::path filePath;
    mutable std::vector<byte> fileContent;
    mutable std::filesystem::file_time_type lastLoadTime;

public:
    LocPackBinFile();
    explicit LocPackBinFile(const std::filesystem::path &path);

    bool load(const std::filesystem::path &path);
    [[nodiscard]] std::string convertHash(const std::string &hash);
    [[nodiscard]] BlockInfo getTextByHash(const std::string &hash) const;

    void updateEntry(const std::string &hexHash, int val1, int val2, const std::string &newText) const;

    [[nodiscard]] std::filesystem::path getPath() const;
    void setPath(const std::filesystem::path &path);
};

#endif //FOPTRANSAPP_FINAL_LOCPACKBINFILE_H
