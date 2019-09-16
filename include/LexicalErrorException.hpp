#ifndef __LEXICAL_ERROR_EXCEPTION_HPP__
#define __LEXICAL_ERROR_EXCEPTION_HPP__

#include <stdexcept>

class LexicalErrorException : public std::runtime_error {
    public:
        LexicalErrorException(const std::string& msg = "");
};

#endif