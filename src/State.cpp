#include "State.hpp"

State::State(const std::string& name, const StatePayload& payload, bool accepting, bool starting) :
    name(name), isAccepting(accepting), isStarting(starting), payload(payload) {
}

bool operator<(const State& a, const State& b) {
    return a.name < b.name;
}

bool operator==(const State& a, const State& b) {
    return a.name == b.name;
}