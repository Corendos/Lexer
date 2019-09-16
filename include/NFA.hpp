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

/**
 * A class representing a Non-Deterministic/Deterministic Finite Automaton.
 * Since a DFA is a particular NFA, I have decided to combine the 2 notions.
 */
class NFA {
    using MarkedState = std::pair<size_t, bool>;
    using MarkedStateSet = std::pair<std::set<size_t>, bool>;
    public:
        /**
         * A constructor.
         * Constructs an empty NFA using the accepted Alphabet.
         * @param alphabet a std::string representing the set of accepted characters.
         */
        NFA(const Alphabet& alphabet);

        /**
         * A constructor.
         * Constructs a NFA using the accepted alphabet, the transitions and the states.
         * @param alphabet a std::string representing the set of accepted characters.
         * @param states a std::vector<State> representing the list of states.
         * @param alphabet a std::map<std::pair<size_t, CharType>, size_t> representing non-empty transitions.
         * @param alphabet a std::map<size_t, std::vector<size_t>> representing empty transitions.
         */
        NFA(const Alphabet& alphabet,
            const std::vector<State>& states,
            const std::map<std::pair<size_t, CharType>, size_t>& characterTransitionTable,
            const std::map<size_t, std::vector<size_t>>& emptyTransitionTable = std::map<size_t, std::vector<size_t>>());
        
        /**
         * A default copy constructor.
         * Constructs a NFA using another NFA.
         * @param other The other NFA.
         */
        NFA(const NFA& other) = default;

        /**
         * A default move constructor.
         * Constructs a NFA by moving another NFA.
         * @param other The other NFA.
         */
        NFA(NFA&& other) = default;

        /**
         * A default copy assignment operator.
         * Assigns a NFA to the current NFA by copying it.
         * @param other The other NFA.
         * @return a reference to the current NFA.
         */
        NFA& operator=(const NFA& other) = default;

        /**
         * A default move assignment operator.
         * Assigns a NFA to the current NFA by moving it.
         * @param other The other NFA.
         * @return a reference to the current NFA.
         */
        NFA& operator=(NFA&& other) = default;

        /**
         * Add a state to the NFA by copying it.
         * @param state The State to add.
         */
        void addState(const State& state);

        /**
         * Add a state to the NFA by moving it.
         * @param state The State to add.
         */
        void addState(State&& state);

        /**
         * Add a transition labelled by a specific character.
         * @param from The State where the transition comes from.
         * @param from The character laebelling the transition.
         * @param to The State where the transition goes to.
         */
        void addTransition(const State& from, const CharType& character, const State& to);

        /**
         * Add an empty transition.
         * @param from The State where the transition comes from.
         * @param to The State where the transition goes to.
         */
        void addTransition(const State& from, /*       Empty         */  const State& to);

        /**
         * Add transitions labelled by specific characters.
         * @param from The State where the transitions come from.
         * @param from The characters labelling the transitions.
         * @param to The State where the transitions go to.
         */
        void addTransitions(const State& from, const Alphabet& characters, const State& to);


        /**
         * Add a transition labelled by a specific character.
         * @param from The state name where the transition comes from.
         * @param from The character laebelling the transition.
         * @param to The state name where the transition goes to.
         */
        void addTransition(const std::string& from, const CharType& character, const std::string& to);

        /**
         * Add an empty transition.
         * @param from The state name where the transition comes from.
         * @param to The state name where the transition goes to.
         */
        void addTransition(const std::string& from, /*       Empty         */  const std::string& to);

        /**
         * Add transitions labelled by specific characters.
         * @param from The state name where the transitions come from.
         * @param from The characters labelling the transitions.
         * @param to The state name where the transitions go to.
         */
        void addTransitions(const std::string& from, const Alphabet& characters, const std::string& to);

        /**
         * Print the NFA in the console.
         * This is a debug function.
         */
        void printDebug() const;

        /**
         * Transform a NFA to a DFA.
         * @return a NFA representing the corresponding DFA
         */
        NFA toDFA() const;

        /**
         * Combine multiple NFAs to a single NFA
         * 
         * @param nfas  the list of NFA we want to combine
         * @return      the resulting NFA
         */
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