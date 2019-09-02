#include <iostream>
#include <chrono>
#include <vector>

#include "Lexer.hpp"

int main() {
    Alphabet alphabet{"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_.+-"};
    NFA nfa{alphabet};

    TokenInfo ifTokenInfo;
    ifTokenInfo.priority = 20;
    ifTokenInfo.type = "IF";

    TokenInfo idTokenInfo;
    idTokenInfo.priority = 10;
    idTokenInfo.type = "ID";

    TokenInfo numTokenInfo;
    numTokenInfo.priority = 10;
    numTokenInfo.type = "NUM";

    TokenInfo floatTokenInfo;
    floatTokenInfo.priority = 10;
    floatTokenInfo.type = "FLOAT";

    std::vector<TokenInfo> ifPayload({ifTokenInfo});
    std::vector<TokenInfo> idPayload({idTokenInfo});
    std::vector<TokenInfo> numPayload({numTokenInfo});
    std::vector<TokenInfo> floatPayload({floatTokenInfo});

    State s1{"S1", false, true};
    State s2{"S2"};
    State s3{"S3"};
    State s4{"S4", true, false, ifPayload};
    State s5{"S5"};
    State s6{"S6", true, false, idPayload};
    State s7{"S7"};
    State s8{"S8"};
    State s9{"S9", true, false, numPayload};
    State s10{"S10"};
    State s11{"S11"};
    State s12{"S12", true, false, floatPayload};
    State s13{"S13", true, false, floatPayload};
    State s14{"S14"};
    State s15{"S15", true, false, floatPayload};
    State s16{"S16"};
    State s17{"S17"};
    State s18{"S18", true, false, floatPayload};

    nfa.addState(s1);
    nfa.addState(s2);
    nfa.addState(s3);
    nfa.addState(s4);
    nfa.addState(s5);
    nfa.addState(s6);
    nfa.addState(s7);
    nfa.addState(s8);
    nfa.addState(s9);
    nfa.addState(s10);
    nfa.addState(s11);
    nfa.addState(s12);
    nfa.addState(s13);
    nfa.addState(s14);
    nfa.addState(s15);
    nfa.addState(s16);
    nfa.addState(s17);
    nfa.addState(s18);

    nfa.addTransition(s1, s2);
    nfa.addTransition(s2, 'i', s3);
    nfa.addTransition(s3, 'f', s4);

    nfa.addTransition(s1, s5);
    nfa.addTransitions(s5, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_", s6);
    nfa.addTransitions(s6, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789", s6);

    nfa.addTransition(s1, s7);
    nfa.addTransitions(s7, "+-", s8);
    nfa.addTransition(s7, s8);
    nfa.addTransitions(s8, "0123456789", s9);
    nfa.addTransition(s9, s8);

    nfa.addTransition(s1, s10);
    nfa.addTransitions(s10, "+-", s11);
    nfa.addTransition(s10, s11);
    nfa.addTransitions(s11, "0123456789", s12);
    nfa.addTransitions(s12, "0123456789", s12);
    nfa.addTransition(s12, '.', s13);
    nfa.addTransitions(s13, "0123456789", s13);
    nfa.addTransition(s11, '.', s14);
    nfa.addTransition(s15, s14);
    nfa.addTransitions(s14, "0123456789", s15);
    nfa.addTransitions(s15, "eE", s16);
    nfa.addTransitions(s12, "eE", s16);
    nfa.addTransitions(s13, "eE", s16);
    nfa.addTransitions(s16, "+-", s17);
    nfa.addTransition(s16, s17);
    nfa.addTransitions(s17, "0123456789", s18);
    nfa.addTransition(s18, s17);

    NFA dfa = nfa.toDFA();

    Lexer lexer(dfa);

    std::string input = "3e-";

    for (const auto& elt : lexer.extractTokens(input)) {
        std::cout << elt.first << "  " << elt.second << std::endl;
    }
}