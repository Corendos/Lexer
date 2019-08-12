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
#include "TokenInfo.hpp"

using Alphabet = std::string;
using CharType = Alphabet::value_type;

template <typename T>
class NFA {
    using MarkedState = std::pair<size_t, bool>;
    using MarkedStateSet = std::pair<std::set<size_t>, bool>;
    public:
        NFA(const Alphabet& alphabet);
        NFA(const std::vector<State<std::vector<T>>>& states,
            const std::map<std::pair<size_t, CharType>, size_t>& characterTransitionTable,
            const Alphabet& alphabet);
        NFA(NFA& other) = delete;
        NFA(NFA&& other) = delete;
        NFA& operator=(NFA& other) = delete;
        NFA& operator=(NFA&& other) = delete;

        void addState(const State<std::vector<T>>& state);
        void addTransition(const State<std::vector<T>>& from, const CharType& character, const State<std::vector<T>>& to);
        void addTransition(const State<std::vector<T>>& from, /*       Empty         */  const State<std::vector<T>>& to);

        void printDebug() const;
        // TODO: make private
        std::set<State<std::vector<T>>> epsilonClosure(const std::set<State<std::vector<T>>>& states) const;
        std::set<State<std::vector<T>>> epsilonClosure(const std::set<size_t>& states) const;
        std::set<size_t> epsilonClosureIndex(const std::set<State<std::vector<T>>>& states) const;
        std::set<size_t> epsilonClosureIndex(const std::set<size_t>& states) const;
        std::set<size_t> computeStartingState() const;
        NFA<T> toDFA() const;

    private:
        Alphabet mAlphabet;

        std::map<std::pair<size_t, CharType>, size_t> mCharacterTransitionTable;
        std::map<size_t, std::vector<size_t>> mEmptyTransitionTable;
        std::vector<State<std::vector<T>>> mStates;

        bool exists(const State<std::vector<T>>& state);
        std::set<size_t> findReachableStates(const std::set<size_t>& startingState,
                                             const CharType& c) const;
        std::vector<State<std::vector<T>>> computeNewStates(const std::vector<MarkedStateSet>& markedStateSetsSet) const;
        static bool isStateMarked(const MarkedState& ms);
};

template<typename T>
NFA<T>::NFA(const Alphabet& alphabet) : mAlphabet{alphabet} {
}

template<typename T>
NFA<T>::NFA(const std::vector<State<std::vector<T>>>& states,
         const std::map<std::pair<size_t, CharType>, size_t>& characterTransitionTable,
         const Alphabet& alphabet) : mCharacterTransitionTable(characterTransitionTable),
        mStates(states), mAlphabet(alphabet) {
}

template<typename T>
void NFA<T>::addState(const State<std::vector<T>>& state) {
    if (exists(state)) {
        throw std::runtime_error("This states already exists");
    }

    mStates.push_back(state);
}

template<typename T>
void NFA<T>::addTransition(const State<std::vector<T>>& from, const CharType& character, const State<std::vector<T>>& to) {
    if (std::find(mAlphabet.begin(), mAlphabet.end(), character) == mAlphabet.end()) {
        throw std::runtime_error("The alphabet must contain the transition character");
    }

    const auto& fromIt = std::find(mStates.begin(), mStates.end(), from);
    if (fromIt == mStates.end()) {
        throw std::runtime_error("The 'from' state must exist");
    }
    size_t fromIndex = std::distance(mStates.begin(), fromIt);

    const auto& toIt = std::find(mStates.begin(), mStates.end(), to);
    if (toIt == mStates.end()) {
        throw std::runtime_error("The 'to' state must exist");
    }
    size_t toIndex = std::distance(mStates.begin(), toIt);

    mCharacterTransitionTable.emplace(std::make_pair(std::make_pair(fromIndex, character), toIndex));
}

template<typename T>
void NFA<T>::addTransition(const State<std::vector<T>>& from, const State<std::vector<T>>& to) {
    const auto& fromIt = std::find(mStates.begin(), mStates.end(), from);
    if (fromIt == mStates.end()) {
        throw std::runtime_error("The 'from' state must exist");
    }
    size_t fromIndex = std::distance(mStates.begin(), fromIt);

    const auto& toIt = std::find(mStates.begin(), mStates.end(), to);
    if (toIt == mStates.end()) {
        throw std::runtime_error("The 'to' state must exist");
    }
    size_t toIndex = std::distance(mStates.begin(), toIt);

    if (mEmptyTransitionTable.find(fromIndex) == mEmptyTransitionTable.end()) {
        mEmptyTransitionTable.emplace(std::make_pair(fromIndex, std::vector({toIndex})));
    } else {
        mEmptyTransitionTable.find(fromIndex)->second.push_back(toIndex);
    }
}

template<typename T>
void NFA<T>::printDebug() const {
    if (!mStates.empty())
        std::cout << "States:" << std::endl;

    for (const State<std::vector<T>>& state : mStates) {
        std::cout << "\t" << state.name;

        if (state.isAccepting)
            std::cout << " (x)";
        else
            std::cout << " ( )";

        if (state.isStarting)
            std::cout << " (x)";
        else
            std::cout << " ( )";
        
        if (!state.payload.empty()) {
            for (const T& e : state.payload) {
                std::cout << " " << e.type;
            }
        }

        std::cout << std::endl;
    }

    if (!mCharacterTransitionTable.empty() || !mEmptyTransitionTable.empty())
        std::cout << "Transitions:" << std::endl;
    
    for (const auto& [key, value] : mCharacterTransitionTable) {
        const size_t& from = key.first;
        const CharType& character = key.second;
        const size_t& to = value;
        std::cout << "\t" << mStates.at(from).name << " <-- " << character << " --> " << mStates.at(to).name << std::endl;
    }

    for (const auto& [key, value] : mEmptyTransitionTable) {
        const size_t& from = key;
        const std::vector<size_t>& toList = value;
        for (const auto& to : toList) {
            std::cout << "\t" << mStates.at(from).name << " <-- ε --> " << mStates.at(to).name << std::endl;
        }
    }
}

template<typename T>
bool NFA<T>::exists(const State<std::vector<T>>& state) {
    return std::find(mStates.begin(), mStates.end(), state) != mStates.end();
}

template<typename T>
std::set<State<std::vector<T>>> NFA<T>::epsilonClosure(const std::set<State<std::vector<T>>>& states) const {
    // This is the set of state that are being explored
    std::vector<MarkedState> markedStatesSet;

    // First we set all the input states as not marked and convert to
    // an index representation
    std::transform(states.begin(), states.end(),
                   std::back_inserter(markedStatesSet),
                   [this](const State<std::vector<T>>& s) {
                       auto it = std::find(mStates.begin(), mStates.end(), s);
                       assert(it != mStates.end());
                       size_t index = std::distance(mStates.begin(), it);
                       return std::make_pair(index, false);
                   });

    // We find the first state that is not marked
    auto it = std::find_if_not(markedStatesSet.begin(), markedStatesSet.end(), isStateMarked);

    // While there are not marked states
    while (it != markedStatesSet.end()) {
        size_t stateIndex = it->first;

        // We get the list or reachable states if it exists
        auto reachableIt = mEmptyTransitionTable.find(stateIndex);
        if (reachableIt != mEmptyTransitionTable.end()) {
            // For each target state
            for (const size_t& toIndex : reachableIt->second) {
                    // We check if it is not already in the marked state set, and insert
                    // it if not
                    auto tempIt = std::find_if(markedStatesSet.begin(), markedStatesSet.end(),
                                               [&toIndex](const MarkedState& ms) { return ms.first == toIndex; });
                    if (tempIt == markedStatesSet.end()) {
                        markedStatesSet.push_back(std::make_pair(toIndex, false));
                    }
                }
        }

        // We set this state as marked and look for another state to explore
        it->second = true;

        it = std::find_if_not(markedStatesSet.begin(), markedStatesSet.end(), isStateMarked);
    }

    // We copy the marked set that is the epsilon-closure of the input states
    std::set<State<std::vector<T>>> closure;
    std::transform(markedStatesSet.begin(), markedStatesSet.end(),
                   std::inserter(closure, closure.begin()),
                   [this](const MarkedState& ms) { return mStates.at(ms.first); });

    return closure;
}

template<typename T>
std::set<State<std::vector<T>>> NFA<T>::epsilonClosure(const std::set<size_t>& states) const {
    // This is the set of state that are being explored
    std::vector<MarkedState> markedStatesSet;

    // First we set all the input states as not marked and convert to
    // an index representation
    std::transform(states.begin(), states.end(), std::back_inserter(markedStatesSet),
                   [](const size_t& index) { return std::make_pair(index, false); });

    // We find the first state that is not marked
    auto it = std::find_if_not(markedStatesSet.begin(), markedStatesSet.end(), isStateMarked);

    // While there are not marked states
    while (it != markedStatesSet.end()) {
        size_t stateIndex = it->first;

        // We get the list or reachable states if it exists
        auto reachableIt = mEmptyTransitionTable.find(stateIndex);
        if (reachableIt != mEmptyTransitionTable.end()) {
            // For each target state
            for (const size_t& toIndex : reachableIt->second) {
                    // We check if it is not already in the marked state set, and insert
                    // it if not
                    auto tempIt = std::find_if(markedStatesSet.begin(), markedStatesSet.end(),
                                               [&toIndex](const MarkedState& ms) { return ms.first == toIndex; });
                    if (tempIt == markedStatesSet.end()) {
                        markedStatesSet.push_back(std::make_pair(toIndex, false));
                    }
                }
        }

        // We set this state as marked and look for another state to explore
        it->second = true;

        it = std::find_if_not(markedStatesSet.begin(), markedStatesSet.end(), isStateMarked);
    }

    // We copy the marked set that is the epsilon-closure of the input states
    std::set<State<std::vector<T>>> closure;
    std::transform(markedStatesSet.begin(), markedStatesSet.end(),
                   std::inserter(closure, closure.begin()),
                   [this](const MarkedState& ms) { return mStates.at(ms.first); });

    return closure;
}

template<typename T>
std::set<size_t> NFA<T>::epsilonClosureIndex(const std::set<State<std::vector<T>>>& states) const {
    // This is the set of state that are being explored
    std::vector<MarkedState> markedStatesSet;

    // First we set all the input states as not marked and convert to
    // an index representation
    std::transform(states.begin(), states.end(),
                   std::back_inserter(markedStatesSet),
                   [this](const State<std::vector<T>>& s) {
                       auto it = std::find(mStates.begin(), mStates.end(), s);
                       assert(it != mStates.end());
                       size_t index = std::distance(mStates.begin(), it);
                       return std::make_pair(index, false);
                   });

    // We find the first state that is not marked
    auto it = std::find_if_not(markedStatesSet.begin(), markedStatesSet.end(), isStateMarked);

    // While there are not marked states
    while (it != markedStatesSet.end()) {
        size_t stateIndex = it->first;

        // We get the list or reachable states if it exists
        auto reachableIt = mEmptyTransitionTable.find(stateIndex);
        if (reachableIt != mEmptyTransitionTable.end()) {
            // For each target state
            for (const size_t& toIndex : reachableIt->second) {
                    // We check if it is not already in the marked state set, and insert
                    // it if not
                    auto tempIt = std::find_if(markedStatesSet.begin(), markedStatesSet.end(),
                                               [&toIndex](const MarkedState& ms) { return ms.first == toIndex; });
                    if (tempIt == markedStatesSet.end()) {
                        markedStatesSet.push_back(std::make_pair(toIndex, false));
                    }
                }
        }

        // We set this state as marked and look for another state to explore
        it->second = true;

        it = std::find_if_not(markedStatesSet.begin(), markedStatesSet.end(), isStateMarked);
    }

    // We copy the marked set that is the epsilon-closure of the input states
    std::set<size_t> closure;
    std::transform(markedStatesSet.begin(), markedStatesSet.end(),
                   std::inserter(closure, closure.begin()),
                   [](const MarkedState& ms) { return ms.first; });

    return closure;
}

template<typename T>
std::set<size_t> NFA<T>::epsilonClosureIndex(const std::set<size_t>& states) const {
    // This is the set of state that are being explored
    std::vector<MarkedState> markedStatesSet;

    // First we set all the input states as not marked and convert to
    // an index representation
    std::transform(states.begin(), states.end(),
                   std::back_inserter(markedStatesSet),
                   [this](const size_t& index) {
                       return std::make_pair(index, false);
                   });

    // We find the first state that is not marked
    auto it = std::find_if_not(markedStatesSet.begin(), markedStatesSet.end(), isStateMarked);

    // While there are not marked states
    while (it != markedStatesSet.end()) {
        size_t stateIndex = it->first;

        // We get the list or reachable states if it exists
        auto reachableIt = mEmptyTransitionTable.find(stateIndex);
        if (reachableIt != mEmptyTransitionTable.end()) {
            // For each target state
            for (const size_t& toIndex : reachableIt->second) {
                    // We check if it is not already in the marked state set, and insert
                    // it if not
                    auto tempIt = std::find_if(markedStatesSet.begin(), markedStatesSet.end(),
                                               [&toIndex](const MarkedState& ms) { return ms.first == toIndex; });
                    if (tempIt == markedStatesSet.end()) {
                        markedStatesSet.push_back(std::make_pair(toIndex, false));
                    }
                }
        }

        // We set this state as marked and look for another state to explore
        it->second = true;

        it = std::find_if_not(markedStatesSet.begin(), markedStatesSet.end(), isStateMarked);
    }

    // We copy the marked set that is the epsilon-closure of the input states
    std::set<size_t> closure;
    std::transform(markedStatesSet.begin(), markedStatesSet.end(),
                   std::inserter(closure, closure.begin()),
                   [this](const MarkedState& ms) { return ms.first; });

    return closure;
}

template<typename T>
std::set<size_t> NFA<T>::computeStartingState() const {
    std::set<State<std::vector<T>>> nfaStartingStates;
    std::copy_if(mStates.begin(), mStates.end(),
                 std::inserter(nfaStartingStates, nfaStartingStates.begin()),
                 [](const State<std::vector<T>>& state) { return state.isStarting; });
    
    return epsilonClosureIndex(nfaStartingStates);
}

template<typename T>
NFA<T> NFA<T>::toDFA() const {
    std::vector<MarkedStateSet> markedStateSetsSet;
    std::map<std::pair<size_t, CharType>, size_t> newCharacterTransitionTable;

    // Compute the starting state
    markedStateSetsSet.push_back(std::make_pair(computeStartingState(), false));
    
    // While there are not marked states
    size_t currentIndex = 0;
    while (currentIndex < markedStateSetsSet.size()) {
        // For each letter of the alphabet
        for (const CharType& c : mAlphabet) {
            // Find the reachable state using the letter c and the current state
            std::set<size_t> newSet = findReachableStates(markedStateSetsSet.at(currentIndex).first, c);
            
            // If there are no reachable state, continue to the next letter
            if (newSet.empty()) {
                continue;
            }

            // Try to find if the current state already is in the state set list
            auto toSetIt = std::find_if(markedStateSetsSet.begin(), markedStateSetsSet.end(),
                                        [&newSet](const MarkedStateSet& ms) { return ms.first == newSet; });
            
            // If not, create it
            if (toSetIt == markedStateSetsSet.end()) {
                markedStateSetsSet.push_back(std::make_pair(newSet, false));
                toSetIt = --markedStateSetsSet.end();
            }

            // Store the transition in the transition table
            size_t toNewStateIndex = std::distance(markedStateSetsSet.begin(), toSetIt);
            newCharacterTransitionTable.insert(std::make_pair(std::make_pair(currentIndex, c), toNewStateIndex));
        }

        // Mark the current state as marked, and go to the next one
        // NOTE: what if somehow, the next state is aleady marked ?
        markedStateSetsSet.at(currentIndex).second = true;
        currentIndex++;
    }

    // Compute the new states from the state sets
    std::vector<State<std::vector<T>>> states = computeNewStates(markedStateSetsSet);

    // Return a NFA which is a DFA
    return NFA(states, newCharacterTransitionTable, mAlphabet);
}

template<typename T>
std::set<size_t> NFA<T>::findReachableStates(const std::set<size_t>& startingState, const CharType& c) const {
    std::set<size_t> newTempSet;
    for (const size_t& fromIndex : startingState) {
        auto transitionIt = mCharacterTransitionTable.find(std::make_pair(fromIndex, c));
        if (transitionIt != mCharacterTransitionTable.end()) {
            newTempSet.insert(transitionIt->second);
        }
    }

    std::set<size_t> newSet = epsilonClosureIndex(newTempSet);

    return newSet;
}

template<typename T>
std::vector<State<std::vector<T>>> NFA<T>::computeNewStates(const std::vector<MarkedStateSet>& markedStateSetsSet) const {
    std::vector<State<std::vector<T>>> states;
    for (const auto& entry : markedStateSetsSet) {
        State<std::vector<T>> s{"S" + std::to_string(states.size())};

        if (states.size() == 0) {
            s.isStarting = true;
        }
        

        if (std::find_if(entry.first.begin(), entry.first.end(),
                         [this](const size_t& index) { return mStates.at(index).isAccepting; }) != entry.first.end()) {
            std::vector<size_t> acceptingStates;
            std::copy_if(entry.first.begin(), entry.first.end(),
                         std::back_inserter(acceptingStates),
                         [this](const size_t& index) { return mStates.at(index).isAccepting; });
            std::vector<T> tokenInfos;
            std::for_each(acceptingStates.begin(), acceptingStates.end(),
                          [&tokenInfos, this](const size_t& acceptingStateIndex) {
                              const State<std::vector<T>>& acceptingState = mStates.at(acceptingStateIndex);
                              std::copy(acceptingState.payload.begin(), acceptingState.payload.end(),
                                        std::back_inserter(tokenInfos));
                          });
            s.isAccepting = true;
            s.payload = tokenInfos;
        }

        states.push_back(s);
    }

    return states;
}

template<typename T>
bool NFA<T>::isStateMarked(const MarkedState& ms) {
    return ms.second;
}

#endif