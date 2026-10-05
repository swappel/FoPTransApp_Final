#include "ui/LocaleTable.h"
#include <cstring>
#include <imgui.h>
#include <string>

/**
 * @brief Used to render the localizatio ntable in ImGui.
 * @param locPack The `LocPackFile` to work with.
 */
void LocaleTable::Render(LocPackFile& locPack) {
    DrawTable(locPack);
    DrawEditModal(locPack);
}

/**
 * @brief Defines how the table is drawn.
 * @param locPack The .locpack(`LocPackFile` Object) to base this of.
 */
void LocaleTable::DrawTable(LocPackFile& locPack) {
    const size_t totalLines = locPack.getEntryCount();
    if (totalLines == 0) {
        ImGui::Text("No entries to display. Load a file first.");
        return;
    }

    // Dynamic Column Calculation
    const unsigned int middleFieldCount = locPack.getFieldCount() - 2;
    const int totalCols = 2 + static_cast<int>(middleFieldCount);

    static ImGuiTableFlags flags = ImGuiTableFlags_Borders |
                                   ImGuiTableFlags_RowBg |
                                   ImGuiTableFlags_ScrollY |
                                   ImGuiTableFlags_Resizable |
                                   ImGuiTableFlags_Hideable |
                                   ImGuiTableFlags_Reorderable;

    // Fill the entire window
    if (ImGui::BeginTable("LocaleDataTable", totalCols, flags, ImVec2(0, 0))) {
        ImGui::TableSetupColumn("Hash", ImGuiTableColumnFlags_WidthFixed, 180.0f);

        for (unsigned int i = 0; i < middleFieldCount; i++) {
            std::string colName = "Field " + std::to_string(i);
            ImGui::TableSetupColumn(colName.c_str(), ImGuiTableColumnFlags_WidthFixed, 70.0f);
        }

        ImGui::TableSetupColumn("Content", ImGuiTableColumnFlags_WidthStretch);

        ImGui::TableHeadersRow();

        ImGuiListClipper clipper;
        clipper.Begin(static_cast<int>(totalLines));

        while (clipper.Step()) {
            for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++) {
                auto line = locPack.findFromIndex(i);
                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);
                bool isSelected = (m_selectedIndex == i);
                if (ImGui::Selectable(line.getHash().c_str(), isSelected, ImGuiSelectableFlags_SpanAllColumns)) {
                    m_selectedIndex = i;
                    m_editingIndex = i;
                    strncpy(m_editBuffer, line.getContent().c_str(), sizeof(m_editBuffer));
                    ImGui::OpenPopup("EditEntryModal");
                }

                const auto& fields = line.getFields();
                for (unsigned int j = 0; j < middleFieldCount; j++) {
                    ImGui::TableSetColumnIndex(j + 1);
                    if (j < fields.size()) {
                        ImGui::Text("%d", fields[j]);
                    }
                }

                ImGui::TableSetColumnIndex(totalCols - 1);
                ImGui::TextUnformatted(line.getContent().c_str());
            }
        }
        ImGui::EndTable();
    }
}

void LocaleTable::DrawEditModal(LocPackFile& locPack) {
    const ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("EditEntryModal", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        if (m_editingIndex != -1) {
            const auto line = locPack.findFromIndex(m_editingIndex);
            ImGui::Text("Key: %s", line.getHash().c_str());
            ImGui::InputTextMultiline("##edit", m_editBuffer, sizeof(m_editBuffer), ImVec2(600, 250));

            if (ImGui::Button("Save", ImVec2(120, 0))) {
                // TODO: Change-saving here
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
        }
        ImGui::EndPopup();
    }
}