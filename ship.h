#ifndef __SHIP_H
#define __SHIP_H

#include "entity.h"

class cShip : public cEntity {
    // Funcs
    public:
        void thrust(float amount);
        void dampenSpeed(float amount);
        bool checkCollide();
};

#endif
