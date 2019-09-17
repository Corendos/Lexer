#ifndef __STATE_HPP__
#define __STATE_HPP__

#include <string>
#include <vector>

#include "TokenInfo.hpp"

/**
 * State structure
 * Represents a state in an automaton
 */
struct State {
    /**
     * A constructor.
     * Constructs a default state.
     */
    State();

    /**
     * A constructor.
     * Constructs a state with the specified parameters.
     * @param name - std::string - The state name.
     * @param accepting - bool - Is the state accepting
     * @param starting - bool - Is is a starting state
     * @param payload - std::vector<TokenInfo> - The list of tokens that the state represents
     */
    State(const std::string& name,
          bool accepting = false,
          bool starting = false,
          const std::vector<TokenInfo>& payload = std::vector<TokenInfo>());
    
    /**
     * A copy constructor.
     * Constructs a State using another State.
     * @param other - State - The other State.
     */
    State(const State& other);
    
    /**
     * A move constructor.
     * Constructs a State by moving another State.
     * @param other - State - The other State.
     */
    State(const State&& other);

    /**
     * A copy assignment operator.
     * Assigns a State to the current State by copying it.
     * @param other - State - The other State.
     * @return a reference to the current State.
     */
    State& operator=(const State& other);

    /**
     * A move assignment operator.
     * Assigns a State to the current State by moving it.
     * @param other - State - The other State.
     * @return a reference to the current State.
     */
    State& operator=(const State&& other);

    std::string name;                   /**< The name of the state */
    bool isAccepting;                   /**< Is this state accepting */
    bool isStarting;                    /**< Is this state a starting state */
    std::vector<TokenInfo> payload;     /**< What type of token is represented by this state */
};


/**
 * The less than comparison operator.
 * Compare two states by comparing their name.
 * @param a - State - The first State.
 * @param b - State - The second State.
 * @return a bool indicating if a is less than b.
 */
bool operator<(const State& a, const State& b);

/**
 * The equality operator.
 * Compare two states by comparing their name.
 * @param a - State - The first State.
 * @param b - State - The second State.
 * @return a bool indicating if a is equal to b.
 */
bool operator==(const State& a, const State& b);

#endif