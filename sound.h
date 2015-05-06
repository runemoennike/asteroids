#ifndef __SOUND_H
#define __SOUND_H

namespace snd {

    enum SNDS {
        ENGINE, LASER, ROCK_EXPLODE, SHIP_EXPLODE, NEXT_LEVEL, SND__COUNT
    };

    bool loadSounds();
    void unloadSounds();
    void playSound(SNDS sound);
    void startSoundOnce(SNDS sound);
    void stopSound(SNDS sound);
}

#endif
