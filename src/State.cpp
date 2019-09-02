#include "State.hpp"

State::State() : State("") {
}

State::State(const std::string& name, bool accepting, bool starting, std::vector<TokenInfo> payload) :
    name(name), isAccepting(accepting), isStarting(starting), payload(payload) {
}

bool operator<(const State& a, const State& b) {
    return a.name < b.name;
}

bool operator==(const State& a, const State& b) {
    return a.name == b.name;
}