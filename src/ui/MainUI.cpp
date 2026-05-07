#include "ui/MainUI.h"
#include "imgui_internal.h"

void MainUI::render()
{
   showDockSpace();

   ImGui::Begin("A:FoP Localization Editor");
   ImGui::Text("Hello Docking!");
   ImGui::End();
}


void MainUI::showDockSpace()
{
   static bool opt_padding = false;
}
