#include <sstream>
#include "sound.h"
#include "FMOD/fmod.h"
#include "debugout.h"

namespace snd {

FSOUND_SAMPLE* snds[SND__COUNT];
bool channelPlaying[48];

bool loadSounds() {
    if (FSOUND_GetVersion() < FMOD_VERSION) {
		std::ostringstream streamTemp;
		streamTemp << "Wrong FMOD dll version. Expected version " << FMOD_VERSION << ".";
        output(streamTemp.str());
        return false;
    }

    if (!FSOUND_Init(32000, 48, 0)) {
        output("Internal sound sub-system error.");
		return false;
    }

    for(int i = 0; i < 48; i++) {
        channelPlaying[i] = false;
    }

    snds[ROCK_EXPLODE] = FSOUND_Sample_Load(FSOUND_FREE, "data/snds/shipexplode.wav", 0,0,0);
    snds[SHIP_EXPLODE] = FSOUND_Sample_Load(FSOUND_FREE, "data/snds/explode.wav", 0,0,0);
    snds[ENGINE] = FSOUND_Sample_Load(FSOUND_FREE, "data/snds/engine.wav", 0,0,0);
    snds[LASER] = FSOUND_Sample_Load(FSOUND_FREE, "data/snds/laser.wav", 0,0,0);
    snds[NEXT_LEVEL] = FSOUND_Sample_Load(FSOUND_FREE, "data/snds/level.wav", 0,0,0);

    debugoutput("Sound: Sound sub-system inited, effects loaded.");

    return true;
}

void unloadSounds()
{
    FSOUND_Sample_Free(snds[ROCK_EXPLODE]);
    FSOUND_Sample_Free(snds[SHIP_EXPLODE]);
    FSOUND_Sample_Free(snds[ENGINE]);
    FSOUND_Sample_Free(snds[LASER]);
    FSOUND_Sample_Free(snds[NEXT_LEVEL]);
    debugoutput("Sound: Deinited.");
}

void playSound(SNDS sound) {
    FSOUND_PlaySound(FSOUND_FREE, snds[sound]);
}

void startSoundOnce(SNDS sound) {
    if(!FSOUND_IsPlaying(sound)) {
        FSOUND_PlaySound(sound, snds[sound]);
        channelPlaying[sound] = true;
    }
}

void stopSound(SNDS sound) {
    FSOUND_StopSound(sound);
    channelPlaying[sound] = false;
}

}
