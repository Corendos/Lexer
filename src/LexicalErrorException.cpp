#include "LexicalErrorException.hpp"

LexicalErrorException::LexicalErrorException(const std::string& msg) : std::runtime_error(msg) {} 