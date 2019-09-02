#include "TokenInfo.hpp"

#include <algorithm>

std::vector<TokenInfo> concatenate(const std::vector<std::vector<TokenInfo>>& statePayloads) {
    size_t finalSize{0};
    std::for_each(statePayloads.begin(), statePayloads.end(),
                  [&finalSize](const std::vector<TokenInfo>& payload) { finalSize += payload.size(); });
    std::vector<TokenInfo> tokenInfos{finalSize};

    auto it = tokenInfos.begin();
    std::for_each(statePayloads.begin(), statePayloads.end(),
                  [&it](const std::vector<TokenInfo>& payload) {
                      it = std::copy(payload.begin(), payload.end(), it);
                  });
                  
    return tokenInfos;
}