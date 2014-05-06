#include "SeaDiver.h"

#include "globals.h"

#include "Trunk.h"
#include "Pipe.h"

#include <iostream>

SeaDiver::SeaDiver() : RenderTree(), t(0), pos(0, 0, 0) {
    Trunk *trunk = new Trunk(WIDTH_TRUNK, HEIGHT_TRUNK, DEPTH_TRUNK);
    addChild("trunk", trunk);

    std::vector<Vec> tmp;
    tmp.push_back(Vec(PIPE_FIXED_PART_X, PIPE_FIXED_PART_Y, PIPE_FIXED_PART_Z));
    tmp.push_back(Vec(0, 2, 4));
    tmp.push_back(Vec(0, 1, 1));
    tmp.push_back(Vec(0, -1, -0.5));

    Pipe *pipe = new Pipe(tmp);
    addChild("tuyau", pipe);

    // Définition de la trajectoire du plongeur
    // cercle ici
    std::vector<Vec> trajectoire;
    for (float f = 0; f <= 2 * M_PI; f += 0.1) {
        float x = 6 * cos(f);
        float y = 6 * sin(f);
        trajectoire.push_back(Vec(x, y, 0));
    }

    cs = new CardinalSpline(trajectoire);
}

void SeaDiver::drawDownwards(const float *currentTransformationMatrix) {
    glPushMatrix();

    glMultTransposeMatrixf(relativeModelMatrix);
}

void SeaDiver::drawUpwards(const float *currentTransformationMatrix) {
    glPopMatrix();
}

void SeaDiver::animateDownwards() {
    Vec newPos = (*cs)(t);
    Vec offset =  newPos - pos;
    pos = newPos;

    translate(offset);
    /* t += Globals::dt; */
    t += 0.1;
}

// Events
void SeaDiver::keyPressEvent(QKeyEvent* e) {
    // TODO
}

void SeaDiver::mouseMoveEvent(QMouseEvent* e) {
    // TODO
}

SeaDiver::~SeaDiver () {
    delete cs;
}

