#ifndef __TRAVERSER_HPP__
#define __TRAVERSER_HPP__

#include "NFA.hpp"


/**
 * The Traverser class. Represents an object that move on a NFA.
 */
class Traverser {
    public:
        /**
         * A constructor.
         * Constructs a Travserser from a NFA.
         */
        Traverser(const NFA& nfa);

        /**
         * A function that resets the traverse to the starting state of the NFA.
         */
        void reset();

        /**
         * A function that returns the next reached state if the transition labelled with 'character'
         * exists.
         * @param character - CharType - The character to look for on transitions.
         */
        std::pair<bool, State> next(const CharType& character);
    
    private:
        const NFA& mNFA;
        size_t mCurrentStateIndex;
        bool mReset;
};

#endif