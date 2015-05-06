#include "rock.h"
#include "settings.h"
#include "model.h"
#include "math.h"
#include "commons.h"
#include "particle.h"
#include "sound.h"
#include "main.h"

namespace rck {

    int cRock::getSize() {
        return this->size;
    }

    void cRock::setSize(int size) {
        this->size = size;
    }

    bool cRock::getActive() {
        return this->active;
    }

    void cRock::setActive(bool state) {
        this->active = state;
    }

    using std::deque;

    deque<cRock> rocks;
    GLuint texId;
    int liveRocks = 100;
    rect4f limits;
    cModel mdl;

    void initRocks()
    {
        rocks.resize(OPT_NUM_INITIAL_PRJCTLS);
        deque<cRock>::iterator ptlIter;
        for( ptlIter = rocks.begin(); ptlIter != rocks.end(); ptlIter ++ )
        {
            ptlIter->setActive(false);
        }

        /*cModel dummyModel;
        texId = dummyModel.loadTexture(OPT_PATH_TEXTURES + (string)"rock" + OPT_PATHEXT_TEXTURES);*/
        mdl.loadFile("rock");
    }

    void setLimits(rect4f newlimits) {
        limits = newlimits;
    }

    void createRocks(deque<cRock> &newRocks)
    {
        deque<cRock>::iterator newPtlIter = newRocks.begin();

        // Check for unused spots
        deque<cRock>::iterator ptlIter;
        for( ptlIter = rocks.begin(); ptlIter != rocks.end(); ptlIter ++ )
        {
            if(! ptlIter->getActive())
            {
                *ptlIter = *newPtlIter;
                ptlIter->setActive(true);
                ptlIter->limitPos(limits);
                newPtlIter ++;
                if(newPtlIter == newRocks.end()) return;
            }
        }

        if((newPtlIter != newRocks.end()) && (liveRocks < OPT_MAX_PRJCTLS))
        {
            // Didn't find unused spot, so create them
            for( newPtlIter = newPtlIter; newPtlIter != newRocks.end(); newPtlIter ++ )
            {
                rocks.push_back(*newPtlIter);
                rocks.back().setActive(true);
            }
        }
    }

    void tickRocks(float ticks)
    {
        deque<cRock>::iterator ptlIter;
        for( ptlIter = rocks.begin(); ptlIter != rocks.end(); ptlIter ++ )
        {
            if(ptlIter->getSize() > 0)
            {
                ptlIter->updatePos(ticks);
                ptlIter->setAngle(ptlIter->getAngle() + 10.0 * ticks);
            }
            else
            {
                ptlIter->setActive(false);
            }
        }
    }

    void renderRocks()
    {
        //glDisable(GL_LIGHTING);
        //glBindTexture(GL_TEXTURE_2D, texId);

        liveRocks = 0;

        deque<cRock>::iterator ptlIter;
        for( ptlIter = rocks.begin(); ptlIter != rocks.end(); ptlIter++ )
        {
            if(ptlIter->getActive())
            {
                glPushMatrix();
                glTranslatef(ptlIter->getPos().x, ptlIter->getPos().y, -5.0);
                glRotatef(ptlIter->getAngle(), 0.0, 0.0, -1.0);
                glScalef(ptlIter->getSize() * 2, ptlIter->getSize() * 2, ptlIter->getSize() * 2);
                /*glBegin(GL_TRIANGLE_STRIP);
                    glColor4f(1.0, 1.0, 1.0, 1);
                    glTexCoord2d(1,1); glVertex3f(+ 1.0f * (float)ptlIter->getSize(), + 1.0f * (float)ptlIter->getSize(), -1.0); // Top Right
                    glTexCoord2d(0,1); glVertex3f(- 1.0f * (float)ptlIter->getSize(), + 1.0f * (float)ptlIter->getSize(), -1.0); // Top Left
                    glTexCoord2d(1,0); glVertex3f(+ 1.0f * (float)ptlIter->getSize(), - 1.0f * (float)ptlIter->getSize(), -1.0); // Bottom Right
                    glTexCoord2d(0,0); glVertex3f(- 1.0f * (float)ptlIter->getSize(), - 1.0f * (float)ptlIter->getSize(), -1.0); // Bottom Left
                glEnd();*/
                mdl.draw();
                glPopMatrix();
                liveRocks ++;
            }
        }

        //glEnable(GL_LIGHTING);
    }

    void explodeRock(deque<cRock>::iterator &iter) {
        iter->setActive(false);

        snd::playSound(snd::ROCK_EXPLODE);
        givePoints(iter->getSize() * 5);

        if(iter->getSize() > 1) {
            rck::cRock newRck;
            std::deque<rck::cRock> newRcks;

            newRck.setPos(pos2f(iter->getPos().x - iter->getSize() / 2, iter->getPos().y - iter->getSize() / 2));
            newRck.setSize(iter->getSize() - 1);
            newRck.setSpeed(vector2f(iter->getSpeed().x + getRndFloat(-1.0, 1.0), iter->getSpeed().y + getRndFloat(-1.0, 1.0)));
            newRck.setAngle(iter->getAngle() + getRndFloat(-90.0, 90.0));
            newRcks.push_back(newRck);

            newRck.setPos(pos2f(iter->getPos().x + iter->getSize() / 2, iter->getPos().y + iter->getSize() / 2));
            newRck.setSize(iter->getSize() - 1);
            newRck.setSpeed(vector2f(iter->getSpeed().x + getRndFloat(-1.0, 1.0), iter->getSpeed().y + getRndFloat(-1.0, 1.0)));
            newRck.setAngle(iter->getAngle() + getRndFloat(-90.0, 90.0));
            newRcks.push_back(newRck);

            createRocks(newRcks);
        }

        std::deque<ptl::cParticle> newPtls;
        for(int i = 0; i < 1000; i ++)
        {
            float rndFloat = getRndFloat(0*M_PI/180.0, 360*M_PI/180.0);
            ptl::cParticle newPtl(  getRndFloat(0.2, 0.4),
                                    1.0, 0.5 + getRndFloat(0.0, 0.5), 0.5,
                                    cVertex(iter->getPos().x + getRndFloat(- iter->getSize() * 2, iter->getSize() * 2), iter->getPos().y + getRndFloat(- iter->getSize() * 2, iter->getSize() * 2),  - 0.5),
                                    cVertex(iter->getSpeed().x + cos(rndFloat+M_PI) * 5, iter->getSpeed().y + sin(rndFloat) * 5, getRndFloat(-2.0, 0.0)),
                                    cVertex(0.0, 0.0, 0.0)
                                );
            newPtls.push_back(newPtl);
        }
        ptl::createParticles(newPtls);
    }


    bool checkCollide(pos2f pos) {
        deque<cRock>::iterator ptlIter;
        for( ptlIter = rocks.begin(); ptlIter != rocks.end(); ptlIter++ )
        {
            if(ptlIter->getActive())
            {
                float rx1 = ptlIter->getPos().x - ptlIter->getSize();
                float rx2 = ptlIter->getPos().x + ptlIter->getSize();
                float ry1 = ptlIter->getPos().y - ptlIter->getSize();
                float ry2 = ptlIter->getPos().y + ptlIter->getSize();
                //float ra = ptlIter->getAngle();

                float px = pos.x;// * cos(ra*M_PI/180.0);
                float py = pos.y;// * sin(ra*M_PI/180.0);

                if((px > rx1) and (px < rx2) and (py > ry1) and (py < ry2)) {
                    explodeRock(ptlIter);
                    return true;
                }
            }
        }
        return false;
    }

    float closestRockDistance(pos2f pos) {
        deque<cRock>::iterator ptlIter;
        float shortest = 10000;
        for( ptlIter = rocks.begin(); ptlIter != rocks.end(); ptlIter++ )
        {
            if(ptlIter->getActive())
            {
                float dist = sqrt(square(ptlIter->getPos().x - pos.x) + square(ptlIter->getPos().y - pos.y)) - ptlIter->getSize() * 2;
                if(dist < shortest) {
                    shortest = dist;
                }
            }
        }
        return shortest;
    }

    int getLiveRocks() {
        return liveRocks;
    }


}
