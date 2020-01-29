#pragma once

#include <string>

struct Bookmark
{
public:
    Bookmark(std::string n, int id) : name(n), id(id) {};
    std::string name;
    int id;
};

using BookmarksContainer = std::vector<Bookmark>;
