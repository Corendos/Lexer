#include "NFA.hpp"

#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <cassert>

NFA::NFA(const Alphabet& alphabet) : mAlphabet{alphabet} {
}

NFA::NFA(const std::vector<State>& states,
         const std::map<std::pair<size_t, CharType>, size_t>& characterTransitionTable,
         const Alphabet& alphabet) : mCharacterTransitionTable(characterTransitionTable),
        mStates(states), mAlphabet(alphabet) {
}

void NFA::addState(const State& state) {
    if (exists(state)) {
        throw std::runtime_error("This states already exists");
    }

    mStates.push_back(state);
}

void NFA::addTransition(const State& from, const CharType& character, const State& to) {
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

void NFA::addTransition(const State& from, const State& to) {
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

void NFA::printDebug() const {
    if (!mStates.empty())
        std::cout << "States:" << std::endl;

    for (const State& state : mStates) {
        std::cout << "\t" << state.name;

        if (state.isAccepting)
            std::cout << " (x)";
        else
            std::cout << " ( )";

        if (state.isStarting)
            std::cout << " (x)";
        else
            std::cout << " ( )";
        
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

bool NFA::exists(const State& state) {
    return std::find(mStates.begin(), mStates.end(), state) != mStates.end();
}

std::set<State> NFA::epsilonClosure(const std::set<State>& states) const {
    using MarkedState = std::pair<size_t, bool>;

    // This is the set of state that are being explored
    std::vector<MarkedState> markedStatesSet;

    // First we set all the input states as not marked and convert to
    // an index representation
    std::transform(states.begin(), states.end(),
                   std::back_inserter(markedStatesSet),
                   [this](const State& s) {
                       auto it = std::find(mStates.begin(), mStates.end(), s);
                       assert(it != mStates.end());
                       size_t index = std::distance(mStates.begin(), it);
                       return std::make_pair(index, false);
                   });

    // We find the first state that is not marked
    auto it = std::find_if(markedStatesSet.begin(), markedStatesSet.end(),
                           [](const MarkedState& ms) { return !ms.second; });

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

        it = std::find_if(markedStatesSet.begin(), markedStatesSet.end(),
                          [](const MarkedState& ms) { return !ms.second; });
    }

    // We copy the marked set that is the epsilon-closure of the input states
    std::set<State> closure;
    std::transform(markedStatesSet.begin(), markedStatesSet.end(),
                   std::inserter(closure, closure.begin()),
                   [this](const MarkedState& ms) { return mStates.at(ms.first); });

    return closure;
}

std::set<size_t> NFA::epsilonClosureIndex(const std::set<State>& states) const {
    using MarkedState = std::pair<size_t, bool>;

    // This is the set of state that are being explored
    std::vector<MarkedState> markedStatesSet;

    // First we set all the input states as not marked and convert to
    // an index representation
    std::transform(states.begin(), states.end(),
                   std::back_inserter(markedStatesSet),
                   [this](const State& s) {
                       auto it = std::find(mStates.begin(), mStates.end(), s);
                       assert(it != mStates.end());
                       size_t index = std::distance(mStates.begin(), it);
                       return std::make_pair(index, false);
                   });

    // We find the first state that is not marked
    auto it = std::find_if(markedStatesSet.begin(), markedStatesSet.end(),
                           [](const MarkedState& ms) { return !ms.second; });

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

        it = std::find_if(markedStatesSet.begin(), markedStatesSet.end(),
                          [](const MarkedState& ms) { return !ms.second; });
    }

    // We copy the marked set that is the epsilon-closure of the input states
    std::set<size_t> closure;
    std::transform(markedStatesSet.begin(), markedStatesSet.end(),
                   std::inserter(closure, closure.begin()),
                   [this](const MarkedState& ms) { return ms.first; });

    return closure;
}

std::set<size_t> NFA::computeStartingState() const {
    std::set<State> nfaStartingStates;
    std::copy_if(mStates.begin(), mStates.end(),
                 std::inserter(nfaStartingStates, nfaStartingStates.begin()),
                 [](const State& state) { return state.isStarting; });
    
    return epsilonClosureIndex(nfaStartingStates);
}

NFA NFA::computeNewStates() const {
    using MarkedStateSet = std::pair<std::set<size_t>, bool>;
    std::map<std::pair<size_t, CharType>, size_t> newCharacterTransitionTable;

    std::vector<MarkedStateSet> markedStateSetsSet;
    markedStateSetsSet.push_back(std::make_pair(computeStartingState(), false));

    auto it = std::find_if(markedStateSetsSet.begin(), markedStateSetsSet.end(),
                           [](const MarkedStateSet& ms) { return !ms.second; });
    
    size_t currentNewStateIndex = 0;
    while (it != markedStateSetsSet.end()) {
        for (const CharType& c : mAlphabet) {
            std::set<State> newTempSet;
            for (const size_t& fromIndex : it->first) {
                auto transitionIt = mCharacterTransitionTable.find(std::make_pair(fromIndex, c));
                if (transitionIt != mCharacterTransitionTable.end()) {
                    newTempSet.insert(mStates.at(transitionIt->second));
                }
            }

            std::set<size_t> newSet = epsilonClosureIndex(newTempSet);
            
            if (newSet.empty()) {
                continue;
            }

            auto toSetIt = std::find_if(markedStateSetsSet.begin(), markedStateSetsSet.end(),
                                        [&newSet](const MarkedStateSet& ms) { return ms.first == newSet; });
            if (toSetIt == markedStateSetsSet.end()) {
                markedStateSetsSet.push_back(std::make_pair(newSet, false));
                it = std::find_if(markedStateSetsSet.begin(), markedStateSetsSet.end(),
                                  [](const MarkedStateSet& ms) { return !ms.second; });
                toSetIt = --markedStateSetsSet.end();
            }

            size_t toNewStateIndex = std::distance(markedStateSetsSet.begin(), toSetIt);
            newCharacterTransitionTable.insert(std::make_pair(std::make_pair(currentNewStateIndex, c), toNewStateIndex));
        }

        it->second = true;

        it = std::find_if(markedStateSetsSet.begin(), markedStateSetsSet.end(),
                          [](const MarkedStateSet& ms) { return !ms.second; });
        currentNewStateIndex++;
    }

    std::vector<State> states;
    for (const auto& entry : markedStateSetsSet) {
        State s{"S" + std::to_string(states.size())};

        if (states.size() == 0) {
            s.isStarting = true;
        }

        if (std::find_if(entry.first.begin(), entry.first.end(),
                         [this](const size_t& index) { return mStates.at(index).isAccepting; }) != entry.first.end()) {
            s.isAccepting = true;
        }

        states.push_back(s);
    }

    return NFA(states, newCharacterTransitionTable, mAlphabet);
}