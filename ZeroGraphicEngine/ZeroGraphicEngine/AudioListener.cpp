/*****************************************************************************/
/*!
\file			AudioListener.cpp
\brief			Audio listener
*/
/*****************************************************************************/

#include "stdafx.h"
#include "AudioListener.h"

FMOD::System* AudioListener::ALSystem;
FMOD::ChannelGroup* AudioListener::channelgroup[NUMBEROF_CHANNELGROUP];
float AudioListener::channelgroup_volume[NUMBEROF_CHANNELGROUP]{ 0.5f, 0.5f };

const int   INTERFACE_UPDATETIME = 800;      // 50ms update for interface
const int   R_INTERFACE_UPDATETIME = 1000 / 800;      // 50ms update for interface

AudioListener::AudioListener() noexcept
	:FMODSoundResult(0), isRunning(true), velocity{ 0.0f, 0.0f, 0.0f },
	lastpos{ 0.0f, 0.0f, 0.0f }
{
	//create system
	FMODSoundResult = FMOD::System_Create(&ALSystem);
	if (FMODSoundResult != FMOD_OK) {
		isRunning = false;
	}
		
	//check version
#if defined(_DEBUG)
	unsigned int      version;
	FMODSoundResult = ALSystem->getVersion(&version);
	if (version < FMOD_VERSION) {
		isRunning = false;
		ErrorMsg("FMOD lib version %08x doesn't match header version %08x");
	}
#endif
	//init system
	FMODSoundResult = ALSystem->init(64, FMOD_INIT_NORMAL, 0);    // Initialize FMOD.
	if (FMODSoundResult != FMOD_OK) {
		isRunning = false;
	}

	if (isRunning) {
		for (unsigned i = 0; i < NUMBEROF_CHANNELGROUP; ++i) {
			ALSystem->createChannelGroup(NULL, &channelgroup[i]);
			channelgroup[i]->setVolume(channelgroup_volume[i]);
		}
		ALSystem->set3DSettings(1.0, 1.0f, 1.0f);
	}
		
}

AudioListener::~AudioListener() {
	if (FMODSoundResult == FMOD_OK) {
		ALSystem->close();
		ALSystem->release();
	}
}

//void AudioListener::Attach(GameObject* obj) {
//	owner = obj;
//}

float AudioListener::GetChannelGroupVolume(unsigned channelnumber) const {
	float tmp = 0.0f;
	channelgroup[channelnumber]->getVolume(&tmp);
	return tmp;
}
void AudioListener::SetChannelGroupVolume(unsigned channelnumber, float value) {
	channelgroup_volume[channelnumber] = MAX2(MIN2(value, 1.0f), 0.0f);
	channelgroup[channelnumber]->setVolume(channelgroup_volume[channelnumber]);
}
void AudioListener::SetMute(unsigned channelnumber, bool DoYouWantToMuteMeThisAudioListenerYo) {
	channelgroup[channelnumber]->setMute(DoYouWantToMuteMeThisAudioListenerYo);
}

bool AudioListener::IsMute(unsigned channelnumber) const {
	bool tmp = true;
	channelgroup[channelnumber]->getMute(&tmp);
	return tmp;
}

void AudioListener::Pause(bool pause_) const {
	for (unsigned i = 0; i < NUMBEROF_CHANNELGROUP; ++i) {
		if(channelgroup[i] != nullptr)
			channelgroup[i]->setPaused(pause_);
	}
}

void AudioListener::Update() {
	//FMOD_VECTOR position{ owner->transform.translation.x, owner->transform.translation.y, owner->transform.translation.z };
	//Vector3 f{ owner->transform.Forward() };
	//Vector3 u{ owner->transform.Up() };
	//
	//FMOD_VECTOR forward{ f.x, f.y, f.z };
	//FMOD_VECTOR up{u.x, u.y, u.z };
	//
	////float tmp =  INTERFACE_UPDATETIME;
	//velocity.x = (position.x - lastpos.x) * R_INTERFACE_UPDATETIME;
	//velocity.y = (position.y - lastpos.y) * R_INTERFACE_UPDATETIME;
	//velocity.z = (position.z - lastpos.z) * R_INTERFACE_UPDATETIME;
	//lastpos = position;
	//
	//ALSystem->set3DListenerAttributes(0, &position, &velocity, &forward, &up);
	//ALSystem->update();
}
