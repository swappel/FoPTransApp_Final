#include "ui/MainUI.h"
#include <string>
#include <cstring>

MainUI::MainUI()
    : m_selectedIndex(-1)
{
    std::memset(m_textBuffer, 0, sizeof(m_textBuffer));
}

MainUI::~MainUI()
= default;

void MainUI::Render()
{
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;

    if (ImGui::Begin("Main Editor", nullptr, window_flags))
    {
        if (ImGui::BeginTable("MainLayout", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersInnerV))
        {
            ImGui::TableSetupColumn("Sidebar", ImGuiTableColumnFlags_WidthFixed, 300.0f);
            ImGui::TableSetupColumn("Editor", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableNextRow();

            // --- LEFT COLUMN: Sidebar with Clipper ---
            ImGui::TableSetColumnIndex(0);
            ImGui::BeginChild("SidebarChild");
            ShowSidebar();
            ImGui::EndChild();

            // --- RIGHT COLUMN: Editor View ---
            ImGui::TableSetColumnIndex(1);
            ImGui::BeginChild("EditorChild");
            ShowEditor();
            ImGui::EndChild();

            ImGui::EndTable();
        }
    }
    ImGui::End();
}

void MainUI::ShowSidebar()
{
    size_t totalLines = m_locPack.getEntryCount();

    if (totalLines == 0)
    {
        ImGui::Text("No data loaded.");
        return;
    }

    // 1. Initialize the clipper
    ImGuiListClipper clipper;
    clipper.Begin((int)totalLines);

    // 2. Loop through visible items only
    while (clipper.Step())
    {
        for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
        {
            // Fetch entry data for the specific index
            LocaleLine line = m_locPack.findFromIndex(i);
            std::string label = line.getHash();

            // Selection logic
            bool isSelected = (m_selectedIndex == i);
            if (ImGui::Selectable(label.c_str(), isSelected))
            {
                m_selectedIndex = i;
                // Update editor buffer with new content
                strncpy(m_textBuffer, line.getContent().c_str(), sizeof(m_textBuffer));
            }

            if (isSelected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
    }
}

void MainUI::ShowEditor()
{
    if (m_selectedIndex == -1)
    {
        ImGui::Text("Select a hash from the list to edit.");
        return;
    }

    ImGui::Text("Editing Index: %d", m_selectedIndex);
    ImGui::Separator();

    // Editor for the content
    if (ImGui::InputTextMultiline("##content", m_textBuffer, IM_ARRAYSIZE(m_textBuffer), ImVec2(-FLT_MIN, -FLT_MIN)))
    {
        // Handle live changes or mark as dirty here
    }
}
