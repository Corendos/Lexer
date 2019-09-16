#include "NFAIO.hpp"

#include <fstream>
#include <iomanip>
#include "json.hpp"

using json = nlohmann::json;

NFA NFAIO::loadFromFilename(const std::string& filename) {
    std::ifstream fileStream(filename);
    json lexicJson;
    fileStream >> lexicJson;
    fileStream.close();

    // Get the alphabet
    Alphabet alphabet = std::move(lexicJson["alphabet"].get<std::string>());

    // Get the tokens mapping
    std::map<std::string, TokenInfo> tokensInfoMap;
    std::transform(lexicJson["tokensInfo"].begin(), lexicJson["tokensInfo"].end(),
                   std::inserter(tokensInfoMap, tokensInfoMap.begin()), [](const json& e) {
                       return std::make_pair(
                           e["name"].get<std::string>(),
                           TokenInfo{e["type"].get<std::string>(), e["priority"].get<int>()});
                   });

    NFA nfa(alphabet);

    std::for_each(lexicJson["states"].begin(), lexicJson["states"].end(),
                  [&tokensInfoMap, &nfa](const json& data) {
                       std::vector<TokenInfo> payload(data["payload"].get<std::vector<std::string>>().size());
                       std::vector<std::string> tokens = std::move(data["payload"].get<std::vector<std::string>>());
                       std::transform(tokens.begin(), tokens.end(),
                                      payload.begin(), [&tokensInfoMap](const std::string& name) {
                                          return tokensInfoMap.at(name);
                                      });
                       nfa.addState(State(
                           data["name"].get<std::string>(),
                           data["accepting"].get<bool>(),
                           data["starting"].get<bool>(),
                           payload));
                  });
    
    std::for_each(lexicJson["transitions"].begin(), lexicJson["transitions"].end(),
                  [&nfa](const json& e) {
                      std::string characters = std::move(e["characters"].get<std::string>());
                      std::string from = std::move(e["from"].get<std::string>());
                      std::string to = std::move(e["to"].get<std::string>());
                      if (characters.empty()) {
                          nfa.addTransition(from, to);
                      } else if (characters.size() == 1) {
                          nfa.addTransition(from, characters[0], to);
                      } else {
                          nfa.addTransitions(from, characters, to);
                      }
                  });
    
    return nfa;
}

bool NFAIO::saveToFile(const NFA& nfa, const std::string& filename) {
    json output;

    output["alphabet"] = nfa.mAlphabet;

    // Create the states
    std::vector<json> jsonStates(nfa.mStates.size());
    std::transform(nfa.mStates.begin(), nfa.mStates.end(),
                   jsonStates.begin(),
                   [](const State& state) {
                       std::vector<std::string> payload(state.payload.size());
                       std::transform(state.payload.begin(), state.payload.end(),
                                      payload.begin(), 
                                      [](const TokenInfo& tokenInfo) {
                                          return tokenInfo.type;
                                      });
                       json jsonState;
                       jsonState["name"] = state.name;
                       jsonState["accepting"] = state.isAccepting;
                       jsonState["starting"] = state.isStarting;
                       jsonState["payload"] = std::move(payload);
                       return std::move(jsonState);
                   });
    
    output["states"] = jsonStates;

    // Create the mapping between the states indices and their real name
    std::map<size_t, std::string> statesNameMapping;
    for (size_t i{0};i < nfa.mStates.size();++i) {
        statesNameMapping.insert(std::make_pair(i, nfa.mStates.at(i).name));
    }

    // Create the transitions (non empty)
    std::map<std::pair<size_t, size_t>, std::string> transitions;
    std::for_each(nfa.mCharacterTransitionTable.begin(), nfa.mCharacterTransitionTable.end(),
                  [&transitions](const std::pair<std::pair<size_t, CharType>, size_t>& entry) {
                      std::pair<size_t, size_t> key = std::make_pair(entry.first.first, entry.second);
                      auto it = transitions.find(key);
                      if (it == transitions.end()) {
                          bool inserted;
                          std::tie(it, inserted) = transitions.insert(std::make_pair(key, std::string()));
                      }
                      it->second.push_back(entry.first.second);
                  });

    // Create the empty transitions
    std::vector<std::pair<size_t, size_t>> emptyTransitions;
    std::for_each(nfa.mEmptyTransitionTable.begin(), nfa.mEmptyTransitionTable.end(),
                  [&emptyTransitions](const std::pair<size_t, std::vector<size_t>>& entry) {
                      std::for_each(entry.second.begin(), entry.second.end(),
                                    [&emptyTransitions, &entry](const size_t& to) {
                                        emptyTransitions.push_back(std::make_pair(entry.first, to));
                                    });
                  });
    
    // Combine them in a single array
    std::vector<json> jsonTransitions(transitions.size() + emptyTransitions.size());
    std::transform(transitions.begin(), transitions.end(),
                   jsonTransitions.begin(), 
                   [&statesNameMapping](const std::pair<std::pair<size_t, size_t>, std::string>& entry) {
                       json transition;
                       transition["from"] = statesNameMapping.at(entry.first.first);
                       transition["to"] = statesNameMapping.at(entry.first.second);
                       transition["characters"] = std::move(entry.second);
                       return std::move(transition);
                   });

    std::transform(emptyTransitions.begin(), emptyTransitions.end(),
                   jsonTransitions.begin() + transitions.size(), 
                   [&statesNameMapping](const std::pair<size_t, size_t>& entry) {
                       json transition;
                       transition["from"] = statesNameMapping.at(entry.first);
                       transition["to"] = statesNameMapping.at(entry.second);
                       transition["characters"] = "";
                       return std::move(transition);
                   });
    
    output["transitions"] = jsonTransitions;

    // Create the list of tokens infos
    std::vector<json> tokensInfo;
    std::set<std::string> tokensNames;
    std::for_each(nfa.mStates.begin(), nfa.mStates.end(),
                  [&tokensInfo, &tokensNames](const State& state) {
                      std::for_each(state.payload.begin(), state.payload.end(),
                                    [&tokensInfo, &tokensNames](const TokenInfo& tokenInfo) {
                                        if (tokensNames.find(tokenInfo.type) == tokensNames.end()) {
                                            tokensNames.insert(tokenInfo.type);
                                            json jsonTokenInfo;
                                            jsonTokenInfo["type"] = tokenInfo.type;
                                            jsonTokenInfo["priority"] = tokenInfo.priority;
                                            tokensInfo.push_back(std::move(jsonTokenInfo));
                                        }
                                    });
                  });
    
    output["tokensInfo"] = tokensInfo;

    std::ofstream outputStream(filename);
    outputStream << std::setw(4) << output;

    outputStream.close();

    return true;
}