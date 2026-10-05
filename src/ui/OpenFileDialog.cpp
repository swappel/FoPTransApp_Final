#include "ui/OpenFileDialog.h"

#include <imgui.h>

/**
 * @brief Function used to render the file opening dialog
 *
 * This functions is used to display the file opening dialog. <br>
 * It uses an externally tracked variable to define whether the window is open or not.
 *
 * @param trackOpen The variable that tracks wether the window is open or not.
 */
void OpenFileDialog::Render(bool* trackOpen)
{
    if (m_isOpened)
    {
        ImGui::OpenPopup("Load Project Files");
        m_isOpened = false;
    }

    const ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("Load Project Files", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::TextDisabled("Select the localization pair to edit:");
        ImGui::Spacing();

        ImGui::Text(".locpack file path:");
        ImGui::SetNextItemWidth(400);
        ImGui::InputText("##locpath", m_locPath, IM_ARRAYSIZE(m_locPath));
        ImGui::SameLine();
        if (ImGui::Button("Browse...##loc")) {
            // Implement the file browser here for selection of .locpack
        }

        ImGui::Text(".locpackbin file path:");
        ImGui::SetNextItemWidth(400);
        ImGui::InputText("##binpath", m_binPath, IM_ARRAYSIZE(m_binPath));
        ImGui::SameLine();
        if (ImGui::Button("Browse...##bin")) {
            // Implement the file browser here for selection of .locpackbin
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // Settings section
        ImGui::TextDisabled("Options:");
        ImGui::Checkbox("Verify file integrity on load", &m_verifyIntegrity);
        ImGui::Checkbox("Create backup of existing files", &m_createBackup);

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // Push buttons to the right
        const float width = ImGui::GetWindowSize().x;
        ImGui::SetCursorPosX(width - 260);

        if (ImGui::Button("Cancel", ImVec2(120, 0))) {
            *trackOpen = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.26f, 0.59f, 0.98f, 1.00f));
        if (ImGui::Button("Load Project", ImVec2(120, 0))) {
            // Call LoadProject here
            ImGui::CloseCurrentPopup();
        }
        ImGui::PopStyleColor();

        ImGui::EndPopup();
    }
}