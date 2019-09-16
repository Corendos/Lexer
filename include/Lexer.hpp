#ifndef __LEXER_HPP__
#define __LEXER_HPP__

#include <vector>
#include <string>

#include "NFA.hpp"
#include "Traverser.hpp"

/**
 * A lexer class. Represent a lexer.
 */
class Lexer {
    public:
        /**
         * A constructor.
         * Constructs a lexer from a NFA representing the detected lexic.
         */
        Lexer(const NFA& nfa);

        /**
         * A function that extracts token from the given input and returns a list of tokens.
         * @param input a std::string representing the input text
         */
        std::vector<std::pair<std::string, std::string>> extractTokens(const std::string& input);

        /**
         * A function that extracts the next token from the input.
         * @param stream a std::string representing the input text
         */
        std::pair<bool, std::pair<std::string, std::string>> next(const std::string& stream);

    private:
        Traverser mTraverser;       //< A helper class that traverse the nfa graph
        State mLastValidState;      //< The last detected valid state
        bool mHasLastValidState;    //< A boolean indicating if the lexer has found a valid state
        size_t mLastStartPosition;  //< An index representing the position where to restart after having returned a token
        size_t mCurrentPosition;    //< An index representing the current position in the input stream
        size_t mStartPosition;      //< An index representing the position where the current read token started
        std::string mTempBuffer;    //< A temporary buffer to store the read token

        /**
         * A function that return the last detected token using the various indices
         * @param input a std::string representing the input text
         */
        std::pair<std::string, std::string> getLastToken(const std::string& input);
};

#endif