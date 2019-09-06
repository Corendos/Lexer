#include "State.hpp"

State::State() {
}

State::State(const std::string& name, bool accepting, bool starting, const std::vector<TokenInfo>& payload) :
    name(name), isAccepting(accepting), isStarting(starting), payload(payload) {
}

State::State(const State& other) :
    name(other.name), isAccepting(other.isAccepting), isStarting(other.isStarting), payload(other.payload) {
}

State::State(const State&& other) :
    name(std::move(other.name)), isAccepting(other.isAccepting), isStarting(other.isStarting), payload(std::move(other.payload)) {
}

State& State::operator=(const State& other) {
    this->name = other.name;
    this->isAccepting = other.isAccepting;
    this->isStarting = other.isStarting;
    this->payload = other.payload;

    return *this;
}

State& State::operator=(const State&& other) {
    this->name = std::move(other.name);
    this->isAccepting = other.isAccepting;
    this->isStarting = other.isStarting;
    this->payload = std::move(other.payload);

    return *this;
}

bool operator<(const State& a, const State& b) {
    return a.name < b.name;
}

bool operator==(const State& a, const State& b) {
    return a.name == b.name;
}