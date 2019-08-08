#ifndef __STATE_HPP__
#define __STATE_HPP__

#include <string>

struct State {
    State(const std::string& name, bool accepting = false, bool starting = false);
    std::string name;
    bool isAccepting;
    bool isStarting;
};

bool operator<(const State& a, const State& b);
bool operator==(const State& a, const State& b);

#endif