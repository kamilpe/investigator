#pragma once

#include <string>

class Bookmark
{
public:
    Bookmark(std::string n, int id) : name(n), id(id) {};
    std::string name;
    int id;
};
