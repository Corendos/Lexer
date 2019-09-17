#ifndef __LEXICAL_ERROR_EXCEPTION_HPP__
#define __LEXICAL_ERROR_EXCEPTION_HPP__

#include <stdexcept>

/**
 * An exception class. Thrown when a lexical error has been detected
 */
class LexicalErrorException : public std::runtime_error {
    public:
        /**
         * A constructor.
         * Constructs the error with the given message.
         */
        LexicalErrorException(const std::string& msg = "");
};

#endif