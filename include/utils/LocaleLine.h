#ifndef FOPTRANSAPP_FINAL_LOCALELINE_H
#define FOPTRANSAPP_FINAL_LOCALELINE_H
#include <string>
#include <vector>

class LocaleLine
{
private:
    std::string m_hash;
    std::string m_convertedHash;
    std::vector<int> m_fields;
    std::string m_content;
    std::string m_convertedContent;

    //void convertContent();

public:
    LocaleLine();
    LocaleLine(std::string hash, std::string content, const std::vector<int>& fields);
    LocaleLine(std::string hash, std::string convertedHash, std::string content,
               std::string convertedContent,
               const std::vector<int>& fields
    );
    [[nodiscard]] const std::string& getHash() const { return m_hash; }
    [[nodiscard]] const std::string& getConvertedHash() const { return m_convertedHash; }
    [[nodiscard]] const std::string& getContent() const { return m_content; }
    [[nodiscard]] const std::string& getConvertedContent() const { return m_convertedContent; }
    [[nodiscard]] const std::vector<int>& getFields() const { return m_fields; }

    void setHash(const std::string& hash);
    void setConvertedHash(const std::string& convertedHash);
    void setContent(const std::string& content);
    void setConvertedContent(const std::string& convertedContent);
    void setFields(const std::vector<int>& fields);
};

#endif //FOPTRANSAPP_FINAL_LOCALELINE_H
