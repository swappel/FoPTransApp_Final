#pragma once

#include <imgui.h>

class MenuBar {
public:
   MenuBar() = default;

   void Render();

private:
   void ShowFileMenu();
   void ShowEditMenu();
   void ShowViewMenu();
};