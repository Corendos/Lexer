#include "Traverser.hpp"

#include <algorithm>
#include <cassert>

Traverser::Traverser(const NFA& nfa) : mNFA(nfa) {
    reset();
}

void Traverser::reset() {
    auto it = std::find_if(mNFA.mStates.begin(), mNFA.mStates.end(),
                           [](const State& state) { return state.isStarting; });
    assert(it != mNFA.mStates.end());

    mCurrentStateIndex = std::distance(mNFA.mStates.begin(), it);
    mReset = true;
}

std::pair<bool, State> Traverser::next(const CharType& character) {
    auto it = mNFA.mCharacterTransitionTable.find(std::make_pair(mCurrentStateIndex, character));
    if (it == mNFA.mCharacterTransitionTable.end()) {
        return std::make_pair(false, State());
    } else {
        mCurrentStateIndex = it->second;
        mReset = false;
        return std::make_pair(true, mNFA.mStates.at(it->second));
    }
}