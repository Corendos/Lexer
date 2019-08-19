#include "Lexer.hpp"

Lexer::Lexer(const NFA& nfa) : mTraverser(nfa) {

}

std::vector<std::string> Lexer::extractTokens(const std::string& input) {
    State lastValidState;
    bool hasLastValidState = false;
    bool startedToReadToken = false;
    size_t lastRestartCharacterIndex = 0;

    size_t currentIndex = 0;
    size_t startCharacterIndex = 0;

    std::vector<std::string> tokens;

    while(currentIndex < input.length()) {
        const CharType& c = input.at(currentIndex);
        auto [found, state] = mTraverser.next(c);
        if (found) {
            startedToReadToken = true;
            if (state.isAccepting) {
                lastRestartCharacterIndex = currentIndex + 1;
                lastValidState = state;
                hasLastValidState = true;
            }
            currentIndex++;

            if (currentIndex == input.length()) {
                assert(state.isAccepting);
                std::string newToken = std::string(input, startCharacterIndex, lastRestartCharacterIndex - startCharacterIndex);
                tokens.push_back(newToken);
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
            std::string newToken = std::string(input, startCharacterIndex, lastRestartCharacterIndex - startCharacterIndex);
            tokens.push_back(newToken);
            currentIndex = lastRestartCharacterIndex;
            startCharacterIndex = lastRestartCharacterIndex;
            hasLastValidState = false;
            startedToReadToken = false;
            mTraverser.reset();
        }
    }

    return tokens;
}