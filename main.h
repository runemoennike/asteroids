#ifndef __MAIN_H
#define __MAIN_H

void givePoints(int amount);
void giveLives(int amount);
void takeLife();
void loadLevel(int lvl);
int getPlayerState();
void setPlayerState(int state);

enum {
    PLST_NORMAL, PLST_DEAD, PLST_NEEDSPAWN
};

#endif
