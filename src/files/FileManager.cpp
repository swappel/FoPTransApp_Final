#include "files/FileManager.h"

#include <algorithm>

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

   const unsigned int fieldCount = locPackFile.getFieldCount() - 2;

   for (auto i = 0; i < locPackFile.getEntryCount(); i++)
   {
      if (onProgress && (i % 50 == 0 || i == fieldCount - 1))
      {
         onProgress(static_cast<int>(i + 1));
      }

      LocaleLine locPackEntry = locPackFile.findFromIndex(i);
      const string& hash = locPackEntry.getHash();

      if (hash.length() < 32) continue;

      BlockInfo locPackBinEntry = locPackBinFile.getTextByHash(hash, locPackFile);

      if (locPackBinEntry.m_offset == -1) {
         cout << "WARNING: Hash " << hash << " (Index " << i << ") not found in binary file!" << endl;
         errorList.push_back(i);
         continue;
      }

      std::string csvContent = sanitize(locPackEntry.getContent());
      std::string binContent = sanitize(locPackBinEntry.m_text);

      if (csvContent != binContent)
      {
         cout << "--------- WARNING: Content mismatch for hash " << hash << " (Index " << i << ") ---------" << endl;
         cout << "CSV version: [" << csvContent << "]" << endl;
         cout << "BIN version: [" << binContent << "]" << endl;

         if (csvContent.length() != binContent.length()) {
            cout << "Size mismatch: CSV is " << csvContent.length() << " chars, BIN is " << binContent.length() << " chars." << endl;
         }

         errorList.push_back(i);
      }

      const auto& lpFields = locPackEntry.getFields();
      const auto& binFields = locPackBinEntry.m_fields;

      for (unsigned int j = 0; j < fieldCount; j++)
      {
         int lpVal = (j < lpFields.size()) ? lpFields[j] : -1;
         int binVal = (j < binFields.size()) ? binFields[j] : -1;

         if (lpVal != binVal)
         {
            cout << "WARNING: Field discrepancy at index " << i << " field " << j << endl;
            if (std::find(errorList.begin(), errorList.end(), i) == errorList.end())
            {
               errorList.push_back(i);
            }
         }
      }
   }

   return errorList;
}