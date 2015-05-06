#ifndef __PROJECTILE_H
#define __PROJECTILE_H

#include <string>
#include <deque>

#include "entity.h"

namespace prjctl {

using std::string;
using std::deque;

class cProjectile : public cEntity {
    // Funcs
    public:
        float getTtl();
        void setTtl(float ttl);

        bool getActive();
        void setActive(bool state);

    // Vars
    private:
        float ttl;
        bool active;
};

void initProjectiles();
void setLimits(rect4f limits);
void createProjectiles(deque<cProjectile> &newProjectiles);
void tickProjectiles(float ticks);
void renderProjectiles();
GLuint loadTexture(string filename);

}

#endif
