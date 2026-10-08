#pragma once
#include <vector>

#include "LocaleLine.h"


class LineSearchResult
{
private:
    const std::vector<LocaleLine> m_foundLines;
    unsigned int m_foundLineCount;

public:
    LineSearchResult();
    LineSearchResult(const std::vector<LocaleLine>& foundLines);

    [[nodiscard]] const std::vector<LocaleLine> getRange(unsigned int startIdx, unsigned int count);
};
