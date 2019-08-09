#include <iostream>

#include <NFA.hpp>

int main() {
    Alphabet alphabet{"abcd"};
    NFA nfa{alphabet};

    State s1("S1", false, true);
    State s2("S2");
    State s3("S3");
    State s4("S4", true);
    State s5("S5");
    State s6("S6");
    State s7("S7");
    State s8("S8");

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

    nfa.printDebug();

    std::set<State> statesSet;
    statesSet.insert(s5);
    statesSet.insert(s2);

    nfa.computeNewStates();
    return 0;
}