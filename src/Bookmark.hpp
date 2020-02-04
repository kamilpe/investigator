#pragma once

#include <string>
#include <vector>
#include "Logs/LogBuffer.hpp"

struct Bookmark
{
public:
    Bookmark(std::string n, LineId id) : name(n), id(id) {};
    std::string name;
    LineId id;
};

class BookmarksContainer : public std::vector<Bookmark>
{
public:
    void insert(const Bookmark& bookmark);
};
