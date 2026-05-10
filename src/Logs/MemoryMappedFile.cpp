#include "MemoryMappedFile.hpp"
#include <stdexcept>
#include <cstring>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>


void MemoryMappedFile::open(const std::string& fileName)
{
    fd_ = ::open(fileName.c_str(), O_RDONLY);
    if (fd_ == -1) {
        throw std::runtime_error("Unable to open " + fileName);
    }

    struct stat sb;
    if (fstat(fd_, &sb) == -1) {
        throw std::runtime_error("Unable to stat " + fileName);
    }
    length_ = sb.st_size;

    buffer_ = static_cast<char*>(mmap(NULL, length_, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd_, 0u));
    if (buffer_ == MAP_FAILED) {
        throw std::runtime_error("Unable to read " + fileName);
    }
    // close(fd_);
    endOfBuffer_ = buffer_ + length_;
    isOpen_ = true;
    processLines();
}

MemoryMappedFile::~MemoryMappedFile()
{
    // if (isOpen_) {
    //     free(buffer_);
    // }
}

void MemoryMappedFile::processLines()
{
    auto current = buffer_;
    while (current && current <= endOfBuffer_)
    {
        auto endOfString = static_cast<char*>(memchr(current, '\n', endOfBuffer_ - current));
        if (endOfString == 0)
            endOfString = current + strlen(current);

        for (auto* c=current; c <= endOfString; ++c)
            if (!std::isprint(*c))
                *c = '\0';

        lines_.push_back(current);
        current = endOfString + 1;
    }
}

bool MemoryMappedFile::isOpen() const
{
    return isOpen_;
}

const std::vector<char const*>& MemoryMappedFile::lines() const
{
    return lines_;
}
