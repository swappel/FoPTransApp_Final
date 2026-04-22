#include "utils/LocaleLine.h"

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
 * @param hash The hash in Big Endian, as a string. No '0x' prefix
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
 * @param hash
 * @param convertedHash
 * @param content
 * @param convertedContent
 * @param character
 * @param unknown
 */
LocaleLine::LocaleLine(const std::string& hash, const std::string& convertedHash, const std::string& content, const std::string &convertedContent, int character, int unknown)
{

}
