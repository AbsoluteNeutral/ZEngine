

#ifndef _AUDIOLISTENER_H_
#define _AUDIOLISTENER_H_

#include "fmod.hpp"
#include <string>

#include "Component.h"

#define NUMBEROF_CHANNELGROUP 2

class ZG_API AudioListener : public Component
{
	AudioListener(const AudioListener&);
	void operator=(const AudioListener&);
public:
	AudioListener() noexcept;
	virtual ~AudioListener();

	//void Attach(GameObject* obj);
	void Update() ;

	float GetChannelGroupVolume(unsigned channelnumber) const;
	void SetChannelGroupVolume(unsigned channelnumber, float value);
	void SetMute(unsigned channelnumber, bool DoYouWantToMuteMeThisAudioListenerYo);
	bool IsMute(unsigned channelnumber) const;
	void Pause(bool pause) const;

	int FMODSoundResult;
	bool isRunning;
	static FMOD::System* ALSystem;
	static FMOD::ChannelGroup* channelgroup[NUMBEROF_CHANNELGROUP];
	static float channelgroup_volume[NUMBEROF_CHANNELGROUP];
private:
	FMOD_VECTOR velocity { 0.0f, 0.0f, 0.0f };
	FMOD_VECTOR lastpos;
};

#endif //_AUDIOLISTENER_H_
