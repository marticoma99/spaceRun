#include "sound.h"
#include <iostream>

std::map<std::string, Sound*> Sound::loadedAudios;

Sound::Sound() {
	sample = 0;
	channel = -1;
}

Sound::~Sound() {
	BASS_SampleFree(sample);
}

void Sound::Init() {
	//-1 dafault system sound out
	if (BASS_Init(-1, 44100, 0, 0, NULL) == false) std::cout << " + Error at BASS init\n";
	else std::cout << " + Bass init correct\n";
}

HCHANNEL Sound::play(float volume) {
	//create a channel if not created
	if(channel == -1) channel = BASS_SampleGetChannel(sample, true);

	//set the volume
	BASS_ChannelSetAttribute(channel, BASS_ATTRIB_VOL, volume);

	//play the channel
	BASS_ChannelPlay(channel, true);

	return channel;
}

//return an audio if is in loaded audio, if not load the audio
Sound* Sound::Get(const char* filename,int atSameTime) {

	//search if the audio is already loaded
	std::map<std::string, Sound*>::iterator it = loadedAudios.find(filename);
	//if is loaded return it
	if(it != loadedAudios.end()) return it->second;
	else {

		//if is not loaded create a new audio
		Sound* newSound = new Sound();
		newSound->sample = BASS_SampleLoad(false, filename, 0, 0, atSameTime, 0);
		loadedAudios[filename] = newSound;

		if (newSound->sample == 0) std::cout << " + Audio " << filename << " not found\n";
		else {
			std::cout << " + Audio " << filename << " loaded\n";
			return newSound;
		}
	}
}

//play the audio filename, at volume volume, and play at same time stSameTime audios
HCHANNEL* Sound::Play(const char* filename, float voume, int atSameTime) {
	//get the audio and play it
	Sound* soundToPlay = Sound::Get(filename, atSameTime);
	soundToPlay->play(voume);
	return &soundToPlay->channel;
}

//play a single audio
HCHANNEL* Sound::PlaySingle(const char* filename, float voume) {
	Sound* soundToPlay = Sound::Get(filename, 1);

	//play only if the audio is not being played
	if (BASS_ChannelIsActive(soundToPlay->channel) != BASS_ACTIVE_PLAYING) soundToPlay->play(voume);

	//loop the audio
	BASS_ChannelFlags(soundToPlay->channel, BASS_SAMPLE_LOOP, BASS_SAMPLE_LOOP);

	return &soundToPlay->channel;
}

//stop a channel
void Sound::Stop(HCHANNEL channel) {
	BASS_ChannelStop(channel);
}

//stop the audio with name filenaname
void Sound::StopAudio(const char* filename) {
	//get the channel where is playing the audio and stop it
	Sound* soundToPlay = Sound::Get(filename, 1);
	Sound::Stop(soundToPlay->channel);
	soundToPlay->channel = -1;
}