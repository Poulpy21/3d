#ifndef RIGHTFOREARM_H
#define RIGHTFOREARM_H

#include "Arm.h"

class RightForeArm : public Arm {
    public:
        RightForeArm (float width, float height);

        void drawDownwards(const float *currentTransformationMatrix);
        void animateDownwards();

    private:
        bool down;
        float theta;
};

#endif
