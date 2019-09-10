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

using Alphabet = std::string;
using CharType = Alphabet::value_type;

class NFA {
    using MarkedState = std::pair<size_t, bool>;
    using MarkedStateSet = std::pair<std::set<size_t>, bool>;
    public:
        NFA(const Alphabet& alphabet);
        NFA(const Alphabet& alphabet,
            const std::vector<State>& states,
            const std::map<std::pair<size_t, CharType>, size_t>& characterTransitionTable,
            const std::map<size_t, std::vector<size_t>>& emptyTransitionTable = std::map<size_t, std::vector<size_t>>());
        NFA(const NFA& other) = default;
        NFA(NFA&& other) = default;
        NFA& operator=(const NFA& other) = default;
        NFA& operator=(NFA&& other) = default;

        void addState(const State& state);
        void addState(State&& state);
        void addTransition(const State& from, const CharType& character, const State& to);
        void addTransition(const State& from, /*       Empty         */  const State& to);
        void addTransitions(const State& from, const Alphabet& characters, const State& to);

        void addTransition(const std::string& from, const CharType& character, const std::string& to);
        void addTransition(const std::string& from, /*       Empty         */  const std::string& to);
        void addTransitions(const std::string& from, const Alphabet& characters, const std::string& to);

        void printDebug() const;
        NFA toDFA() const;

        static NFA combine(const std::vector<NFA>& nfas);

        friend class Traverser;

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
    
    friend class NFAIO;
};

#endif