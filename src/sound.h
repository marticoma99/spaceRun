#ifndef SOUND_H
#define SOUND_H

#include "bass.h"
#include "framework.h"
#include "utils.h"
#include <map>

class Sound {
public:
	//all loaded audios
	static std::map<std::string, Sound*> loadedAudios;

	//sample and channel of specific audio
	HSAMPLE sample;
	HCHANNEL channel;

	Sound();
	~Sound();
	//init the bass (only once)
	static void Init();
	//play a sound and ruturn the channel
	HCHANNEL play(float volume);
	//return an audio if is in loaded audio, if not load the audio
	static Sound* Get(const char* filename, int atSameTime);
	//play the audio filename, at volume volume, and play at same time stSameTime audios
	static HCHANNEL* Play(const char* filename, float voume, int atSameTime);
	//for playing a single audio, no more that one at the same time
	static HCHANNEL* PlaySingle(const char* filename, float voume);
	//stop a channel
	static void Stop(HCHANNEL channel);
	//stop the audio with name filenaname
	static void StopAudio(const char* filename);
};

#endif
