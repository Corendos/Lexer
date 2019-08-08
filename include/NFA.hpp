#ifndef __NFA_HPP__
#define __NFA_HPP__

#include <string>
#include <variant>
#include <map>
#include <vector>
#include <set>

#include "State.hpp"

using Alphabet = std::string;
using CharType = Alphabet::value_type;

class NFA {
    public:
        NFA(const Alphabet& alphabet);
        NFA(NFA& other) = delete;
        NFA(NFA&& other) = delete;
        NFA& operator=(NFA& other) = delete;
        NFA& operator=(NFA&& other) = delete;

        void addState(const State& state);
        void addTransition(const State& from, const CharType& character, const State& to);
        void addTransition(const State& from, /*       Empty         */  const State& to);

        void printDebug() const;
        std::set<State> epsilonClosure(const std::set<State>& states) const;

    private:
        Alphabet mAlphabet;

        std::map<std::pair<size_t, CharType>, size_t> mCharacterTransitionTable;
        std::map<size_t, std::vector<size_t>> mEmptyTransitionTable;
        std::vector<State> mStates;

        bool exists(const State& state);
};

#endif