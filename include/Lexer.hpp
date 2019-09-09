#ifndef __LEXER_HPP__
#define __LEXER_HPP__

#include <vector>
#include <string>

#include "NFA.hpp"
#include "Traverser.hpp"

class Lexer {
    public:
        Lexer(const NFA& nfa);

        std::vector<std::pair<std::string, std::string>> extractTokens(const std::string& input);
        std::pair<bool, std::pair<std::string, std::string>> next(const std::string& stream);

    private:
        Traverser mTraverser;
        State mLastValidState;
        bool mHasLastValidState;
        bool mStartedToReadToken;
        size_t mLastStartPosition;
        size_t mCurrentPosition;
        size_t mStartPosition;
        std::string mTempBuffer;

        std::pair<std::string, std::string> getLastToken(const std::string& input);
};

#endif