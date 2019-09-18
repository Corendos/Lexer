#ifndef __TOKENINFO_HPP__
#define __TOKENINFO_HPP__

#include <string>
#include <vector>

/**
 * TokenInfo structure.
 * Represents the information about a token.
 */
struct TokenInfo {
    std::string type;   //< The token type.
    int priority;       //< The token priority.
};

/**
 * Concatenate multiple list of token infos in a single list.
 * @param statePayloads The list of list of tokens to concatenate.
 * @return a list of token infos.
 */
std::vector<TokenInfo> concatenate(const std::vector<std::vector<TokenInfo>>& statePayloads);

#endif