#include "Lexer.hpp"

Lexer::Lexer(const NFA& nfa) :
    mTraverser(nfa), mHasLastValidState(false), mStartedToReadToken(false),
    mLastStartPosition(0), mCurrentPosition(0), mStartPosition(0) {
    mTempBuffer.reserve(1000);
}

std::vector<std::pair<std::string, std::string>> Lexer::extractTokens(const std::string& input) {
    std::vector<std::pair<std::string, std::string>> tokens;

    while(mCurrentPosition < input.length()) {
        // Get the next character
        const CharType& c = input.at(mCurrentPosition);

        // Find if there is a transition associated to the current character
        auto [found, state] = mTraverser.next(c);

        if (found) {
            // If the state is  accepting, we store it and set the variable telling where
            // to start from if the token is added to the list of tokens
            if (state.isAccepting) {
                mLastStartPosition = mCurrentPosition + 1;
                mLastValidState = state;
                mHasLastValidState = true;
            }
            // We go to the next character
            mCurrentPosition++;

            // If we reached the end of the input, we need to add the last valid token
            // to the list of tokens (if it exists)
            if (mCurrentPosition == input.length()) {
                assert(mHasLastValidState);
                std::string newToken(input, mStartPosition, mLastStartPosition - mStartPosition);
                std::string tokenType;
                size_t priority{0};
                for (const auto& e : mLastValidState.payload) {
                    if (e.priority > priority) {
                        priority = e.priority;
                        tokenType = e.type;
                    }
                }
                tokens.push_back(std::make_pair(newToken, tokenType));
                mCurrentPosition = mLastStartPosition;
                mStartPosition = mLastStartPosition;
                mHasLastValidState = false;
                mTraverser.reset();
            }
        } else {
            if (c == ' ' || c == '\n') {
                if (mHasLastValidState) {
                    //TODO: what if the string is empty ? If the first transition does not exist ?
                    std::string newToken(input, mStartPosition, mLastStartPosition - mStartPosition);
                    std::string tokenType;
                    size_t priority{0};
                    for (const auto& e : mLastValidState.payload) {
                        if (e.priority > priority) {
                            priority = e.priority;
                            tokenType = e.type;
                        }
                    }
                    tokens.push_back(std::make_pair(newToken, tokenType));
                    mCurrentPosition = mLastStartPosition;
                    mStartPosition = mLastStartPosition;
                    mHasLastValidState = false;
                    mTraverser.reset();
                } else {
                    mCurrentPosition++;
                    mStartPosition++;
                }
            } else {
                assert(mHasLastValidState);
                //TODO: what if the string is empty ? If the first transition does not exist ?
                std::string newToken(input, mStartPosition, mLastStartPosition - mStartPosition);
                std::string tokenType;
                size_t priority{0};
                for (const auto& e : mLastValidState.payload) {
                    if (e.priority > priority) {
                        priority = e.priority;
                        tokenType = e.type;
                    }
                }
                tokens.push_back(std::make_pair(newToken, tokenType));
                mCurrentPosition = mLastStartPosition;
                mStartPosition = mLastStartPosition;
                mHasLastValidState = false;
                mTraverser.reset();
            }
        }
    }

    return tokens;
}
//*
std::pair<bool, std::pair<std::string, std::string>> Lexer::next(const std::string& stream) {
    bool stateFound = false;
    char c;
    std::pair<std::string, std::string> returnedValue;

    do {
        if (mCurrentPosition == stream.size()) {
            if (!mHasLastValidState) {
                return std::make_pair(false, std::make_pair("", ""));
            }

            //TODO: what if the string is empty ? If the first transition does not exist ?
            std::string newToken = mTempBuffer.substr(0, mLastStartPosition - mStartPosition);
            std::string tokenType;
            size_t priority{0};
            for (const auto& e : mLastValidState.payload) {
                if (e.priority > priority) {
                    priority = e.priority;
                    tokenType = e.type;
                }
            }
            returnedValue = std::make_pair(newToken, tokenType);
            mCurrentPosition = mLastStartPosition;
            mStartPosition = mLastStartPosition;
            mHasLastValidState = false;
            mStartedToReadToken = false;
            mTraverser.reset();
            mTempBuffer.clear();
            stateFound = true;
            continue;
        }

        c = stream[mCurrentPosition];

        mTempBuffer.push_back(c);

        auto [found, state] = mTraverser.next(c);

        if (found) {
            mStartedToReadToken = true;

            if (state.isAccepting) {
                mLastStartPosition = mCurrentPosition + 1;
                mLastValidState = state;
                mHasLastValidState = true;
            }

            mCurrentPosition++;
        } else {
            if (mStartedToReadToken) {
                assert(mHasLastValidState);

                //TODO: what if the string is empty ? If the first transition does not exist ?
                std::string newToken = mTempBuffer.substr(0, mLastStartPosition - mStartPosition);
                std::string tokenType;
                size_t priority{0};
                for (const auto& e : mLastValidState.payload) {
                    if (e.priority > priority) {
                        priority = e.priority;
                        tokenType = e.type;
                    }
                }
                returnedValue = std::make_pair(newToken, tokenType);
                mCurrentPosition = mLastStartPosition;
                mStartPosition = mLastStartPosition;
                mHasLastValidState = false;
                mStartedToReadToken = false;
                mTraverser.reset();
                mTempBuffer.clear();
                stateFound = true;
            } else {
                mTempBuffer.clear();
                mCurrentPosition++;
                mStartPosition++;
            }
        }
    } while(!stateFound);

    return std::make_pair(true, returnedValue);
}//*/