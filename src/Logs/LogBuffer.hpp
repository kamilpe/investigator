#pragma once

#include "MemoryMappedFile.hpp"
#include <vector>
#include <string>

using LineId = std::size_t;

class LogBuffer
{
public:
    LogBuffer(const std::vector<std::string>& fileNames);

    bool initialized() const;
    const std::string& name() const;
    char const * line(const LineId id) const;
    unsigned long linesCount() const;

private:
    const std::string name_;
    MemoryMappedFile mappedFile_;
};
