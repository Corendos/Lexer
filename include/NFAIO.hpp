#ifndef __NFA_IO_HPP__
#define __NFA_IO_HPP__

#include <string>

#include "NFA.hpp"

class NFAIO {
    public:
        static NFA loadFromFilename(const std::string& filename);
};

#endif