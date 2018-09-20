#pragma once

#include <vector>
#include <string>

class ArgParser
{
public:
    ArgParser(int argv, char **argc);

    bool errors() const;
    std::vector<std::string> files() const;

private:
    std::vector<std::string> files_;
    bool errors_ = false;

    void handleArgument(const std::string& arg);
    void handleFileName(const std::string& file);
};
