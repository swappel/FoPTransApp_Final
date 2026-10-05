#pragma once
#include <string>

class SearchBar
{
private:
   std::string m_currentContent;
   long m_lastChangeTime = 0;
   bool finishedSearch = true;

   void RenderSettings();
   void DetectSearch();
public:
   void Render();
   void Search();
};
