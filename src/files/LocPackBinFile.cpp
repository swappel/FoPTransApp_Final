/**
 * @file LocPackBinFile.cpp
 * @brief The file containing the logic and class to manage .locpackbin files.
 *
 * The file containing the logic and class to manage .locpackbin files.
 */

#include "files/LocPackBinFile.h"

#include <cstring>
#include <utility>

#include "rapidcsv.h"
#include "files/LocPackFile.h"

#define HASH_WIDTH_BYTES 16
#define FIELD_WIDTH_BYTES 4
#define LENGTH_WIDTH_BYTES 2

using namespace std;

/**
 * @brief The default constructor with no parameters.
 */
LocPackBinFile::LocPackBinFile() = default;

/**
 * @brief The constructor for the LocPackBinFile class with a path argument
 * @param path The path of the .locpackbin file
 */
LocPackBinFile::LocPackBinFile(const std::filesystem::path& path)
{
    m_filePath = path;
}

/**
 * @brief The constructor for the `BlockInfo` struct.
 *
 * The constructor with all the parameters for the `BlockInfo` struct.
 *
 * @param offset The offset of a given block
 * @param length The length of a given block of information
 * @param fields
 * @param text The actual text contained in the .locpackbin file.
 */
BlockInfo::BlockInfo(const int& offset, const uint16_t& length, const std::vector<int>& fields, std::string text) :
    m_offset(offset), m_length(length), m_fields(fields), m_text(std::move(text))
{
}

/**
 * @brief This function loads a .locpackbin file into memory.
 *
 * This functions uses the path in the `LocPackBinFile::m_filePath` variable to load a given .locpackbin file into memory for the program to use.
 *
 * @return `true` if the file has successfully been loaded, `false` otherwise.
 */
bool LocPackBinFile::load()
{
    bool testsPassed = true;

    // Test conditions to indicate whether the path exists or not.
    if (!filesystem::exists(m_filePath)) testsPassed = false;
    if (filesystem::is_directory(m_filePath)) testsPassed = false;
    if (m_filePath.extension() != ".locpackbin") testsPassed = false;

    if (!testsPassed) throw runtime_error("The file at path '" + m_filePath.string() + "' could not be loaded.");

    try
    {
        return readFile();
    }
    catch (runtime_error& e)
    {
        printf("An error occurred while reading the file:\n %s\n", e.what());
    }

    // Set the last load time to the time the file was last edited
    m_lastLoadTime = filesystem::last_write_time(m_filePath);
    return true;
}

/**
 * @brief Reloads the file
 *
 * This function only reloads the file if said file has been edited.
 * This is checked by looking at the last write timestamp.
 *
 * @return Returns a boolean with value `true` if function ran without an error, `false` otherwise.
 */
bool LocPackBinFile::reload()
{
    if (!filesystem::exists(m_filePath)) return false;

    // Create the currentModTime(currently last modification time of the file) and compare it with the last time the file was loaded in this program.
    if (const auto currentModTime = filesystem::last_write_time(m_filePath);
        currentModTime > m_lastLoadTime)
    {
        printf("Reloading the .locpackbin file...");

        return load();
    }

    return true;
}

/**
 * @brief Reads the information from a .locpackbin file into memory.
 *
 * Mainly a helper function for the `LocPackBinFile::load()` function. <br>
 * Uses the `LocPackBinFile::m_filePath` variable to get the file and its contents and parse them to memory for use by the program.
 *
 * @return `true` if the reading and parsing was successful, `false` otherwise.
 */
bool LocPackBinFile::readFile() const
{
    ifstream input(m_filePath, ios::binary | ios::ate);

    if (!input.is_open())
    {
        throw runtime_error("Could not open file at path '" + m_filePath.string() + "'.");
    }

    const size_t fileSize = input.tellg();

    // Set the vector to the size of the file
    m_fileContent.resize(fileSize);
    // Got to the start of the file
    input.seekg(0, ios::beg);

    if (input.read(reinterpret_cast<char*>(m_fileContent.data()), static_cast<long>(fileSize)))
    {
        m_lastLoadTime = filesystem::last_write_time(m_filePath);
        return true;
    }
    return false;
}

void LocPackBinFile::setPath(const std::filesystem::path& path)
{
    m_filePath = path;
}

/**
 * @brief Converts a hash string to its big endian byte version.
 *
 * This function converts a hash from the .locpackbin file to an array of 16 bytes contaning the hash. <br>
 * Used to convert the hash from a .locpack... file to the hash version in a .locpack... file
 *
 * @param hash The hash in .lockpackbin version(Little Endian) as a string. No '0x' prefix.
 * @return An array of length 16 with the converted hash.
 */
std::array<uint8_t, 16> LocPackBinFile::hashToBytes(const std::string& hash)
{
    auto hexToVal = [](const char c) -> uint8_t
    {
        if (c >= '0' && c <= '9') return c - '0';
        if (c >= 'a' && c <= 'f') return c - 'a' + 10;
        if (c >= 'A' && c <= 'F') return c - 'A' + 10;

        return 0;
    };

    // Convert Hash String to bytes
    array<uint8_t, 16> hashBytes{};
    for (int i = 0; i < 16; i++)
    {
        hashBytes[i] = hexToVal(hash[i * 2]) << 4 | hexToVal(hash[i * 2 + 1]);
    }

    return hashBytes;
}

/**
 * @brief Fetches the text from a .locpackbin files by its hash string
 *
 * This function fetches a specific line of text from the .locpackbin file by its hash string.
 * It then returns a `BlockInfo` object with the information of the line, including the integer fields.
 *
 * @param hash A string of hex-bytes containing the hash of a specific line. Little Endian.
 * @param locPackFile A `LocPackFile` object.
 *
 * @return `BlockInfo` object with the information of the line, including the integer fields.
 */
BlockInfo LocPackBinFile::getTextByHash(const std::string& hash, const LocPackFile& locPackFile) const
{
    std::string flippedHex = hash;
    LocPackBinFile::flipEndianness(flippedHex, 8);

    std::vector<uint8_t> binaryHash;
    binaryHash.reserve(HASH_WIDTH_BYTES);
    for (size_t i = 0; i < flippedHex.length(); i += 2) {
        std::string byteString = flippedHex.substr(i, 2);
        auto byte = static_cast<uint8_t>(std::strtol(byteString.c_str(), nullptr, 16));
        binaryHash.push_back(byte);
    }

    const auto it = ranges::search(m_fileContent, binaryHash).begin();

    if (it == m_fileContent.end()) {
        return BlockInfo(-1, 0, {}, "HASH NOT FOUND");
    }

    const size_t startIndex = std::distance(m_fileContent.begin(), it);

    const unsigned int actualFieldCount = locPackFile.getFieldCount() - 2;
    std::vector<int> fields;
    fields.reserve(actualFieldCount);

    for (unsigned int i = 0; i < actualFieldCount; ++i) {
        const size_t fieldOffset = startIndex + HASH_WIDTH_BYTES + (i * FIELD_WIDTH_BYTES);

        if (fieldOffset + 4 > m_fileContent.size()) break;

        int32_t val = 0;
        std::memcpy(&val, &m_fileContent[fieldOffset], 4);
        fields.push_back(val);
    }

    const size_t textLengthPosition = startIndex + HASH_WIDTH_BYTES + (actualFieldCount * FIELD_WIDTH_BYTES);

    if (textLengthPosition + 2 > m_fileContent.size()) {
        return BlockInfo(-1, 0, {}, "ERR: FILE TRUNCATED");
    }

    uint16_t textLen = 0;
    std::memcpy(&textLen, &m_fileContent[textLengthPosition], 2);

    const size_t textOffset = textLengthPosition + LENGTH_WIDTH_BYTES;
    if (textOffset + textLen > m_fileContent.size()) {
        return BlockInfo(-1, 0, {}, "ERR: STRING OUT OF BOUNDS");
    }

    std::string text;
    text.assign(reinterpret_cast<const char*>(&m_fileContent[textOffset]), textLen);

    return BlockInfo(static_cast<int>(startIndex), textLen, fields, text);
}

/**
 * @brief Flips the endianness of a string of hex bytes.
 *
 * This function flips the endianness of a string of hex bytes.
 *
 * @param hex The string of hex bytes to flip the endianness of.
 * @param byteChunkSize The size of a word that gets flipped. Needs to be the exact size, not a maximum!
 * @attention Changes the hex string in-place. No return.
 */
void LocPackBinFile::flipEndianness(std::string& hex, const size_t byteChunkSize)
{
    if (hex.length() % 2 != 0) return;

    const size_t charChunkSize = byteChunkSize * 2;

    if (hex.length() < charChunkSize || hex.length() % charChunkSize != 0)
    {
        return;
    }

    std::string result;
    result.reserve(hex.length());

    for (size_t i = 0; i < hex.length(); i += charChunkSize)
    {
        std::string chunk = hex.substr(i, charChunkSize);

        // Flip bytes within this chunk
        for (int j = static_cast<int>(chunk.length()) - 2; j >= 0; j -= 2)
        {
            result += chunk.substr(j, 2);
        }
    }

    hex = result;
}

/**
 * @brief Applies an update to an entry inside m_fileContent in-memory.
 *        Does NOT write the changes to disk.
 *
 * @param hexHash The entry hash in hex string format (Little Endian).
 * @param val1 The first field integer value.
 * @param val2 The second field integer value.
 * @param newText The replacement text content.
 */
void LocPackBinFile::applyEntryUpdate(const std::string& hexHash, int val1, int val2, const std::string& newText) const
{
    std::string flippedHex = hexHash;
    LocPackBinFile::flipEndianness(flippedHex, 8);

    std::vector<uint8_t> binaryHash;
    binaryHash.reserve(HASH_WIDTH_BYTES);
    for (size_t i = 0; i < flippedHex.length(); i += 2)
    {
        std::string byteString = flippedHex.substr(i, 2);
        auto byte = static_cast<uint8_t>(std::strtol(byteString.c_str(), nullptr, 16));
        binaryHash.push_back(byte);
    }

    const auto it = std::ranges::search(m_fileContent, binaryHash).begin();
    if (it == m_fileContent.end())
    {
        throw std::runtime_error("Hash not found: " + hexHash);
    }

    const size_t startIndex = std::distance(m_fileContent.begin(), it);
    const size_t val1Offset = startIndex + HASH_WIDTH_BYTES;
    const size_t val2Offset = val1Offset + FIELD_WIDTH_BYTES;
    const size_t textLenOffset = val2Offset + FIELD_WIDTH_BYTES;

    if (textLenOffset + LENGTH_WIDTH_BYTES > m_fileContent.size())
    {
        throw std::runtime_error("File truncated at target entry.");
    }

    uint16_t oldTextLen = 0;
    std::memcpy(&oldTextLen, &m_fileContent[textLenOffset], LENGTH_WIDTH_BYTES);

    const size_t oldTextOffset = textLenOffset + LENGTH_WIDTH_BYTES;
    const size_t oldBlockEnd = oldTextOffset + oldTextLen;

    if (oldBlockEnd > m_fileContent.size())
    {
        throw std::runtime_error("Old string bounds exceed file size.");
    }

    std::memcpy(&m_fileContent[val1Offset], &val1, sizeof(int32_t));
    std::memcpy(&m_fileContent[val2Offset], &val2, sizeof(int32_t));

    uint16_t newTextLen = static_cast<uint16_t>(newText.length());
    std::memcpy(&m_fileContent[textLenOffset], &newTextLen, sizeof(uint16_t));

    const auto textBeginIt = m_fileContent.begin() + oldTextOffset;
    const auto textEndIt = m_fileContent.begin() + oldBlockEnd;

    auto insertedIt = m_fileContent.erase(textBeginIt, textEndIt);
    m_fileContent.insert(insertedIt, newText.begin(), newText.end());
}

/**
 * @brief Flushes m_fileContent to disk once after all updates are applied.
 *
 * @return true if successful, false otherwise.
 */
bool LocPackBinFile::save() const
{
    std::ofstream output(m_filePath, std::ios::binary | std::ios::trunc);
    if (!output.is_open())
    {
        return false;
    }

    output.write(reinterpret_cast<const char*>(m_fileContent.data()), m_fileContent.size());
    if (!output)
    {
        return false;
    }

    m_lastLoadTime = std::filesystem::last_write_time(m_filePath);
    return true;
}