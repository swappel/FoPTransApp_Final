#ifndef FOPTRANSAPP_FINAL_LOCALELINE_H
#define FOPTRANSAPP_FINAL_LOCALELINE_H
#include <string>

class LocaleLine
{
    std::string m_hash;
    std::string M_convertedHash;
    int m_character;
    int m_unknown;
    std::string m_content;
    std::string m_convertedContent;

    void convertContent();
    void convertHash();

public:
    LocaleLine();
    LocaleLine(const std::string& hash, const std::string& content, int character, int unknown);
    LocaleLine(const std::string& hash, const std::string& convertedHash, const std::string& content, int character,
               int unknown);
    [[nodiscard]] std::string getHash() const;
    [[nodiscard]] std::string getConvertedHash() const;
    [[nodiscard]] std::string getContent() const;
    [[nodiscard]] std::string getConvertedContent() const;
    [[nodiscard]] int getCharacter() const;
    [[nodiscard]] int getUnknown() const;

    void setHash(const std::string& hash);
    void setConvertedHash(const std::string& convertedHash);
    void setContent(const std::string& content);
    void setConvertedContent(const std::string& convertedContent);
    void setCharacter(const int& character);
    void setUnknown(const int& unknown);
};

#endif //FOPTRANSAPP_FINAL_LOCALELINE_H
