#include "files/FileManager.h"

#include <algorithm>
#include <cstring>

#include "files/LocPackFile.h"

using namespace std;

/**
 * @brief Removes potential differences present in .locpack files for comparison with .locpackbin.
 * Helper function to remove `"` surrounding the lines in the .locpack files.
 * @param s The string to sanitize.
 * @return The sanitized string with the removed `"`
 */
std::string sanitize(std::string s) {
   if (s.size() >= 2 && s.front() == '"' && s.back() == '"') {
      s = s.substr(1, s.size() - 2);
   }
   return s;
}

static inline void bytesToHexUpper(const uint8_t* bytes, size_t len, std::string& outHex) {
   static const char hexTable[] = "0123456789ABCDEF";
   outHex.resize(len * 2);
   for (size_t i = 0; i < len; ++i) {
      outHex[i * 2]     = hexTable[(bytes[i] >> 4) & 0x0F];
      outHex[i * 2 + 1] = hexTable[bytes[i] & 0x0F];
   }
}

/**
 * @brief Checks if a .locpack and .locpackbin file are the same and returns a list of discrepancies.
 *
 * Function that verifies all lines in both files by comparing all the lines and checking if everything is equal.
 * Goes over all the indexes in .locpack files and takes the hashes from .locpack to fetch lines in .locpackbin and compares
 * obtained content.
 *
 * @param locPackFile The .locpack file to check the validity for.
 * @param locPackBinFile The .locpackbin file to check the validity for.
 * @return A vector of integer, representing the indexes of erroneous entries.
 */
vector<int> verifyFiles(LocPackFile &locPackFile, LocPackBinFile &locPackBinFile, std::function<void(int)> onProgress)
{
   locPackFile.reload();
   locPackBinFile.reload();

   vector<int> errorList;

   vector<LocaleLine> csvEntries;
   try {
      csvEntries = locPackFile.parseLocPackComplete();
   } catch (...) {
      return errorList;
   }

   const size_t totalEntries = csvEntries.size();
   if (totalEntries == 0) return errorList;

   const unsigned int fieldCount = locPackFile.getFieldCount() - 2;

   const std::filesystem::path binPath = locPackBinFile.getPath();
   ifstream input(binPath, ios::binary | ios::ate);
   if (!input.is_open()) return errorList;

   const size_t binSize = input.tellg();
   vector<uint8_t> binBuffer(binSize);
   input.seekg(0, ios::beg);
   input.read(reinterpret_cast<char*>(binBuffer.data()), static_cast<long>(binSize));

   unordered_map<string, BlockInfo> binMap;
   binMap.reserve(totalEntries);

   size_t offset = 0;
   const size_t minHeaderSize = 16 + (fieldCount * 4) + 2;

   while (offset + minHeaderSize <= binSize) {
      const size_t entryStart = offset;

      uint8_t rawHash[16];
      memcpy(rawHash, &binBuffer[offset], 16);
      offset += 16;

      for (size_t chunk = 0; chunk < 16; chunk += 8) {
         reverse(rawHash + chunk, rawHash + chunk + 8);
      }

      string hexHash;
      bytesToHexUpper(rawHash, 16, hexHash);

      vector<int> fields(fieldCount);
      for (unsigned int j = 0; j < fieldCount; ++j) {
         int32_t val = 0;
         memcpy(&val, &binBuffer[offset], 4);
         fields[j] = val;
         offset += 4;
      }

      uint16_t textLen = 0;
      std::memcpy(&textLen, &binBuffer[offset], 2);
      offset += 2;

      if (offset + textLen > binSize) break;

      string text(reinterpret_cast<const char*>(&binBuffer[offset]), textLen);
      offset += textLen;

      binMap.emplace(std::move(hexHash), BlockInfo(static_cast<int>(entryStart), textLen, std::move(fields), std::move(text)));
   }

   for (size_t i = 0; i < totalEntries; ++i)
   {
      const int entryIndex = static_cast<int>(i + 2);

      if (onProgress && (i % 50 == 0 || i == totalEntries - 1))
      {
         onProgress(entryIndex);
      }

      const LocaleLine& locPackEntry = csvEntries[i];
      string hash = locPackEntry.getHash();
      ranges::transform(hash, hash.begin(), ::toupper);

      if (hash.length() < 32) continue;

      auto it = binMap.find(hash);
      if (it == binMap.end()) {
         cout << "WARNING: Hash " << hash << " (Index " << entryIndex << ") not found in binary file!\n";
         errorList.push_back(entryIndex);
         continue;
      }

      const BlockInfo& locPackBinEntry = it->second;

      string csvContent = sanitize(locPackEntry.getContent());
      string binContent = sanitize(locPackBinEntry.m_text);

      bool hasError = false;

      if (csvContent != binContent)
      {
         cout << "--------- WARNING: Content mismatch for hash " << hash << " (Index " << entryIndex << ") ---------\n";
         cout << "CSV version: [" << csvContent << "]\n";
         cout << "BIN version: [" << binContent << "]\n";

         if (csvContent.length() != binContent.length()) {
            cout << "Size mismatch: CSV is " << csvContent.length() << " chars, BIN is " << binContent.length() << " chars.\n";
         }
         hasError = true;
      }

      const auto& lpFields = locPackEntry.getFields();
      const auto& binFields = locPackBinEntry.m_fields;

      for (unsigned int j = 0; j < fieldCount; j++)
      {
         int lpVal = (j < lpFields.size()) ? lpFields[j] : -1;
         int binVal = (j < binFields.size()) ? binFields[j] : -1;

         if (lpVal != binVal)
         {
            cout << "WARNING: Field discrepancy at index " << entryIndex << " field " << j << "\n";
            hasError = true;
         }
      }

      if (hasError) {
         errorList.push_back(entryIndex);
      }
   }

   return errorList;
}