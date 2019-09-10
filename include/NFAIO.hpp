#ifndef __NFA_IO_HPP__
#define __NFA_IO_HPP__

#include <string>

#include "NFA.hpp"

class NFAIO {
    public:
        static NFA loadFromFilename(const std::string& filename);
        static bool saveToFile(const NFA& nfa, const std::string& filename);
    
    private:
        struct JSONState {
            std::string name;
            bool accepting;
            bool starting;
            std::vector<std::string> payload;
        };
};

#endif