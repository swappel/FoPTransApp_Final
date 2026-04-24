#ifndef FOPTRANSAPP_FINAL_LOCPACKBINFILE_H
#define FOPTRANSAPP_FINAL_LOCPACKBINFILE_H
#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>
#include <array>

struct BlockInfo
{
    int m_offset{};
    int m_fieldCount{};
    uint16_t m_length{};
    std::string m_text;

    BlockInfo() = default;
    explicit BlockInfo(int offset, int fieldCount, uint16_t length, std::string text);
};

using byte = uint8_t;

class LocPackBinFile
{
private:
    std::filesystem::path m_filePath;
    mutable std::vector<byte> m_fileContent;
    mutable std::filesystem::file_time_type m_lastLoadTime;

    static std::array<uint8_t, 16> hashToBytes(const std::string& hash);

public:
    LocPackBinFile();
    explicit LocPackBinFile(const std::filesystem::path& path);

    bool load() const;
    bool readFile() const;
    [[nodiscard]] BlockInfo getTextByHash(const std::string& hash) const;

    void updateEntry(const std::string& hexHash, int val1, int val2, const std::string& newText) const;

    [[nodiscard]] const std::filesystem::path& getPath() { return m_filePath; }
    void setPath(const std::filesystem::path& path);

    static void flipEndianness(std::string& hex, size_t byteChunkSize = 8);
};

#endif //FOPTRANSAPP_FINAL_LOCPACKBINFILE_H
