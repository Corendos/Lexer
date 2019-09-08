#include <iostream>
#include <streambuf>
#include <iterator>
#include <fstream>
#include <chrono>
#include <vector>

#include "NFALoader.hpp"
#include "Lexer.hpp"


int main() {
    NFA floatNFA = NFALoader::fromFilename("../resources/num_lexic.json");


    NFA floatDFA = floatNFA.toDFA();
    floatDFA.printDebug();

    Lexer lexer(floatDFA);

    for (const auto& elt : lexer.extractTokens("1 1 10  \n\t")) {
        std::cout << elt.first << "  " << elt.second << "\n";
    }

    return 0;


    /*NFA nfa = NFALoader::fromFilename("../resources/lexic.json");

    NFA dfa = nfa.toDFA();

    Lexer lexer(dfa);

    std::ifstream fileStream("../resources/main.code");
    std::string stream(std::istreambuf_iterator<char>(fileStream), {});

    bool c = false;
    std::pair<std::string, std::string> token;

    /*for (const auto& elt : lexer.extractTokens(stream)) {
        continue;
    }

    return 0;

    std::tie(c, token) = lexer.next(stream);
    while (c) {
        //output << token.first << "  " << token.second << "\n";
        std::tie(c, token) = lexer.next(stream);
    }*/
}