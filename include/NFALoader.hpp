#ifndef __NFA_LOADER_HPP__
#define __NFA_LOADER_HPP__

#include <string>

#include "NFA.hpp"

class NFALoader {
    public:
        static NFA fromFilename(const std::string& filename);
};

#endif