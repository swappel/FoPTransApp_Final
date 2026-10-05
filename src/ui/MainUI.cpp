#include "ui/MainUI.h"
#include <string>
#include <cstring>

#include "files/FileManager.h"

MainUI::MainUI()
    : m_selectedIndex(-1)
{
    std::memset(m_textBuffer, 0, sizeof(m_textBuffer));
}

MainUI::~MainUI()
= default;

/**
 * Renders the main UI.
 */
void MainUI::Render()
{
    m_MenuBar.Render();
    if (m_MenuBar.fileSelectOpen) {
        m_openFileDialog.Open();
    }
    m_openFileDialog.Render(&m_MenuBar.fileSelectOpen);

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration |
                                    ImGuiWindowFlags_NoMove |
                                    ImGuiWindowFlags_NoResize |
                                    ImGuiWindowFlags_NoBringToFrontOnFocus;

    if (ImGui::Begin("Main Editor", nullptr, window_flags))
    {
        m_localeTable.Render(m_locPack);
    }
    ImGui::End();
}

/**
 * Uses the paths retrieved from the user over the UI to load the localization files.
 * Also checks for validity.
 * @param locPackPath The path of the .locpack
 * @param locPackBinPath The path of the .locpackbin file to edit
 * @return `true` if the loading of the files was successful, `false` otherwise.
 */
bool MainUI::LoadProject(const std::filesystem::path& locPackPath, const std::filesystem::path& locPackBinPath)
{
    m_locPack.setPath(locPackPath);
    bool lpSuccess = m_locPack.load();

    m_locPackBin.setPath(locPackBinPath);
    bool lpbSuccess = m_locPackBin.load();

    if (!(lpSuccess && lpbSuccess))
    {
        fprintf(stderr, "Failed to load one or more files.\n");
        return false;
    }

    // TODO: Lots of text output. Add debug flag somewhere to toggle this.
    // if (!verifyFiles(m_locPack, m_locPackBin).empty())
    // {
    //     fprintf(stderr, "Files are not equal and were not loaded.");
    //     return false;
    // }

    printf("Successfully loaded files.\n");
    return true;
}