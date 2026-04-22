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
 * @brief Loads the file saved in the locPackFilePath variable
 *
 * This function loads the file saved in the locPackFilePath variable
 * and returns a boolean true if that succeeded, false if not.
 * The file needs to exist and to have a .locpack extension on order to be valid.
 *
 * @return Returns `true` if all conditions are met, `false` otherwise.
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
 * @return Returns `true` if function ran without an error, `false` otherwise. 
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
        readStrings[3].erase(
            ranges::remove(readStrings[3], '\r').begin(),
            readStrings[3].end()
        );

        lines.emplace_back(
            readStrings[0],
            readStrings[3],
            stoi(readStrings[1]),
            stoi(readStrings[2])
        );
    }

    return lines;
}
