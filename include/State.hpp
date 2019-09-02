#ifndef __STATE_HPP__
#define __STATE_HPP__

#include <string>
#include <vector>

#include "TokenInfo.hpp"

struct State {
    State();
    State(const std::string& name,
          bool accepting = false,
          bool starting = false,
          std::vector<TokenInfo> payload = std::vector<TokenInfo>());
    std::string name;
    bool isAccepting;
    bool isStarting;
    std::vector<TokenInfo> payload;
};

bool operator<(const State& a, const State& b);
bool operator==(const State& a, const State& b);

#endif