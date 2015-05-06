#ifndef __H_ENTITY
#define __H_ENTITY

#include "metrics.h"

class cEntity {

    //Funcs
    public:
        cEntity();

        pos2f getPos();
        void setPos(pos2f pos);
        void updatePos(float ticks);
        void limitPos(rect4f limits);
        rect4f getLimits();

        float getAngle();
        void setAngle(float angle);
        void turn(int direction, float amount);

        cVertex getAngleXYZ();
        void setAngleXYZ(cVertex angleXYZ);

        vector2f getSpeed();
        void setSpeed(vector2f speed);

    // Vars
    private:
        pos2f pos;
        rect4f posLimits;
        float angle;
        vector2f speed;
        cVertex angleXYZ;

};

#endif
