#include "ship.h"
#include "math.h"
#include "rock.h"
#include "sound.h"
#include "particle.h"
#include "commons.h"

void cShip::thrust(float amount) {
    float oldXSpeed = getSpeed().x;
    float oldYSpeed = getSpeed().y;

    float modXSpeed = cos((getAngle()-90.0)*M_PI/180.0)  * amount;
    float modYSpeed = sin((getAngle()+90.0)*M_PI/180.0) * amount;

    vector2f newSpeed = vector2f(oldXSpeed + modXSpeed, oldYSpeed + modYSpeed);

    setSpeed(newSpeed);
}

bool cShip::checkCollide() {
    if(rck::checkCollide(pos2f(getPos().x, getPos().y))) {
        snd::playSound(snd::SHIP_EXPLODE);

        std::deque<ptl::cParticle> newPtls;
        for(int i = 0; i < 3000; i ++)
        {
            float rndFloat = getRndFloat(0*M_PI/180.0, 360*M_PI/180.0);
            ptl::cParticle newPtl(  getRndFloat(0.01, 0.1),
                                    0.5 + sin(i), 0.5 + cos(i), 0.5 + tan(i),
                                    cVertex(getPos().x + getRndFloat(- 3.0,  3.0), getPos().y + getRndFloat(-  3.0,  3.0),  - 0.5),
                                    cVertex(getSpeed().x + cos(rndFloat+M_PI) * 5, getSpeed().y + sin(rndFloat) * 5, getRndFloat(-2.0, 0.0)),
                                    cVertex(0.0, 0.0, 0.0)
                                );
            newPtls.push_back(newPtl);
        }
        ptl::createParticles(newPtls);

        return true;
    }
    return false;
}

void cShip::dampenSpeed(float amount) {
    float oldXSpeed = getSpeed().x;
    float oldYSpeed = getSpeed().y;

    float oldSignX = oldXSpeed / fabs(oldXSpeed);
    float oldSignY = oldYSpeed / fabs(oldYSpeed);

    float newXSpeed = oldXSpeed - amount * oldSignX;
    float newYSpeed = oldYSpeed - amount * oldSignY;

    float newSignX = newXSpeed / fabs(newXSpeed);
    float newSignY = newYSpeed / fabs(newYSpeed);

    if(newSignX != oldSignX) newXSpeed = 0;
    if(newSignY != oldSignY) newYSpeed = 0;

    vector2f newSpeed = vector2f(newXSpeed, newYSpeed);

    setSpeed(newSpeed);
}
