#include "../../include/utils/LineSearchResult.h"

LineSearchResult::LineSearchResult() = default;

LineSearchResult::LineSearchResult(const std::vector<LocaleLine>& foundLines) :
    m_foundLines(foundLines)

{
    m_foundLineCount = foundLines.size();
}

const std::vector<LocaleLine> LineSearchResult::getRange(unsigned int startIdx, unsigned int count)
{

}
