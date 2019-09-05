#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>

#include "Lexer.hpp"

int main() {
    Alphabet alphabet{"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_.+-"};

    NFA nfa = NFA::fromFile("../resources/lexic.json");

    NFA dfa = nfa.toDFA();

    Lexer lexer(dfa);

    std::ifstream stream("../resources/main.code");
    std::string input;
    stream.seekg(0, std::ios::end);
    input.reserve(stream.tellg());
    stream.seekg(0, std::ios::beg);

    input.assign(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());

    std::ofstream output("output.txt");

    for (const auto& elt : lexer.extractTokens(input)) {
        output << elt.first << "  " << elt.second << "\n";
    }
}