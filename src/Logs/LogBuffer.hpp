#pragma once

#include <vector>
#include <string>

class LogBuffer
{
public:
    LogBuffer(const std::vector<std::string>& fileNames);

    bool initialized() const;
    const std::string& name() const;
    const char* line(const unsigned id) const;
    unsigned long linesCount() const;

private:
    const std::string name_;
    bool initialized_ = false;
    std::vector<std::string> lines_; // TODO: mmap
};
