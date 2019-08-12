#ifndef __TOKENINFO_HPP__
#define __TOKENINFO_HPP__

#include <string>

struct TokenInfo {
    TokenInfo() : TokenInfo("", 0) {}
    TokenInfo(const std::string& type, int priority) : type(type), priority(priority) {}
    std::string type;
    int priority;
};

#endif