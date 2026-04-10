#pragma once
#include <AL/al.h>

enum class FadeState {
	None,
	FadingIn,
	FadingOut,
	DoneFadingIn,
	DoneFadingOut
};


class SoundSource
{
public:
	SoundSource();
	~SoundSource();

	void Play(ALuint bufferToPlay);

	void Stop();

	void Update(float dt);

	void SetIsLooping(bool loop);

	void SetPitch(float pitch);

	void SetGain(float gain);

	void SetPosition(float x, float y, float z);

	void SetVelocity(float x, float y, float z);

	void SetFade(FadeState state, float fadeDuration);



private:

	void HandleFade(float dt);

	ALuint sourceID = 0;
	float pitch = 1.0f;
	float gain = 1.0f;
	float position[3] = { 0.0f, 0.0f, 0.0f };
	float velocity[3] = { 0.0f, 0.0f, 0.0f };
	bool isLooping = false;
	ALuint buffer = 0;

	FadeState fadeState = FadeState::None;
	float fadeTime = 0.0f;
	float fadeTimeTarget = 0.0f;

};



