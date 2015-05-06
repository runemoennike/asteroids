#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/gl.h>
#include "SDL.h"

#include "ship.h"
#include "metrics.h"
#include "particle.h"
#include "commons.h"
#include "projectile.h"
#include "rock.h"
#include "sound.h"
#include "model.h"
#include "settings.h"
#include "gltext.h"
#include "main.h"
#include "script.h"
#include "debugout.h"

static GLint T0 = 0;
static GLint Frames = 0;
static GLint mainloopDone = 0;

cShip player;
int playerState = PLST_NORMAL;
int playerLives, playerScore, curLvl;
int bgTexId;
int fps;

static void drawShip() {
    glDisable(GL_LIGHTING);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBegin(GL_TRIANGLES);
    glColor3f( 0.0, 0.0, 1.0); glVertex3f( 0.0, 1.0, 0.0);
    glColor3f( 1.0, 1.0, 0.0); glVertex3f(-1.0,-1.0, 0.0);
    glColor3f( 1.0, 1.0, 0.0); glVertex3f( 1.0,-1.0, 0.0);

    glColor3f( 1.0, 1.0, 0.0); glVertex3f(-1.0,-1.0, 0.0);
    glColor3f( 1.0, 0.0, 0.0); glVertex3f(-1.5,-2.0, 0.0);
    glColor3f( 1.0, 1.0, 0.0); glVertex3f( 0.0,-1.0, 0.0);

    glColor3f( 1.0, 1.0, 0.0); glVertex3f( 0.0,-1.0, 0.0);
    glColor3f( 1.0, 0.0, 0.0); glVertex3f( 1.5,-2.0, 0.0);
    glColor3f( 1.0, 1.0, 0.0); glVertex3f( 1.0,-1.0, 0.0);
    glEnd();
    glEnable(GL_LIGHTING);
}

int getPlayerState() {
    return playerState;
}

void setPlayerState(int state) {
    playerState = state;
}

static void drawBg() {
    glDisable(GL_LIGHTING);
    glBindTexture(GL_TEXTURE_2D, bgTexId);
    glBegin(GL_QUADS);
        glColor3f(1.0,1.0,1.0);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0f, -7.5f,  -10.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 10.0f, -7.5f,  -10.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 10.0f,  7.5f,  -10.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-10.0f,  7.5f,  -10.0f);
    glEnd();
    glEnable(GL_LIGHTING);
}

pos2f rotate(pos2f p0, pos2f p1, float r) {
    float x2, y2;
    x2 = (p1.x) * cos(r) - ( p1.y) * sin(r) + p0.x;
    y2 = (p1.y) * sin(r) + ( p1.x) * cos(r) + p0.y;
    return pos2f(x2, y2);
}

static void
draw(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glDisable(GL_BLEND);

    drawBg();

    glScalef( 0.29, 0.29, 0.29);

    glPushMatrix();
    glTranslatef( player.getPos().x, player.getPos().y, 0.0);
    glRotatef(player.getAngle(), 0.0, 0.0, -1.0);
    drawShip();
    glPopMatrix();

    rck::renderRocks();

    glEnable(GL_BLEND);

    ptl::renderParticles();
    prjctl::renderProjectiles();

    gltext::beginOrtho();

    string text;


    glColor3f(1.0, 0.0, 0.0);
    text = "Score: " + intToStr(playerScore);
    gltext::print(0, 460, text.c_str());

    glColor3f(1.0, 0.0, 1.0);
    text = "Level: " + intToStr(curLvl);
    gltext::print(300, 460, text.c_str());

    glColor3f(1.0, 1.0, 0.0);
    text = "Lives: " + intToStr(playerLives);
    gltext::print(525, 460, text.c_str());

    glColor3f(1.0, 1.0, 1.0);
    text = intToStr(fps);
    gltext::print(270, 0, text.c_str());


    gltext::endOrtho();

    glFlush();

    glPopMatrix();
    SDL_GL_SwapBuffers();

    Frames++;
    {
        GLint t = SDL_GetTicks();
        if (t - T0 >= 5000) {
            GLfloat seconds = (t - T0) / 1000.0;
            fps = (int)(Frames / seconds);
            //printf("%d frames in %g seconds = %g FPS\n", Frames, seconds, fps);
            T0 = t;
            Frames = 0;
        }
    }
}


static void
idle(void)
{
    static float lastT = SDL_GetTicks();
    float ticks;
    ticks = SDL_GetTicks() - lastT;
    lastT = SDL_GetTicks();

    /*
    * WORLD HANDLING
    */
    ptl::tickParticles();
    prjctl::tickProjectiles(ticks / 100.0);
    rck::tickRocks(ticks / 1000.0);

    rect4f plLimits;
    switch(getPlayerState()) {
        case PLST_DEAD:
            setPlayerState(PLST_NEEDSPAWN);
            playerLives --;
            break;

        case PLST_NEEDSPAWN:
            plLimits = player.getLimits();
            for(int i=0; i < 10; i ++) {
                float x = getRndFloat(plLimits.x1, plLimits.x2);
                float y = getRndFloat(plLimits.y1, plLimits.y2);

                if(rck::closestRockDistance(pos2f(x, y)) > 4.0) {
                    player.setPos(pos2f(x, y));
                    player.setSpeed(vector2f(0.0, 0.0));
                    setPlayerState(PLST_NORMAL);
                }
            }
            break;

        default:
        case PLST_NORMAL:
            if(player.checkCollide()) {
                setPlayerState(PLST_DEAD);
            }

            player.updatePos(ticks / 10.0);
            player.dampenSpeed(ticks / 50000.0);
            break;
    }

    if(rck::getLiveRocks() == 0) {
        curLvl ++;
        snd::playSound(snd::NEXT_LEVEL);
        loadLevel(curLvl);
    }

    /*
    * KEY HANDLING
    */
    Uint8 *keys;
    keys = SDL_GetKeyState(NULL);

    if ( keys[SDLK_ESCAPE] ) {
      mainloopDone = 1;
    }
    if ( keys[SDLK_UP] ) {
        player.thrust(ticks / 10000.0);

        snd::startSoundOnce(snd::ENGINE);

        std::deque<ptl::cParticle> newPtls;
        for(int i = 0; i < 50; i ++)
        {
            float rndFloat = getRndFloat((player.getAngle()+200.0)*M_PI/180.0, (player.getAngle()+340.0)*M_PI/180.0);
            ptl::cParticle newPtl(  getRndFloat(0.2, 0.4),
                                    1.0, getRndFloat(0.0, 0.5), 0.0,
                                    cVertex(player.getPos().x + cos((player.getAngle()+90.0)*M_PI/180.0) * getRndFloat(1.1, 1.5), player.getPos().y + sin((player.getAngle()-90.0)*M_PI/180.0) * getRndFloat(1.1, 1.5),  - 0.5),
                                    cVertex(-player.getSpeed().x*10.0 + cos(rndFloat+M_PI) * 5, -player.getSpeed().y*10.0 + sin(rndFloat)*5, getRndFloat(-2.0, 0.0)),
                                    cVertex(0.0, 0.0, 0)
                                );
            newPtls.push_back(newPtl);
        }
        ptl::createParticles(newPtls);
    }
    if ( !keys[SDLK_UP] ) {
        snd::stopSound(snd::ENGINE);
    }
    if ( keys[SDLK_DOWN] ) {

    }
    if ( keys[SDLK_LEFT] ) {
        player.turn(LEFT, ticks / 5.0);
    }
    if ( keys[SDLK_RIGHT] ) {
        player.turn(RIGHT, ticks / 5.0);
    }
    if ( keys[SDLK_SPACE] ) {
        static float shotTimer = 0;

        if(SDL_GetTicks() - shotTimer > 500) {
            snd::playSound(snd::LASER);

            prjctl::cProjectile newPrjctl;
            std::deque<prjctl::cProjectile> newPrjctls;

            newPrjctl.setPos(pos2f(player.getPos().x - cos((player.getAngle()+90.0)*M_PI/180.0) * 1.3, player.getPos().y - sin((player.getAngle()-90.0)*M_PI/180.0) * 1.3));
            newPrjctl.setTtl(20.0);
            newPrjctl.setSpeed(vector2f(cos((player.getAngle()-90.0)*M_PI/180.0), sin((player.getAngle()+90.0)*M_PI/180.0)));
            newPrjctl.setAngle(player.getAngle());

            newPrjctls.push_back(newPrjctl);

            prjctl::createProjectiles(newPrjctls);

            shotTimer = SDL_GetTicks();
        }
    }
    if ( keys[SDLK_z] ) {

    }
}

/* new window size or exposure */
static void
reshape(int width, int height)
{
  GLfloat h = (GLfloat) height / (GLfloat) width;

  glViewport(0, 0, (GLint) width, (GLint) height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-1.0, 1.0, -h, h, 5.0, 200.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -40.0);
}

static void
init(int argc, char *argv[])
{
  static GLfloat pos[4] =
  {5.0, 5.0, 10.0, 0.0};

    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0,0.0,0.0,0.0);
    glDepthFunc (GL_LEQUAL);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_SHADE_MODEL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glEnable(GL_NORMALIZE);
    glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    gltext::init();
    ptl::initParticles();
    rck::initRocks();
    prjctl::initProjectiles();

    player.setPos(pos2f(0.0,0.0));
    player.limitPos(rect4f(-30.0,-23.0,30.0,23.0));

    rck::setLimits(rect4f(-32.0,-25.0,31.0,23.0));
    prjctl::setLimits(rect4f(-30.0,-24.0,30.0,23.0));
    ptl::setLimits(rect4f(-30.0,-24.0,30.0,23.0));

    curLvl = 1;
    loadLevel(curLvl);

    cModel dummyModel;
    bgTexId = dummyModel.loadTexture(OPT_PATH_TEXTURES + (string)"bg1" + OPT_PATHEXT_TEXTURES);

    playerLives = 5;
    playerScore = 0;

    scrpt::initVM(scrpt::VM_LEVEL, "levelloader");

    if (argc > 1 && strcmp(argv[1], "-info")==0) {
        printf("GL_RENDERER   = %s\n", (char *) glGetString(GL_RENDERER));
        printf("GL_VERSION    = %s\n", (char *) glGetString(GL_VERSION));
        printf("GL_VENDOR     = %s\n", (char *) glGetString(GL_VENDOR));
        printf("GL_EXTENSIONS = %s\n", (char *) glGetString(GL_EXTENSIONS));
    }
}

void deinit() {
    scrpt::deinitVM(scrpt::VM_LEVEL);
    snd::unloadSounds();
}

void givePoints(int amount) {
    playerScore += amount;
}

void takeLife() {
    playerLives --;
}

void giveLives(int amount) {
    playerLives += amount;
}

void loadLevel(int lvl) {
    int rockSize = (int)floor((float)lvl / 4.0) + 1;
    int rockCount = (lvl % 4) + 1;

    if(rockSize > 5) rockSize = 5;

    rck::cRock newRck;
    std::deque<rck::cRock> newRcks;

    for(int i = 0; i < rockCount; i ++) {
        newRck.setPos(pos2f(getRndFloat(-15.0,15.0),getRndFloat(-15.0,15.0)));
        newRck.setSize(rockSize);
        newRck.setSpeed(vector2f(getRndFloat(-2.0, 2.0), getRndFloat(-2.0, 2.0)));
        newRck.setAngle(getRndFloat(0, 360));
        newRcks.push_back(newRck);
    }

    rck::createRocks(newRcks);
}

int main(int argc, char *argv[])
{
  debugoutput("--- Program start ---");

  SDL_Surface *screen;

  SDL_Init(SDL_INIT_VIDEO);
  debugoutput("Video: SDL inited.");


  screen = SDL_SetVideoMode(SCR_W, SCR_H, 32, SDL_OPENGL);
  if ( ! screen ) {
    fprintf(stderr, "Couldn't set GL video mode: %s\n", SDL_GetError());
    SDL_Quit();
    exit(2);
  }
  debugoutput("Video: OpenGL inited, videomode set.");

  SDL_WM_SetCaption("Asteroids", "asteroids");

  snd::loadSounds();

  init(argc, argv);
  reshape(screen->w, screen->h);
  mainloopDone = 0;
  while ( ! mainloopDone ) {
    SDL_Event event;

    idle();
    while ( SDL_PollEvent(&event) ) {
      switch(event.type) {
        case SDL_VIDEORESIZE:
          screen = SDL_SetVideoMode(event.resize.w, event.resize.h, 32,
                                    SDL_OPENGL);
          if ( screen ) {
            reshape(screen->w, screen->h);
          } else {
            /* Uh oh, we couldn't set the new video mode?? */;
          }
          break;

        case SDL_QUIT:
          mainloopDone = 1;
          break;
      }
    }

    draw();
  }
  deinit();
  SDL_Quit();

  debugoutput("Program end.");

  return 0;
}
