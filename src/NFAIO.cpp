#include "NFAIO.hpp"

#include <fstream>
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