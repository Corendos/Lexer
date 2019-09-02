#ifndef __TOKENINFO_HPP__
#define __TOKENINFO_HPP__

#include <string>
#include <vector>

struct TokenInfo {
    std::string type;
    int priority;
};

std::vector<TokenInfo> concatenate(const std::vector<std::vector<TokenInfo>>& statePayloads);

#endif