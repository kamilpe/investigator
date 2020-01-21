#include "LogBuffer.hpp"

#include <cctype>
#include <fstream>
#include <algorithm>

constexpr auto LINE_BUFFER = 1024;

LogBuffer::LogBuffer(const std::vector<std::string>& fileNames)
    : name_(fileNames.front())
{
    for (const auto& fileName : fileNames)
    {
        std::ifstream logFile{fileName};
        if (!logFile.is_open())
        {
            return;
        }

        char buffer[LINE_BUFFER];
        while(logFile.getline(buffer, LINE_BUFFER-1))
        {
            std::string line(buffer);
            auto notValidCharacter = [](const char c){return !std::isprint(c);};
            line.erase(
                std::remove_if(
                    line.begin(),
                    line.end(),
                    notValidCharacter),
                line.end());
            lines_.emplace_back(line);
        }
    }

    initialized_ = true;
}

bool LogBuffer::initialized() const
{
    return initialized_;
}

const std::string& LogBuffer::name() const
{
    return name_;
}

const char* LogBuffer::line(const unsigned id) const
{
    return lines_[id].c_str();
}

unsigned long LogBuffer::linesCount() const
{
    return lines_.size();
}
