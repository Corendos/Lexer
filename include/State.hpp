#ifndef __STATE_HPP__
#define __STATE_HPP__

#include <string>

#include "StatePayload.hpp"

struct State {
    State();
    State(const std::string& name,
          const StatePayload& payload = StatePayload(),
          bool accepting = false,
          bool starting = false);
    std::string name;
    bool isAccepting;
    bool isStarting;
    StatePayload payload;
};

bool operator<(const State& a, const State& b);
bool operator==(const State& a, const State& b);

#endif