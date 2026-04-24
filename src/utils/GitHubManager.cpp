#include "utils/GitHubManager.h"

using namespace std;

namespace GitHubManager
{
    static string s_username{};
    static filesystem::path s_sshPublicKeyPath{};
    static const string s_REPO_URL = "git@github.com:swappel/fop-translations.git";

    void setupUser()
    {
        /*
         * - Prompt the user for name
         * - Create ./config/config.yaml
         * - Create ./.ssh/ and generate key
         * - Add GitHub to known hosts
         * - Check if user branch is present, if not, create branch, pull from main and push to new branch
         */
    }

    void initializeConnection()
    {
        /*
         * - Check if configuration is present, otherwise, run setupUser()
         * If present:
         * - Pull main, push to user branch
         * - Update local
         */
    }

    bool pullUpdates()
    {
        /*
         * - Pull main into user branch and push to remote.
         * Returns true if successful, false if conflict or other issue.
         * (Maybe find an interactive way for user to manage this, or prompt to contact admin)
         */
        return false;
    }

    bool pushUpdates()
    {
        /*
         * - Push local changes to remote personal branch
         * Returns true if successful, otherwise returns false and prompts user in interactive way(same as above)
         */
        return false;
    }

    void startupTasks()
    {
        /*
         * - Checks if setup is complete
         * - If not, starts setup
         * - If yes, starts initializing connection
         */
    }

    bool isConfigPresent()
    {
        // Checks if configuration files and structure is present
        return false;
    }

    void readConfig()
    {
        // Parses the config and initializes the variables in here
    }
}
