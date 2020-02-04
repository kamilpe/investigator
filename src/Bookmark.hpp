#pragma once

#include <string>
#include <vector>

struct Bookmark
{
public:
    Bookmark(std::string n, int id) : name(n), id(id) {};
    std::string name;
    int id;
};

class BookmarksContainer : public std::vector<Bookmark>
{
public:
    void insert(const Bookmark& bookmark);
};
