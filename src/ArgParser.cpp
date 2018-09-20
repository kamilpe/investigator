#include "ArgParser.hpp"

#include <iostream>

ArgParser::ArgParser(int argv, char **argc)
{
    for (int i=1; i < argv; ++i)
    {
        const auto arg = std::string(argc[i]);
        if (!arg.compare(0, 2, "--"))
        {
            handleArgument(arg);
        }
        else
        {
            handleFileName(arg);
        }
    }
}


void ArgParser::handleArgument(const std::string& arg)
{
    std::cout << "Unrecognized argument: " << arg << std::endl;
    errors_ = true;
}

void ArgParser::handleFileName(const std::string& file)
{
    files_.push_back(file);
}

bool ArgParser::errors() const
{
    return errors_;
}

std::vector<std::string> ArgParser::files() const
{
    return files_;
}
