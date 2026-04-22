/**
 * @file LocPackFile.cpp
 * @brief Implementation of the LocPackFile class that manages .locpack files.
 */
#include "files/LocPackFile.h"

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
LocPackFile::LocPackFile(const std::filesystem::path& path)
{
    locPackFilePath = path;
}

// Getters and setters
filesystem::path LocPackFile::getPath() const
{
    return locPackFilePath;
}

void LocPackFile::setPath(const filesystem::path& path)
{
    locPackFilePath = path;
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

void LocPackFile::rebuildCache()
{
    hashCache.clear();
    if (!document) return;

    const size_t rowCount = document->GetRowCount();
    hashCache.reserve(rowCount);

    for (int i = 0; i < rowCount; i++)
    {
        string hash = document->GetCell<string>(0, i);

        ranges::transform(hash, hash.begin(), ::toupper);

        hashCache[hash] = i;
    }
}

/**
 * @brief Loads the file saved in the locPackFilePath variable
 *
 * This function loads the file saved in the locPackFilePath variable
 * and returns a boolean true if that succeeded, false if not.
 * The file needs to exist and to have a .locpack extension on order to be valid.
 *
 * @return Returns a boolean with value `true` if all conditions are met, `false` otherwise.
 */
bool LocPackFile::load()
{
    // Test conditions to indicate whether the path exists or not.
    if (!filesystem::exists(locPackFilePath)) return false;
    if (filesystem::is_directory(locPackFilePath)) return false;
    if (locPackFilePath.extension() != ".locpack") return false;

    // Create the CSV Parser
    auto doc = make_unique<rapidcsv::Document>(
        locPackFilePath.string(),
        rapidcsv::LabelParams(-1, -1),
        rapidcsv::SeparatorParams(',', false, true, true, false)
    );

    // Save the document to the class
    document = std::move(doc);
    rebuildCache();

    // Set the last load time to the time the file was last edited
    lastLoadTime = filesystem::last_write_time(locPackFilePath);
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
    if (!filesystem::exists(locPackFilePath)) return false;

    // Create the currentModTime(currently last modification time of the file) and compare it with the last time the file was loaded in this program.
    if (const auto currentModTime = filesystem::last_write_time(locPackFilePath);
        currentModTime > lastLoadTime)
    {
        printf("Realoding the file...");

        return load();
    }

    return true;
}

/**
 * @brief Takes the file and loads every line into the program
 *
 * This function uses the `document` variable, tries to reload it,
 * and loads the whole content into a `std::vector` object of `LocaleLine`
 *
 * It reserves the memory needed for all the lines.
 *
 * @throws This throws a runtime error if the reloading of the file fails. See `LocPackFile::load()` and `LocPackFile::reload()` for more information about loading
 *
 * @return Returns the vector with all the lines from the document in it.
 */
vector<LocaleLine> LocPackFile::parseLocPackComplete()
{
    if (!reload()) throw runtime_error("Reloading file path at '" + locPackFilePath.string() + "' failed.");

    // Create and reserve vector space.
    vector<LocaleLine> lines;
    const size_t lineCount = document->GetRowCount();
    lines.reserve(lineCount);

    // Read all the lines
    for (int i = 0; i < lineCount; i++)
    {
        // Get the string at i
        vector<string> readStrings = document->GetRow<string>(i);

        if (readStrings.size() < 4) continue;

        // Remove the \r from the string.
        convertReadContent(readStrings[3]);

        lines.emplace_back(
            readStrings[0],
            readStrings[3],
            stoi(readStrings[1]),
            stoi(readStrings[2])
        );
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
    if (!reload()) throw runtime_error("Reloading file path at '" + locPackFilePath.string() + "' failed.");

    // Make sure the hash has uppercase letters
    string formattedHash = hash;
    ranges::transform(formattedHash, formattedHash.begin(), ::toupper);

    if (auto it = hashCache.find(formattedHash); it != hashCache.end())
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
    }

    vector<string> readLine = document->GetRow<string>(foundIndex);

    // Remove the \r from the string.
    convertReadContent(readLine[3]);

    return LocaleLine{readLine[0], readLine[3], stoi(readLine[1]), stoi(readLine[2])};
}

void LocPackFile::writeEntry(const std::string& hash, int character, int unknown, const std::string& content, bool overwrite)
{
    // TODO: Fill out stub!!!
}
