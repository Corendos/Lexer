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

    private:
        Traverser mTraverser;
};

#endif