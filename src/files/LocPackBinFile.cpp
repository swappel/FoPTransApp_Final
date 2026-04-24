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

// Use the already defined classes and methods in the old backend project.

// TODO: Fetch the .locpackbin file
// TODO: Parse the .locpackbin file
// TODO: Write in the .locpackbin file

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
 * @param locPackFile
 *
 * @return `BlockInfo` object with the information of the line, including the integer fields.
 */
BlockInfo LocPackBinFile::getTextByHash(const std::string& hash, const LocPackFile& locPackFile) const
{
    // Fetch the length of the fields
    const unsigned int fieldCount = locPackFile.getFieldNumber();
    const unsigned int integerFieldCount = fieldCount - 2;

    // Prepare the hash for search
    string beHash = hash;
    flipEndianness(beHash);
    array<uint8_t, 16> hashBytes = hashToBytes(beHash);

    // Search hashBytes in fileContent vector
    auto searchRange = ranges::search(m_fileContent, hashBytes);
    if (searchRange.empty())
    {
        cerr << "Hash not found: " << hash << endl;
        return {};
    }

    const size_t startIndex = distance(m_fileContent.begin(), searchRange.begin());

    // Read the integer fields
    vector<int> fields;
    for (int i = 0; i < integerFieldCount; i++)
    {
        const size_t fieldOffset = startIndex + HASH_WIDTH_BYTES + (i * FIELD_WIDTH_BYTES);

        fields.emplace_back(readBigEndian<int>(&m_fileContent[fieldOffset]));
    }

    // Read the text length
    const size_t textLengthPosition = startIndex + HASH_WIDTH_BYTES + (integerFieldCount * FIELD_WIDTH_BYTES);
    if (textLengthPosition + LENGTH_WIDTH_BYTES > m_fileContent.size())
    {
        throw runtime_error("The given text length position is non contained in the loaded file content.");
    }

    const auto textLen = readBigEndian<uint16_t>(&m_fileContent[textLengthPosition]);

    // Read the text
    const size_t textOffset = textLengthPosition + LENGTH_WIDTH_BYTES;
    if (textOffset + textLen > m_fileContent.size())
    {
        throw runtime_error("File content is too short to contain requested data.");
    }

    string text;
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

    // Convert byte size to character size (2 chars per byte)
    const size_t charChunkSize = byteChunkSize * 2;

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
