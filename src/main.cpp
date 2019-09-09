#include <iostream>
#include <streambuf>
#include <iterator>
#include <fstream>
#include <chrono>
#include <vector>

#include "NFALoader.hpp"
#include "Lexer.hpp"


int main() {
    std::vector<NFA> nfas = {
        NFALoader::fromFilename("../resources/identifier_lexic.json"),
        NFALoader::fromFilename("../resources/operator_lexic.json"),
        NFALoader::fromFilename("../resources/num_lexic.json"),
        NFALoader::fromFilename("../resources/float_lexic.json")
    };

    NFA combined = NFA::combine(nfas);

    NFA dfa = combined.toDFA();

    dfa.printDebug();

    Lexer lexer(dfa);

    std::string input = "1 + 2 * (3 * (2 - 4))";
    for (const auto& elt : lexer.extractTokens(input)) {
        std::cout << elt.first << "  " << elt.second << std::endl;
    }

    return 0;
}