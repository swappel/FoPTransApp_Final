#pragma once

#include <imgui.h>
#include "files/LocPackFile.h"
#include "files/LocPackBinFile.h"

class MainUI
{
public:
    MainUI();
    ~MainUI();

    // The main render loop for your UI
    void Render();

private:
    // File logic instances
    LocPackFile m_locPack;
    LocPackBinFile m_locPackBin;

    int m_selectedIndex = -1;
    char m_textBuffer[4096] = "";

    // UI State
    float m_leftPaneWidth = 300.0f; // Initial width for the draggable splitter
    void ShowSidebar();
    void ShowEditor();
};
