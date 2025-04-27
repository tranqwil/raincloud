#pragma once
#include <AL/alc.h>

class SoundDevice
{

public:
	static SoundDevice* Get();

	ALCint GetSampleRate() const;

private: 
	SoundDevice();
	~SoundDevice();

	ALCdevice* pALCdevice;
	ALCcontext* pALCcontext;

	

};

