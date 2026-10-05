#include "ui/MenuBar.h"

#include <cstdlib>
#include <imgui.h>

/**
 * Function to render the top Navbar
 */
void MenuBar::Render() {
   if (ImGui::BeginMainMenuBar()) {
      if (ImGui::BeginMenu("File")) {
         if (ImGui::MenuItem("Open...", "Ctrl+O")) { m_fileSelectOpen = true; }
         if (ImGui::MenuItem("Save", "Ctrl+S"))
         {
            // TODO: Logic for file saving
         }
         ImGui::Separator();
         if (ImGui::MenuItem("Exit", "Alt+F4")) { exit(0); }
         ImGui::EndMenu();
      }

      if (ImGui::BeginMenu("Edit")) {
         if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
         ImGui::EndMenu();
      }

      if (ImGui::BeginMenu("View")) {
         if (ImGui::MenuItem("Dark Mode")) { ImGui::StyleColorsDark(); }
         if (ImGui::MenuItem("Light Mode")) { ImGui::StyleColorsLight(); }
         ImGui::EndMenu();
      }
      ImGui::EndMainMenuBar();
   }
}

/**
 * Function used to display the "File" menu.
 */
void MenuBar::ShowFileMenu() {
   if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("Open...", "Ctrl+O")) { /* Trigger callback or event */ }
      if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Trigger callback or event */ }
      ImGui::Separator();
      if (ImGui::MenuItem("Exit")) { /* Handle Exit */ }
      ImGui::EndMenu();
   }
}

/**
 * Function used to display the "Edit" menu.
 */
void MenuBar::ShowEditMenu() {
   if (ImGui::BeginMenu("Edit")) {
      if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
      if (ImGui::MenuItem("Redo", "Ctrl+Y")) {}
      ImGui::EndMenu();
   }
}

/**
 * Function used to display the "View" menu.
 */
void MenuBar::ShowViewMenu() {
   if (ImGui::BeginMenu("View")) {
      static bool show_sidebar = true;
      ImGui::MenuItem("Show Sidebar", nullptr, &show_sidebar);
      ImGui::EndMenu();
   }
}