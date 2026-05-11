#include "ui/MenuBar.h"

#include <cstdlib>

void MenuBar::Render() {
   if (ImGui::BeginMainMenuBar()) { // This stays ABOVE all other windows
      if (ImGui::BeginMenu("File")) {
         if (ImGui::MenuItem("Open...", "Ctrl+O")) { /* logic */ }
         if (ImGui::MenuItem("Save", "Ctrl+S")) { /* logic */ }
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

void MenuBar::ShowFileMenu() {
   if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("Open...", "Ctrl+O")) { /* Trigger callback or event */ }
      if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Trigger callback or event */ }
      ImGui::Separator();
      if (ImGui::MenuItem("Exit")) { /* Handle Exit */ }
      ImGui::EndMenu();
   }
}

void MenuBar::ShowEditMenu() {
   if (ImGui::BeginMenu("Edit")) {
      if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
      if (ImGui::MenuItem("Redo", "Ctrl+Y")) {}
      ImGui::EndMenu();
   }
}

void MenuBar::ShowViewMenu() {
   if (ImGui::BeginMenu("View")) {
      static bool show_sidebar = true;
      ImGui::MenuItem("Show Sidebar", nullptr, &show_sidebar);
      ImGui::EndMenu();
   }
}