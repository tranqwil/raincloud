#pragma once
#include <AL\al.h>
class SoundSource
{
public:
	SoundSource();
	~SoundSource();

	void Play(ALuint bufferToPlay);

	void SetIsLooping(bool loop);

	void SetPitch(float pitch);

	void SetGain(float gain);

private:
	ALuint sourceID = 0;
	float pitch = 1.0f;
	float gain = 1.0f;
	float position[3] = { 0.0f, 0.0f, 0.0f };
	float velocity[3] = { 0.0f, 0.0f, 0.0f };
	bool isLooping = false;
	ALuint buffer = 0;

};

