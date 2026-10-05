#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <limits>

#include "files/LocPackFile.h"
#include "files/LocPackBinFile.h"
#include "files/FileManager.h"

using namespace std;

// Helper to reliably read a full line (including spaces) from std::cin
string readPathInput() {
    string path;
    getline(cin >> ws, path); // 'ws' strips any leading whitespace/newlines left in the buffer
    // Remove surrounding quotes if the user dragged and dropped a file into the terminal
    if (path.size() >= 2 && path.front() == '"' && path.back() == '"') {
        path = path.substr(1, path.size() - 2);
    }
    return path;
}

void printLineInfo(const LocaleLine& line) {
    cout << "\n--- Line Info ---" << endl;
    cout << "Hash:    " << line.getHash() << endl;
    cout << "Content: " << line.getContent() << endl;
    cout << "Fields:  [";
    const auto& fields = line.getFields();
    for (size_t i = 0; i < fields.size(); ++i) {
        cout << fields[i] << (i + 1 < fields.size() ? ", " : "");
    }
    cout << "]" << endl;
    cout << "-----------------" << endl;
}

int main() {
    cout << "=== LocPack Testing Tool ===" << endl;
    cout << "Enter path to .locpack file: ";
    string locPackPathStr = readPathInput();

    filesystem::path locPackPath(locPackPathStr);
    LocPackFile locPackFile(locPackPath);

    if (!locPackFile.load()) {
        cerr << "Error: Could not load .locpack file at: " << locPackPathStr << endl;
        return 1;
    }
    cout << "Successfully loaded .locpack file!" << endl;

    cout << "Enter path to corresponding .locpackbin file: ";
    string locPackBinPathStr = readPathInput();

    filesystem::path locPackBinPath(locPackBinPathStr);
    LocPackBinFile locPackBinFile(locPackBinPath);

    if (!locPackBinFile.load()) {
        cerr << "Error: Could not load .locpackbin file at: " << locPackBinPathStr << endl;
        return 1;
    }
    cout << "Successfully loaded .locpackbin file!" << endl;

    int choice = 0;
    while (choice != 7) {
        cout << "\nChoose an action:" << endl;
        cout << "1. Print total line count" << endl;
        cout << "2. Find entry by Index" << endl;
        cout << "3. Find entry by Hash" << endl;
        cout << "4. Verify files against each other" << endl;
        cout << "5. Add changes to cache (stage modification)" << endl;
        cout << "6. Commit/Flush all cached changes to disk (.locpack & .locpackbin)" << endl;
        cout << "7. Exit" << endl;
        cout << "Choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1: {
                try {
                    auto lines = locPackFile.parseLocPackComplete();
                    cout << "\nTotal entries parsed: " << lines.size() << endl;
                } catch (const exception& e) {
                    cerr << "Error parsing file: " << e.what() << endl;
                }
                break;
            }
            case 2: {
                int index;
                cout << "Enter index: ";
                cin >> index;
                try {
                    LocaleLine line = locPackFile.findFromIndex(index);
                    printLineInfo(line);
                } catch (const exception& e) {
                    cerr << "Error: " << e.what() << endl;
                }
                break;
            }
            case 3: {
                string hash;
                cout << "Enter hash: ";
                cin >> hash;
                try {
                    LocaleLine line = locPackFile.findFromHash(hash);
                    if (line.getHash().empty()) {
                        cout << "Hash not found." << endl;
                    } else {
                        printLineInfo(line);
                    }
                } catch (const exception& e) {
                    cerr << "Error: " << e.what() << endl;
                }
                break;
            }
            case 4: {
                cout << "Verifying files..." << endl;
                vector<int> discrepancies = verifyFiles(locPackFile, locPackBinFile);

                if (discrepancies.empty()) {
                    cout << ">>> Files match perfectly! No discrepancies found. <<<" << endl;
                } else {
                    cout << "\n>>> Found discrepancies at " << discrepancies.size() << " indices: <<<" << endl;
                    for (int idx : discrepancies) {
                        cout << " - Index " << idx << endl;
                    }
                }
                break;
            }
            case 5: {
                string hash, content;
                cout << "Enter target hash: ";
                cin >> hash;

                const unsigned int expectedFields = locPackFile.getFieldCount() - 2;
                cout << "Enter field count (expected " << expectedFields << "): ";
                int numFields;
                cin >> numFields;

                vector<int> fields(numFields);
                for (int i = 0; i < numFields; ++i) {
                    cout << " Field [" << i << "]: ";
                    cin >> fields[i];
                }

                cout << "Enter new text content: ";
                getline(cin >> ws, content);

                try {
                    // Stage in CSV change cache
                    locPackFile.addChanges(hash, fields, content);

                    // Stage in binary buffer
                    int val1 = fields.size() > 0 ? fields[0] : 0;
                    int val2 = fields.size() > 1 ? fields[1] : 0;
                    locPackBinFile.applyEntryUpdate(hash, val1, val2, content);

                    cout << ">>> Staged change in cache! Choose option 6 to flush to disk. <<<" << endl;
                } catch (const exception& e) {
                    cerr << "Failed to add change: " << e.what() << endl;
                }
                break;
            }
            case 6: {
                cout << "Flushing changes to disk..." << endl;
                try {
                    // 1. Flush CSV (.locpack)
                    locPackFile.writeEntry();

                    // 2. Flush Binary (.locpackbin)
                    if (!locPackBinFile.save()) {
                        throw runtime_error("Failed to write updated binary file.");
                    }

                    cout << ">>> Successfully flushed all cached changes to disk! <<<" << endl;
                } catch (const exception& e) {
                    cerr << "Error writing files to disk: " << e.what() << endl;
                }
                break;
            }
            case 7:
                cout << "Exiting test harness." << endl;
                break;
            default:
                cout << "Invalid choice. Try again." << endl;
                break;
        }
    }

    return 0;
}