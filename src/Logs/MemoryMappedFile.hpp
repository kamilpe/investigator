#pragma once

#include <string>
#include <vector>

class MemoryMappedFile
{
public:
    ~MemoryMappedFile();
    void open(const std::string& fileName);
    bool isOpen() const;
    const std::vector<char const*>& lines() const;

private:
    bool isOpen_ = false;
    int fd_ = 0;
    char *buffer_ = NULL;
    char *endOfBuffer_ = NULL;
    unsigned long length_ = 0;
    std::vector<char const *> lines_;

    void processLines();
};
