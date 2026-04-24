#pragma once
#include <filesystem>
#include <string>

namespace GitHubManager
{
    // Initial configuration
    void setupUser(const std::string& username);

    // GitHub management
    void initializeConnection();
    bool pullUpdates();
    bool pushUpdates();

    // Utilities
    void startupTasks();

    bool isConfigPresent();
    void readConfig();
};
