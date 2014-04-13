#include "LeftThigh.h"
#include "LeftLeg.h"
#include "Dimensions.h"

LeftThigh::LeftThigh (float width, float height) : Leg(width, height) {
    LeftLeg *leftLeg = new LeftLeg(WIDTH_LEG, HEIGHT_LEG);
    addChild("leftLeg", leftLeg);
    translateChild("leftLeg", 0, -(WIDTH_TRUNK - getWidth()) / 2, -1.5 * HEIGHT_TRUNK -getHeight());
}

