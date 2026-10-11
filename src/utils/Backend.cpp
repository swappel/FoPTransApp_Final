#include "../../include/utils/Backend.h"

#include <complex>

#include "files/FileManager.h"

namespace
{
    constexpr int HEADER_ROW_COUNT = 2;
}


void Backend::loadFiles(const std::string& locpackPath, const std::string& locpackbinPath)
{
    locpackFile = LocPackFile(locpackPath);
    locpackbinFile = LocPackBinFile(locpackbinPath);

    locpackFile.load();
    locpackbinFile.load();
}

unsigned int Backend::countLines() const
{
    try {
        return locpackFile.getEntryCount();
    } catch (...) {
        return 0;
    }
}

std::vector<int> Backend::verify(std::function<void(int)> onProgress)
{
    try {
        return verifyFiles(locpackFile, locpackbinFile, onProgress);
    } catch (const std::exception& e) {
        printf("Error during verification: %s\n", e.what());
        return {};
    } catch (...) {
        printf("Unknown native error during verification.\n");
        return {};
    }
}

std::vector<LocaleLine> Backend::getLinesRange(int startIndex, int count)
{
    std::vector<LocaleLine> result;
    int total = static_cast<int>(locpackFile.getEntryCount());
    if (startIndex < 2) startIndex = 2;

    int endIndex = std::min(startIndex + count, total);
    result.reserve(endIndex - startIndex);

    for (int i = startIndex; i < endIndex; ++i) {
        result.push_back(locpackFile.findFromIndex(i));
    }
    return result;
}

void Backend::saveChangeToCache(const LocaleLine& newLine)
{

    if (locpackbinFile.getTextByHash(newLine.getHash(), locpackFile).m_offset == -1 || locpackFile.findHashIndex(newLine.getHash()) == -1)
    {
        throw std::runtime_error("Files have failed to load or hash " + newLine.getHash() + " could not be found.");
    }

    LocaleLine originalLine = locpackFile.findFromHash(newLine.getHash());

    // TODO: Check if new data has same amount of fields.

    locpackFile.addChanges(newLine.getHash(), originalLine.getFields(), newLine.getContent());

    locpackbinFile.applyEntryUpdate(newLine.getHash(), originalLine.getFields(), newLine.getContent());
}

void Backend::saveToFile(const std::string& newPathString)
{
    locpackFile.writeEntry(newPathString);
    locpackbinFile.save(newPathString + "bin");
}

std::string Backend::normalizeForSearch(const std::string& value, bool caseSensitive)
{
    if (value.empty())
    {
        return {};
    }

    std::string normalized = value;

    if (!caseSensitive)
    {
        std::transform(
            normalized.begin(),
            normalized.end(),
            normalized.begin(),
            [](unsigned char ch)
            {
                return static_cast<char>(std::tolower(ch));
            }
        );
    }

    return normalized;
}

bool Backend::matchesQuery(const std::string& candidate, const std::string& query, bool exactMatch, bool caseSensitive)
{
    if (query.empty())
    {
        return false;
    }

    const std::string normalizedCandidate = normalizeForSearch(candidate, caseSensitive);
    const std::string normalizedQuery = normalizeForSearch(query,caseSensitive);

    if (exactMatch)
    {
        return normalizedCandidate == normalizedQuery;
    }

    return normalizedCandidate.find(normalizedQuery) != std::string::npos;
}

std::vector<SearchMatch> Backend::searchLines(const SearchOptions& options)
{
    std::vector<SearchMatch> allMatches;
    std::vector<SearchMatch> pageMatches;

    if (searchableLines.empty())
    {
        return {};
    }

    if (options.query.empty())
    {
        return {};
    }

    const std::string normalizedQuery = normalizeForSearch(options.query, options.caseSensitive);

    allMatches.reserve(searchableLines.size());

    for (size_t i = 0; i < searchableLines.size(); i++)
    {
        const LocaleLine& line = searchableLines[i];
        const int rowIndex = static_cast<int>(i + HEADER_ROW_COUNT);

        const bool hashMatches = options.searchHash && matchesQuery(
            line.getHash(),
            normalizedQuery,
            options.exactMatch,
            options.caseSensitive
        );

        const bool contentMatch = options.searchContent && matchesQuery(
            line.getContent(),
            normalizedQuery,
            options.exactMatch,
            options.caseSensitive
        );

        if (!hashMatches && !contentMatch)
        {
            continue;
        }

        SearchMatch match;
        match.rowIndex = rowIndex;
        match.hash = line.getHash();
        match.fields = line.getFields();
        match.content = line.getContent();

        allMatches.push_back(match);
    }

    const int startIndex = std::max(0, options.startIndex);
    const int count = std::max(0, options.count);

    if (startIndex >= static_cast<int>(allMatches.size()))
    {
        return {};
    }

    const size_t begin = static_cast<size_t>(startIndex);
    const size_t end = count > 0
        ? std::min<size_t>(allMatches.size(), begin + static_cast<size_t>(count))
        : allMatches.size();

    pageMatches.assign(
        allMatches.begin() + static_cast<ptrdiff_t>(begin),
        allMatches.begin() + static_cast<ptrdiff_t>(end)
    );

    return pageMatches;
}