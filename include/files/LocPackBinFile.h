#ifndef FOPTRANSAPP_FINAL_LOCPACKBINFILE_H
#define FOPTRANSAPP_FINAL_LOCPACKBINFILE_H
#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>
#include <array>

#include "LocPackFile.h"

struct BlockInfo
{
    int m_offset{};
    uint16_t m_length{};
    std::vector<int> m_fields;
    std::string m_text;

    BlockInfo() = default;
    explicit BlockInfo(const int& offset, const uint16_t& length, const std::vector<int>& fields,
                       std::string text);
};

typedef uint8_t byte_t;

class LocPackBinFile
{
private:
    std::filesystem::path m_filePath;
    mutable std::vector<byte_t> m_fileContent;
    mutable std::filesystem::file_time_type m_lastLoadTime;

    static std::array<uint8_t, 16> hashToBytes(const std::string& hash);

    /**
     * @brief Reads a Big Endian value from a byte buffer and returns it in host endianness.
     * @tparam T The integral type to return (e.g., int32_t, uint16_t).
     * @param buffer Pointer to the start of the bytes.
     */
    template <typename T>
    static T readBigEndian(const uint8_t* buffer)
    {
        T value = 0;
        for (size_t i = 0; i < sizeof(T); ++i)
        {
            value = (value << 8) | buffer[i];
        }
        return value;
    }

public:
    LocPackBinFile();
    explicit LocPackBinFile(const std::filesystem::path& path);

    bool load();
    bool readFile() const;
    [[nodiscard]] BlockInfo getTextByHash(const std::string& hash, const LocPackFile& locPackFile) const;

    void updateEntry(const std::string& hexHash, int val1, int val2, const std::string& newText) const;

    [[nodiscard]] const std::filesystem::path& getPath() { return m_filePath; }
    void setPath(const std::filesystem::path& path);

    static void flipEndianness(std::string& hex, size_t byteChunkSize = 8);
};

#endif //FOPTRANSAPP_FINAL_LOCPACKBINFILE_H
