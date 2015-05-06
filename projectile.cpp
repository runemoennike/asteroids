#include "projectile.h"
#include "settings.h"
#include "model.h"
#include "rock.h"

namespace prjctl {

    float cProjectile::getTtl() {
        return this->ttl;
    }

    void cProjectile::setTtl(float ttl) {
        this->ttl = ttl;
    }

    bool cProjectile::getActive() {
        return this->active;
    }

    void cProjectile::setActive(bool state) {
        this->active = state;
    }

    using std::deque;

    deque<cProjectile> projectiles;
    GLuint texId;
    int liveProjectiles;
    rect4f limits;

    void initProjectiles()
    {
        projectiles.resize(OPT_NUM_INITIAL_PRJCTLS);
        deque<cProjectile>::iterator ptlIter;
        for( ptlIter = projectiles.begin(); ptlIter != projectiles.end(); ptlIter ++ )
        {
            ptlIter->setActive(false);
        }

        cModel dummyModel;
        texId = dummyModel.loadTexture(OPT_PATH_TEXTURES + (string)"projectile" + OPT_PATHEXT_TEXTURES);
    }

    void setLimits(rect4f newlimits) {
        limits = newlimits;
    }

    void createProjectiles(deque<cProjectile> &newProjectiles)
    {
        deque<cProjectile>::iterator newPtlIter = newProjectiles.begin();

        // Check for unused spots
        deque<cProjectile>::iterator ptlIter;
        for( ptlIter = projectiles.begin(); ptlIter != projectiles.end(); ptlIter ++ )
        {
            if(! ptlIter->getActive())
            {
                *ptlIter = *newPtlIter;
                ptlIter->setActive(true);
                ptlIter->limitPos(limits);
                newPtlIter ++;
                if(newPtlIter == newProjectiles.end()) return;
            }
        }

        if((newPtlIter != newProjectiles.end()) && (liveProjectiles < OPT_MAX_PRJCTLS))
        {
            // Didn't find unused spot, so create them
            for( newPtlIter = newPtlIter; newPtlIter != newProjectiles.end(); newPtlIter ++ )
            {
                projectiles.push_back(*newPtlIter);
                projectiles.back().setActive(true);
            }
        }
    }

    void tickProjectiles(float ticks)
    {
        deque<cProjectile>::iterator ptlIter;
        for( ptlIter = projectiles.begin(); ptlIter != projectiles.end(); ptlIter ++ )
        {
            if(ptlIter->getActive()) {
                if(ptlIter->getTtl() > 0)
                {
                    ptlIter->updatePos(ticks);

                    ptlIter->setTtl(ptlIter->getTtl() - ticks);

                    if(rck::checkCollide(pos2f(ptlIter->getPos().x, ptlIter->getPos().y))) {
                        ptlIter->setActive(false);
                    }
                }
                else
                {
                    ptlIter->setActive(false);
                }
            }
        }
    }

    void renderProjectiles()
    {
        glDisable(GL_LIGHTING);
        glBindTexture(GL_TEXTURE_2D, texId);

        liveProjectiles = 0;

        deque<cProjectile>::iterator ptlIter;
        for( ptlIter = projectiles.begin(); ptlIter != projectiles.end(); ptlIter++ )
        {
            if(ptlIter->getActive())
            {
                glPushMatrix();
                glTranslatef(ptlIter->getPos().x, ptlIter->getPos().y, 0.0);
                glRotatef(ptlIter->getAngle(), 0.0, 0.0, -1.0);
                glBegin(GL_TRIANGLE_STRIP);
                    glColor4f(1.0, 1.0, 0.0, 1.0);
                    glTexCoord2d(1,1); glVertex3f(+ 0.1f, + 0.3f, 0.0); // Top Right
                    glTexCoord2d(0,1); glVertex3f(- 0.1f, + 0.3f, 0.0); // Top Left
                    glTexCoord2d(1,0); glVertex3f(+ 0.1f, - 0.3f, 0.0); // Bottom Right
                    glTexCoord2d(0,0); glVertex3f(- 0.1f, - 0.3f, 0.0); // Bottom Left
                glEnd();
                glPopMatrix();
                liveProjectiles ++;
            }
        }

        glEnable(GL_LIGHTING);
    }



}
