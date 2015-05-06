#include "entity.h"

cEntity::cEntity() {

}

pos2f cEntity::getPos() {
    return this->pos;
}

void cEntity::setPos(pos2f pos) {
    this->pos = pos;
}

void cEntity::limitPos(rect4f limits) {
    this->posLimits = limits;
}

rect4f cEntity::getLimits() {
    return this->posLimits;
}


void cEntity::updatePos(float ticks) {
    this->pos = pos2f(this->pos.x + this->speed.x * ticks, this->pos.y + this->speed.y * ticks);

    if(this->pos.x < this->posLimits.x1) this->pos.x = this->posLimits.x2;
    if(this->pos.y < this->posLimits.y1) this->pos.y = this->posLimits.y2;
    if(this->pos.x > this->posLimits.x2) this->pos.x = this->posLimits.x1;
    if(this->pos.y > this->posLimits.y2) this->pos.y = this->posLimits.y1;
}

float cEntity::getAngle() {
    return this->angle;
}

void cEntity::setAngle(float angle) {
    this->angle = angle;
}

void cEntity::turn(int direction, float amount) {
    switch(direction) {
        case LEFT:
            this->angle -= amount;
            break;
        case RIGHT:
            this->angle += amount;
            break;
    }
}

cVertex cEntity::getAngleXYZ() {
    return this->angleXYZ;
}

void cEntity::setAngleXYZ(cVertex angleXYZ) {
    this->angleXYZ = angleXYZ;
}

vector2f cEntity::getSpeed() {
    return this->speed;
}

void cEntity::setSpeed(vector2f speed) {
    this->speed = speed;
}
