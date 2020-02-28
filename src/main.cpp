#include <iostream>
#include <streambuf>
#include <iterator>
#include <fstream>
#include <chrono>
#include <vector>

#include "NFAIO.hpp"
#include "Lexer.hpp"


int main() {
    std::vector<NFA> nfas = {
        NFAIO::loadFromFilename("../resources/identifier_lexic.json"),
        NFAIO::loadFromFilename("../resources/operator_lexic.json"),
        NFAIO::loadFromFilename("../resources/num_lexic.json"),
        NFAIO::loadFromFilename("../resources/float_lexic.json")
    };

    NFA combined = NFA::combine(nfas);

    NFA dfa = combined.toDFA();

    NFAIO::saveToFile(dfa, "../resources/final.json");

    Lexer lexer(dfa);

    std::string input = "1 + 2 * (3e-2 * (2 - 4))";

    std::cout << "The input string is: " << input << std::endl;

    std::cout << "Extracted tokens:" << std::endl;

    for (const auto& elt : lexer.extractTokens(input)) {
        std::cout << elt.first << "  " << elt.second << std::endl;
    }

    return 0;
}