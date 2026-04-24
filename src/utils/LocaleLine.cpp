#include "utils/LocaleLine.h"

#include <utility>

using namespace std;

/**
 * @brief Default constructor without arguments.
 *
 * Default constructor without arguments.
 */
LocaleLine::LocaleLine() = default;

/**
 * @brief Constructor with all parameters for lines in the localization files.
 *
 * A constructor with all the parameters needed to save a line from the .locpack files.
 *
 * @param hash The hash in Big Endian, as a string. No '0x' prefix.
 * @param content The content of a given line. UTF-8
 * @param fields A vector of integers representing the fields in the file(excluding hash and text)
 */
LocaleLine::LocaleLine(std::string hash, std::string content, const vector<int>& fields) :
    m_hash(std::move(hash)), m_fields(fields), m_content(std::move(content))
{
}

/**
 * @brief A complete constructor featuring the converted versions of hash and content
 *
 * A constructor with all the parameter of the LocaleLine class, including the convert variants.
 *
 * @param hash The hash in Big Endian, as a string. No '0x' prefix.
 * @param convertedHash The hash in Little Endian, as a string. No '0x' prefix.
 * @param content The content of a given line. UTF-8
 * @param convertedContent The content of a given line. ASCII with UTF-8 encoded characters included.
 * @param fields A vector of integers representing the fields in the file(excluding hash and text)
 */
LocaleLine::LocaleLine(std::string hash, std::string convertedHash, std::string content,
                       std::string convertedContent, const vector<int>& fields) :
    m_hash(std::move(hash)), m_convertedHash(std::move(convertedHash)), m_fields(fields),
    m_content(std::move(content)), m_convertedContent(std::move(convertedContent))
{
}

// Setters:
void LocaleLine::setHash(const std::string& hash)
{
    m_hash = hash;
}

void LocaleLine::setConvertedHash(const std::string& convertedHash)
{
    m_convertedHash = convertedHash;
}

void LocaleLine::setContent(const std::string& content)
{
    m_content = content;
}

void LocaleLine::setConvertedContent(const std::string& convertedContent)
{
    m_convertedContent = convertedContent;
}

void LocaleLine::setFields(const std::vector<int>& fields)
{
    m_fields = fields;
}


