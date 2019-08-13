#ifndef __STATEPAYLOAD_HPP__
#define __STATEPAYLOAD_HPP__

#include <vector>

#include "TokenInfo.hpp"

struct StatePayload {
    using Iterator = std::vector<TokenInfo>::iterator;
    using ConstIterator = std::vector<TokenInfo>::const_iterator;
    using ReverseIterator = std::vector<TokenInfo>::reverse_iterator;
    using ConstReverseIterator = std::vector<TokenInfo>::const_reverse_iterator;

    std::vector<TokenInfo> tokenInfos;

    static StatePayload concatenate(const StatePayload& a, const StatePayload& b);
    static StatePayload concatenate(const std::vector<StatePayload>& statePayloads);
};

#endif