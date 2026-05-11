#pragma once

#include <imgui.h>

#include "MenuBar.h"
#include "files/LocPackFile.h"
#include "files/LocPackBinFile.h"

class MainUI
{
public:
    MainUI();
    ~MainUI();

    // The main render loop for your UI
    void Render();

    bool LoadProject(const std::filesystem::path& locPackPath, const std::filesystem::path& binPath);
private:
    // File logic instances
    LocPackFile m_locPack;
    LocPackBinFile m_locPackBin;

    // Elelements
    MenuBar m_MenuBar;

    int m_selectedIndex = -1;
    char m_textBuffer[4096] = "";

    // UI State
    float m_leftPaneWidth = 300.0f; // Initial width for the draggable splitter
    void ShowSidebar();
    void ShowEditor();
};
