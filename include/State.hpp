#ifndef __STATE_HPP__
#define __STATE_HPP__

#include <string>

template <typename T>
struct State {
    State(const std::string& name, const T& payload = T(), bool accepting = false, bool starting = false);
    std::string name;
    bool isAccepting;
    bool isStarting;
    T payload;
};

template <typename T>
State<T>::State(const std::string& name, const T& payload, bool accepting, bool starting) :
    name(name), isAccepting(accepting), isStarting(starting), payload(payload) {
}

template <typename T>
bool operator<(const State<T>& a, const State<T>& b) {
    return a.name < b.name;
}

template <typename T>
bool operator==(const State<T>& a, const State<T>& b) {
    return a.name == b.name;
}

#endif