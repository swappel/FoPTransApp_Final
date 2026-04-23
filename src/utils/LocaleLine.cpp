#include "utils/LocaleLine.h"

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
 * @param character An integer corresponding to a character. Can be found in <SPECIFY FILE HERE>
 * @param unknown An integer of unknown use. I knew the use at some point, but I don't remember.
 */
LocaleLine::LocaleLine(const std::string& hash, const std::string& content, int character, int unknown)
{
    m_hash = hash;
    m_content = content;
    m_character = character;
    m_unknown = unknown;
}

/**
 * @brief A complete constructor featuring the converted versions of hash and content
 *
 * A constructor with all the parameter of the LocaleLine class, including the converte variants.
 *
 * @param hash The hash in Big Endian, as a string. No '0x' prefix.
 * @param convertedHash The hash in Little Endian, as a string. No '0x' prefix.
 * @param content The content of a given line. UTF-8
 * @param convertedContent The content of a given line. ASCII with UTF-8 encoded characters included.
 * @param character An integer corresponding to a character. Can be found in <SPECIFY FILE HERE>
 * @param unknown An integer of unknown use. I knew the use at some point, but I don't remember.
 */
LocaleLine::LocaleLine(const std::string& hash, const std::string& convertedHash, const std::string& content,
                       const std::string& convertedContent, int character, int unknown)
{
    m_hash = hash;
    m_convertedHash = convertedHash;
    m_content = content;
    m_convertedContent = convertedContent;
    m_character = character;
    m_unknown = unknown;
}

// Getters and Setters
std::string LocaleLine::getHash() const
{
    return m_hash;
}

std::string LocaleLine::getConvertedHash() const
{
    return m_convertedHash;
}

std::string LocaleLine::getContent() const
{
    return m_content;
}

std::string LocaleLine::getConvertedContent() const
{
    return m_convertedContent;
}

int LocaleLine::getCharacter() const
{
    return m_character;
}

int LocaleLine::getUnknown() const
{
    return m_unknown;
}

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

void LocaleLine::setCharacter(const int character)
{
    m_character = character;
}

void LocaleLine::setUnknown(const int unknown)
{
    m_unknown = unknown;
}

// Private function(s?). Have to see whether I add contentConvert function.

/**
 * @brief Converts a hash from Big to Little Endian
 *
 * Converts a hash from Big to Little Endian.<br>
 * Converts the hash stored in `LocaleLine::m_hash` tores the result in `LocaleLine::m_convertedHash`.<br>
 * If the hash is invalid(not 16 bytes long), nothing is stored. Function therefor fails silently.
 */
void LocaleLine::convertHash()
{
    if (m_hash.size() < 32) return;

    string temp = m_hash.substr(16);

    temp.append(m_hash.substr(17, 16));

    m_convertedHash = temp;
}
