#pragma once

#include "files/LocPackFile.h"

class LocaleTable {
public:
   LocaleTable() = default;

   void Render(LocPackFile& locPack);

private:
   void DrawTable(LocPackFile& locPack);
   void DrawEditModal(LocPackFile& locPack);

   int m_selectedIndex = -1;
   int m_editingIndex = -1;
   char m_editBuffer[4096] = "";
};