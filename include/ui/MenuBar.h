#pragma once

class MenuBar {
public:
   MenuBar() = default;

   bool m_fileSelectOpen = false;

   void Render();

private:
   void ShowFileMenu();
   void ShowEditMenu();
   void ShowViewMenu();
};