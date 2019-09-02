#include "Lexer.hpp"

Lexer::Lexer(const NFA& nfa) : mTraverser(nfa) {

}

std::vector<std::pair<std::string, std::string>> Lexer::extractTokens(const std::string& input) {
    State lastValidState;
    bool hasLastValidState = false;
    bool startedToReadToken = false;
    size_t lastRestartCharacterIndex = 0;

    size_t currentIndex = 0;
    size_t startCharacterIndex = 0;

    std::vector<std::pair<std::string, std::string>> tokens;

    while(currentIndex < input.length()) {
        // Get the next character
        const CharType& c = input.at(currentIndex);

        // Find if there is a transition associated to the current character
        auto [found, state] = mTraverser.next(c);

        if (found) {
            // If we've found a transition, we store that we have started to
            // read characters (Hack to handle spaces. It may be useless in the future)
            startedToReadToken = true;
            // If the state is  accepting, we store it and set the variable telling where
            // to start from if the token is added to the list of tokens
            if (state.isAccepting) {
                lastRestartCharacterIndex = currentIndex + 1;
                lastValidState = state;
                hasLastValidState = true;
            }
            // We go to the next character
            currentIndex++;

            // If we reached the end of the input, we need to add the last valid token
            // to the list of tokens (if it exists)
            if (currentIndex == input.length()) {
                assert(hasLastValidState);
                std::string newToken(input, startCharacterIndex, lastRestartCharacterIndex - startCharacterIndex);
                std::string tokenType;
                size_t priority{0};
                for (const auto& e : lastValidState.payload) {
                    if (e.priority > priority) {
                        priority = e.priority;
                        tokenType = e.type;
                    }
                }
                tokens.push_back(std::make_pair(newToken, tokenType));
                currentIndex = lastRestartCharacterIndex;
                startCharacterIndex = lastRestartCharacterIndex;
                hasLastValidState = false;
                startedToReadToken = false;
                mTraverser.reset();
            }
        } else {
            if (startedToReadToken) {
                assert(hasLastValidState);
            } else {
                currentIndex++;
                startCharacterIndex++;
                continue;
            }
            //TODO: what if the string is empty ? If the first transition does not exist ?
            std::string newToken(input, startCharacterIndex, lastRestartCharacterIndex - startCharacterIndex);
            std::string tokenType;
            size_t priority{0};
            for (const auto& e : lastValidState.payload) {
                if (e.priority > priority) {
                    priority = e.priority;
                    tokenType = e.type;
                }
            }
            tokens.push_back(std::make_pair(newToken, tokenType));
            currentIndex = lastRestartCharacterIndex;
            startCharacterIndex = lastRestartCharacterIndex;
            hasLastValidState = false;
            startedToReadToken = false;
            mTraverser.reset();
        }
    }

    return tokens;
}