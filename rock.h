#ifndef __ROCK_H
#define __ROCK_H

#include <string>
#include <deque>

#include "entity.h"

namespace rck {

using std::string;
using std::deque;

class cRock : public cEntity {
    // Funcs
    public:
        int getSize();
        void setSize(int size);

        bool getActive();
        void setActive(bool state);

    // Vars
    private:
        int size;
        bool active;
};

void initRocks();
void setLimits(rect4f limits);
void createRocks(deque<cRock> &newRocks);
void tickRocks(float ticks);
void renderRocks();
GLuint loadTexture(string filename);
bool checkCollide(pos2f pos);
int getLiveRocks();
float closestRockDistance(pos2f pos);

}

#endif
