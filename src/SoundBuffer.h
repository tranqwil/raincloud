#pragma once
#include <AL\al.h>
#include <vector>
class SoundBuffer
{
public: 
	
	static SoundBuffer* Get();

	ALuint AddSoundEffect(const char* filename);

	bool RemoveSoundEffect(ALuint& buffer);	

private:
	SoundBuffer();
	~SoundBuffer();
	
	std::vector<ALuint> pSoundEffectBuffers;

};

