/**
 * @file LocPackFile.cpp
 * @brief Implementation of the LocPackFile class that manages .locpack files.
 */
#include "files/LocPackFile.h"

#include <utility>
#define HEADER_ROW_COUNT 2

using namespace std;

// Use the already defined classes and methods in the old backend project.
// TODO: Write in the .locpack file

/**
 * @brief Standard constructor.
 */
LocPackFile::LocPackFile() = default;

/**
 * @brief Constructor with path to .locpack file.
 *
 * This constructor takes a path that refers to a .locpack file.
 *
 * @param path A path to a .locpack file.
 */
LocPackFile::LocPackFile(std::filesystem::path path) : m_locPackFilePath(std::move(path))
{
}

// Getters and setters
filesystem::path LocPackFile::getPath() const
{
    return m_locPackFilePath;
}

void LocPackFile::setPath(const filesystem::path& path)
{
    m_locPackFilePath = path;
}

/**
 * @brief Helper to remove '\r' from strings.
 *
 * This function is a helper to remove the '\r' exit code from strings.
 *
 * @attention Modifies the content in-place!
 *
 * @param content The content to modify.
 */
void convertReadContent(string& content)
{
    content.erase(
        ranges::remove(content, '\r').begin(),
        content.end()
    );
}

/**
 * @brief Rebuilds the Hash-Cache
 *
 * Rebuilds the Hash-Cache containing the different hashes corresponding to the different lines in a given file.<br>
 * Saves the results in `LocPackFile::m_hashCache` as an unordered_map<std::string, int> with the string being the hash and the int being the line number.<br>
 * <br>
 * Starts counting on line 2 to avoid the header common to all .locpack files.
 */
void LocPackFile::rebuildCache()
{
    m_hashCache.clear();
    if (!m_document) return;

    const size_t rowCount = m_document->GetRowCount();
    m_hashCache.reserve(rowCount);

    for (int i = HEADER_ROW_COUNT; i < rowCount; i++)
    {
        auto hash = m_document->GetCell<string>(0, i);

        ranges::transform(hash, hash.begin(), ::toupper);

        m_hashCache[hash] = i;
    }
}

/**
 * @brief Loads the file saved in the m_locPackFilePath variable
 *
 * This function loads the file saved in the m_locPackFilePath variable
 * and returns a boolean true if that succeeded, false if not.
 * The file needs to exist and to have a .locpack extension on order to be valid.
 *
 * @return Returns a boolean with value `true` if all conditions are met, `false` otherwise.
 */
bool LocPackFile::load()
{
    // Test conditions to indicate whether the path exists or not.
    if (!filesystem::exists(m_locPackFilePath)) return false;
    if (filesystem::is_directory(m_locPackFilePath)) return false;
    if (m_locPackFilePath.extension() != ".locpack") return false;

    // Create the CSV Parser
    auto doc = make_unique<rapidcsv::Document>(
        m_locPackFilePath.string(),
        rapidcsv::LabelParams(-1, -1),
        rapidcsv::SeparatorParams(',', false, true, true, false)
    );

    // Get the number of total fields
    m_fieldNumber = doc->GetColumnCount();

    // Save the m_document to the class
    m_document = std::move(doc);
    rebuildCache();

    // Set the last load time to the time the file was last edited
    m_lastLoadTime = filesystem::last_write_time(m_locPackFilePath);
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
bool LocPackFile::reload()
{
    if (!filesystem::exists(m_locPackFilePath)) return false;

    // Create the currentModTime(currently last modification time of the file) and compare it with the last time the file was loaded in this program.
    if (const auto currentModTime = filesystem::last_write_time(m_locPackFilePath);
        currentModTime > m_lastLoadTime)
    {
        printf("Reloading the file...");

        return load();
    }

    return true;
}

/**
 * @brief Takes the file and loads every line into the program
 *
 * This function uses the `m_document` variable, tries to reload it,
 * and loads the whole content into a `std::vector` object of `LocaleLine`
 *
 * It reserves the memory needed for all the lines.
 *
 * @throws This throws a runtime error if the reloading of the file fails. See `LocPackFile::load()` and `LocPackFile::reload()` for more information about loading
 *
 * @return Returns the vector with all the lines from the m_document in it.
 */
vector<LocaleLine> LocPackFile::parseLocPackComplete()
{
    if (!reload()) throw runtime_error("Reloading file path at '" + m_locPackFilePath.string() + "' failed.");

    // Create and reserve vector space.
    vector<LocaleLine> lines;
    const size_t lineCount = m_document->GetRowCount();
    lines.reserve(lineCount);

    // Read all the lines
    for (int i = HEADER_ROW_COUNT; i < lineCount; i++)
    {
        // Get the string at i
        vector<string> readStrings = m_document->GetRow<string>(i);

        if (readStrings.size() < 4) continue;

        // Remove the \r from the string.
        convertReadContent(readStrings[3]);

        vector<int> fields;
        fields.reserve(m_fieldNumber - 2);

        for (int j = 1; j < m_fieldNumber - 1; j++)
        {
            fields.emplace_back(stoi(readStrings[j]));
        }

        lines.emplace_back(readStrings[0], readStrings[m_fieldNumber - 1], fields);
    }

    return lines;
}

/**
 * @brief Finds the Index of an index in a .locpack file.
 *
 * Finds the index of a lien based off of its hash.
 *
 * @throws This function throws a runtime error if the reloading of the file fails. See `LocPackFile::load()` and `LocPackFile::reload()` for more information about loading
 *
 * @param hash The hash in .lockpack version(Big Endian) as a string. No '0x' prefix
 * @return The 0-based index of a line as an int. -1 if line is not found.
 */
int LocPackFile::findHashIndex(const std::string& hash)
{
    if (!reload()) throw runtime_error("Reloading file path at '" + m_locPackFilePath.string() + "' failed.");

    // Make sure the hash has uppercase letters
    string formattedHash = hash;
    ranges::transform(formattedHash, formattedHash.begin(), ::toupper);

    if (const auto it = m_hashCache.find(formattedHash); it != m_hashCache.end())
    {
        return it->second;
    }

    return -1;
}

/**
 * @brief Finds a line by its hash in a .locpack file.
 *
 * @throws This function uses the `LocPackFile::findHashIndex()` function and therefor throws an exception if the reload of a file fails. <br>See `LocPackFile::load()` and `LocPackFile::reload()`.
 *
 * @param hash The hash in .lockpack version(Big Endian) as a string. No '0x' prefix.
 * @return A `LocaleLine` object with the information of the read line.
 */
LocaleLine LocPackFile::findFromHash(const std::string& hash)
{
    const int foundIndex = findHashIndex(hash);
    if (foundIndex == -1)
    {
        printf("Hash %s not found. Try again\n", hash.c_str());

        // TODO: Call the prompt function again here
        return LocaleLine{};
    }

    const vector<string> readLine = m_document->GetRow<string>(foundIndex);

    if (readLine.size() < static_cast<size_t>(m_fieldNumber))
    {
        return LocaleLine{};
    }

    string content = readLine[m_fieldNumber - 1];
    convertReadContent(content);

    vector<int> fields;
    fields.reserve(m_fieldNumber - 2);

    for (int j = 1; j < m_fieldNumber - 1; j++)
    {
        try
        {
            fields.emplace_back(stoi(readLine[j]));
        }
        catch (...)
        {
            fields.emplace_back(0);
        }
    }

    return LocaleLine{readLine[0], content, fields};
}

void LocPackFile::writeEntry(const std::string& hash, int character, int unknown, const std::string& content,
                             bool overwrite)
{
    // TODO: Fill out stub!!!
}
