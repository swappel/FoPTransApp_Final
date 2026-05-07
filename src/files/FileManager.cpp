#include "files/FileManager.h"

#include "files/LocPackFile.h"

using namespace std;

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
vector<int> verifyFiles(LocPackFile &locPackFile, LocPackBinFile &locPackBinFile)
{
   // TODO: Might have to implement check for size discrepancies between the two files(check if both have the same amount of entries).

   locPackFile.reload();
   locPackBinFile.reload();

   vector<int> errorList;
   const unsigned int fieldCount = locPackFile.getFieldCount();

   // Loop through all entries in the file to verify the validity
   for (auto i = 0; i < locPackFile.getEntryCount(); i++)
   {
      // Get .locpack entry
      LocaleLine locPackEntry = locPackFile.findFromIndex(i);

      // Get hash from .locpack file
      const string& hash = locPackEntry.getHash();

      // Get the .locpackbin entry
      BlockInfo locPackBinEntry = locPackBinFile.getTextByHash(hash, locPackFile);

      // Compare the entries
      if (locPackEntry.getContent() != locPackBinEntry.m_text)
      {
         cout << "WARNING: Ćontent of entry with hash \"" << hash << "\" or index" << i << " is invalid!\n";
         errorList.push_back(i);
      }

      // Check the middle field content
      for (auto j = 0; j < fieldCount; j++)
      {
         if (locPackEntry.getFields()[j] != locPackBinEntry.m_fields[j])
         {
            cout << "WARNING: Ćontent of entry with hash \"" << hash << "\" or index" << i << " is invalid!\n";
            // Check if the entry is not already registered as an error.
            if (ranges::find(errorList, i) != errorList.end())
            {
               errorList.push_back(i);
            }
         }
      }
   }

   return errorList;
}