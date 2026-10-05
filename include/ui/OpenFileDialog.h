#pragma once

class OpenFileDialog {
public:
   void Open() { m_isOpened = true; }
   void Render(bool* trackOpen);
private:
   bool m_isOpened = false;
   char m_locPath[512] = "";
   char m_binPath[512] = "";
   bool m_verifyIntegrity = true;
   bool m_createBackup = false;
};