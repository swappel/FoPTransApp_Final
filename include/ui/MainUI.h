#pragma once

#include <imgui.h>

#include "LocaleTable.h"
#include "MenuBar.h"
#include "OpenFileDialog.h"
#include "files/LocPackFile.h"
#include "files/LocPackBinFile.h"

class MainUI
{
public:
    MainUI();
    ~MainUI();

    void Render();

    bool LoadProject(const std::filesystem::path& locPackPath, const std::filesystem::path& locPackBinPath);
private:
    LocPackFile m_locPack;
    LocPackBinFile m_locPackBin;

    MenuBar m_MenuBar;
    OpenFileDialog m_openFileDialog;
    LocaleTable m_localeTable;

    int m_selectedIndex = -1;
    char m_textBuffer[4096] = "";

    float m_leftPaneWidth = 300.0f;
};
