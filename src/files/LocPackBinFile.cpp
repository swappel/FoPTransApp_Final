/**
 * @file LocPackBinFile.cpp
 * @brief The file containing the logic and class to manage .locpackbin files.
 *
 * The file containing the logic and class to manage .locpackbin files.
 */

#include "files/LocPackBinFile.h"

#include <utility>

#include "rapidcsv.h"

#define HASH_WIDTH_BYTES 16
#define NUMBER_WIDTH_BYTES 4
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

BlockInfo::BlockInfo(const int size, const uint16_t length, std::string text) :
    m_offset(size), m_length(length), m_text(std::move(text))
{
}

bool LocPackBinFile::load() const
{
    bool testsPassed = true;

    // Test conditions to indicate whether the path exists or not.
    if (!filesystem::exists(m_filePath)) testsPassed = false;
    if (filesystem::is_directory(m_filePath)) testsPassed = false;
    if (m_filePath.extension() != ".locpack") testsPassed = false;

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

std::filesystem::path LocPackBinFile::getPath() const
{
    return m_filePath;
}

void LocPackBinFile::setPath(const std::filesystem::path& path)
{
    m_filePath = path;
}

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

BlockInfo LocPackBinFile::getTextByHash(const std::string& hash) const
{
    array<uint8_t, 16> hashBytes = hashToBytes(hash);

    auto searchRange = ranges::search(m_fileContent, hashBytes);

    if (searchRange.empty())
    {
        printf("Hash not found: %s", hash.c_str());
        return {};
    }

    if (const size_t findIndex = distance(m_fileContent.begin(), searchRange.begin()))
    {
    }
}
