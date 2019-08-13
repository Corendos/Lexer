#include "StatePayload.hpp"

#include <algorithm>

StatePayload StatePayload::concatenate(const StatePayload& a, const StatePayload& b) {
    std::vector<TokenInfo> tokenInfos;
    tokenInfos.resize(a.tokenInfos.size() + b.tokenInfos.size());
    auto it = std::copy(a.tokenInfos.begin(), a.tokenInfos.end(), tokenInfos.begin());
    std::copy(b.tokenInfos.begin(), b.tokenInfos.end(), it);

    StatePayload payload;
    payload.tokenInfos = std::move(tokenInfos);
    return payload;
}

StatePayload StatePayload::concatenate(const std::vector<StatePayload>& statePayloads) {
    size_t finalSize{0};
    std::for_each(statePayloads.begin(), statePayloads.end(),
                  [&finalSize](const StatePayload& payload) { return payload.tokenInfos.size(); });
    std::vector<TokenInfo> tokenInfos{finalSize};

    StatePayload::ConstIterator it = tokenInfos.begin();
    std::for_each(statePayloads.begin(), statePayloads.end(),
                  [&it](const StatePayload& payload) {
                      it = std::copy(payload.tokenInfos.begin(), payload.tokenInfos.end(), it);
                  });
                  
    StatePayload payload;
    payload.tokenInfos = std::move(tokenInfos);
    return payload;
}