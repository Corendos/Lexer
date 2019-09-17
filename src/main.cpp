#include <iostream>
#include <streambuf>
#include <iterator>
#include <fstream>
#include <chrono>
#include <vector>

#include "NFAIO.hpp"
#include "Lexer.hpp"


int main() {
    NFA numLexic = NFAIO::loadFromFilename("../resources/num_lexic.json");
    NFA floatLexic = NFAIO::loadFromFilename("../resources/float_lexic.json");
    std::vector<NFA> nfas = {
        NFAIO::loadFromFilename("../resources/identifier_lexic.json"),
        NFAIO::loadFromFilename("../resources/operator_lexic.json"),
        numLexic,
        NFAIO::loadFromFilename("../resources/float_lexic.json")
    };

    NFA combined = NFA::combine(nfas);

    NFA dfa = combined.toDFA();

    dfa.printDebug();

    NFAIO::saveToFile(dfa, "../resources/final.json");

    Lexer lexer(dfa);

    std::string input = "1 + 2 * (3e-2 * (2 -& 4))";
    for (const auto& elt : lexer.extractTokens(input)) {
        std::cout << elt.first << "  " << elt.second << std::endl;
    }

    return 0;
}