#ifndef __NFA_IO_HPP__
#define __NFA_IO_HPP__

#include <string>

#include "NFA.hpp"

/**
 * A helper class. Used to read/write lexics.
 */
class NFAIO {
    public:
        /**
         * A function that reads a lexic from a file.
         * @param filename - std::string - The lexic file name.
         * @return NFA - The NFA described by the lexic file
         */
        static NFA loadFromFilename(const std::string& filename);

        /**
         * A function that writes a lexic to a file.
         * @param nfa - std::string - The lexic file name.
         * @param filename - NFA - The NFA representing the lexic to output.
         * @return bool - Returns true if it succeeded
         */
        static bool saveToFile(const NFA& nfa, const std::string& filename);
};

#endif