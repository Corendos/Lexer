#ifndef __STATE_HPP__
#define __STATE_HPP__

#include <string>
#include <vector>

#include "TokenInfo.hpp"

/** State Structure
 * Represent a state in an automata
 */
struct State {
    State();
    State(const std::string& name,
          bool accepting = false,
          bool starting = false,
          const std::vector<TokenInfo>& payload = std::vector<TokenInfo>());
    State(const State& other);
    State(const State&& other);
    State& operator=(const State& other);
    State& operator=(const State&& other);

    std::string name;                   /**< The name of the state */
    bool isAccepting;                   /**< Is this state accepting */
    bool isStarting;                    /**< Is this state a starting state */
    std::vector<TokenInfo> payload;     /**< What type of token is represented by this state */
};

bool operator<(const State& a, const State& b);
bool operator==(const State& a, const State& b);

#endif