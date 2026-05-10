#include "LogBuffer.hpp"

LogBuffer::LogBuffer(const std::vector<std::string>& fileNames)
    : name_(fileNames.front())
{
    mappedFile_.open(name_);
}


const std::string& LogBuffer::name() const
{
    return name_;
}

char const * LogBuffer::line(const unsigned id) const
{
    return mappedFile_.lines().at(id);
}

unsigned long LogBuffer::linesCount() const
{
    return mappedFile_.lines().size();
}
