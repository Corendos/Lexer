#ifndef __NFA_HPP__
#define __NFA_HPP__

#include <string>
#include <variant>
#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <cassert>

#include "State.hpp"
#include "StatePayload.hpp"

using Alphabet = std::string;
using CharType = Alphabet::value_type;

class NFA {
    using MarkedState = std::pair<size_t, bool>;
    using MarkedStateSet = std::pair<std::set<size_t>, bool>;
    public:
        NFA(const Alphabet& alphabet);
        NFA(const std::vector<State>& states,
            const std::map<std::pair<size_t, CharType>, size_t>& characterTransitionTable,
            const Alphabet& alphabet);
        NFA(NFA& other) = delete;
        NFA(NFA&& other) = delete;
        NFA& operator=(NFA& other) = delete;
        NFA& operator=(NFA&& other) = delete;

        void addState(const State& state);
        void addTransition(const State& from, const CharType& character, const State& to);
        void addTransition(const State& from, /*       Empty         */  const State& to);
        void addTransitions(const State& from, const Alphabet& characters, const State& to);

        std::vector<TokenInfo> find(const std::string& word) const;

        void printDebug() const;
        NFA toDFA() const;

    private:
        Alphabet mAlphabet;

        std::map<std::pair<size_t, CharType>, size_t> mCharacterTransitionTable;
        std::map<size_t, std::vector<size_t>> mEmptyTransitionTable;
        std::vector<State> mStates;

        bool exists(const State& state);
        std::set<size_t> findReachableStates(const std::set<size_t>& startingState,
                                             const CharType& c) const;
        std::vector<State> computeNewStates(const std::vector<MarkedStateSet>& markedStateSetsSet) const;
        static bool isStateMarked(const MarkedState& ms);
        
        std::set<State> epsilonClosure(const std::set<State>& states) const;
        std::set<State> epsilonClosure(const std::set<size_t>& states) const;
        std::set<size_t> epsilonClosureIndex(const std::set<State>& states) const;
        std::set<size_t> epsilonClosureIndex(const std::set<size_t>& states) const;
        std::set<size_t> computeStartingState() const;
};

#endif