#ifndef RIGHTLEG_H
#define RIGHTLEG_H

#include "Leg.h"

class RightLeg : public Leg {
    public:
        RightLeg (float width, float height);

        void animateDownwards ();

    private:
        bool down;
        float theta;
};

#endif
