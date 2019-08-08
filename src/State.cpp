#include "State.hpp"

State::State(const std::string& name, bool accepting, bool starting) :
    name(name), isAccepting(accepting), isStarting(starting) {
}

bool operator<(const State& a, const State& b) {
    return a.name < b.name;
}

bool operator==(const State& a, const State& b) {
    return a.name == b.name;
}