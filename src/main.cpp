#include <iostream>

#include <NFA.hpp>

int main() {
    Alphabet alphabet{"abc"};
    NFA<TokenInfo> nfa{alphabet};

    std::vector<TokenInfo> tokenInfo1 = {TokenInfo("identifier", 10)};
    std::vector<TokenInfo> tokenInfo2 = {TokenInfo("number", 10)};

    State<std::vector<TokenInfo>> s1("S1", std::vector<TokenInfo>(), false, true);
    State<std::vector<TokenInfo>> s2("S2");
    State<std::vector<TokenInfo>> s3("S3");
    State<std::vector<TokenInfo>> s4("S4", tokenInfo1, true);
    State<std::vector<TokenInfo>> s5("S5");
    State<std::vector<TokenInfo>> s6("S6");
    State<std::vector<TokenInfo>> s7("S7", tokenInfo2, true);
    State<std::vector<TokenInfo>> s8("S8");

    nfa.addState(s1);
    nfa.addState(s2);
    nfa.addState(s3);
    nfa.addState(s4);
    nfa.addState(s5);
    nfa.addState(s6);
    nfa.addState(s7);
    nfa.addState(s8);

    nfa.addTransition(s1, s2);
    nfa.addTransition(s1, s5);
    nfa.addTransition(s2, 'a', s3);
    nfa.addTransition(s3, 'c', s4);
    nfa.addTransition(s5, s6);
    nfa.addTransition(s5, s7);
    nfa.addTransition(s6, 'a', s8);
    nfa.addTransition(s7, 'b', s8);
    nfa.addTransition(s8, s1);

    NFA<TokenInfo> dfa = nfa.toDFA();
    dfa.printDebug();
    return 0;
}