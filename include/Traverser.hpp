#ifndef __TRAVERSER_HPP__
#define __TRAVERSER_HPP__

#include "NFA.hpp"

class Traverser {
    public:
        Traverser(const NFA& nfa);

        void reset();
        std::pair<bool, State> next(const CharType& character);

        bool isReset() const;
    
    private:
        const NFA& mNFA;
        size_t mCurrentStateIndex;
        bool mReset;
};

#endif