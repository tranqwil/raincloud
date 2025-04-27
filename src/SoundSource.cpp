#include "SoundSource.h"
#include <stdio.h>



SoundSource::SoundSource()
{
	alGenSources(1, &sourceID);
	alSourcef(sourceID, AL_PITCH, pitch);
	alSourcef(sourceID, AL_GAIN, gain);
	alSource3f(sourceID, AL_POSITION, position[0], position[1], position[2]);
	alSource3f(sourceID, AL_VELOCITY, velocity[0], velocity[1], velocity[2]);
	alSourcei(sourceID, AL_LOOPING, isLooping);
	alSourcei(sourceID, AL_BUFFER, buffer);
}

SoundSource::~SoundSource()
{
	alDeleteSources(1, &sourceID);
}

void SoundSource::Play(ALuint bufferToPlay)
{
	if(bufferToPlay != buffer)
	{
		buffer = bufferToPlay;
		alSourcei(sourceID, AL_BUFFER, (ALint)buffer);
	}

	alSourcePlay(sourceID);
	printf("Playing %i \n", buffer);

	ALint playState;
	alGetSourcei(sourceID, AL_SOURCE_STATE, &playState);

}

void SoundSource::Stop()
{
	alSourceStop(sourceID);
	ALint playState;
	alGetSourcei(sourceID, AL_SOURCE_STATE, &playState);

	if (playState == AL_PLAYING)
	{
		printf("Stopped Playing %i \n", buffer);
	}
}

void SoundSource::Update(float dt)
{
	HandleFade(dt);

}

void SoundSource::SetIsLooping(bool loop)
{
	isLooping = loop;
	alSourcei(sourceID, AL_LOOPING, isLooping);
}

void SoundSource::SetPitch(float in_pitch)
{
	this->pitch = in_pitch;
	alSourcef(sourceID, AL_PITCH, pitch);
}

void SoundSource::SetGain(float in_gain)
{
	this->gain = in_gain;
	alSourcef(sourceID, AL_GAIN, gain);
}

void SoundSource::SetPosition(float x, float y, float z)
{
  this->position[0] = x;
  this->position[1] = y;
  this->position[2] = z;

  alSource3f(sourceID, AL_POSITION, position[0], position[1], position[2]);

}

void SoundSource::SetVelocity(float x, float y, float z)
{
  this->velocity[0] = x;
  this->velocity[1] = y;
  this->velocity[2] = z;

  alSource3f(sourceID, AL_VELOCITY, velocity[0], velocity[1], velocity[2]);
}

void SoundSource::SetFade(FadeState state, float fadeDuration)
{
	//Ignore calls to nothing, or fade out when we've already faded out, or fade in when we've already faded in.
	if (state == FadeState::None){return;}
	if (state == FadeState::FadingIn && fadeState == FadeState::DoneFadingIn){return;}
	if (state == FadeState::FadingOut && fadeState == FadeState::DoneFadingOut){return;}

	fadeState = state;
	if (fadeState == FadeState::FadingIn)
	{
		fadeTimeTarget = fadeDuration;
		fadeTime = 0.0f;
		printf("Fading in. \n");

	}
	else if (fadeState == FadeState::FadingOut)
	{
		fadeTimeTarget = fadeDuration;
		fadeTime = fadeTimeTarget;

		printf("Fading out. \n");
	}
}

void SoundSource::HandleFade(float dt) {

	float gainMultiplier = 1.0f;
	if (fadeState == FadeState::FadingIn)
	{
		fadeTime += dt;
		gainMultiplier = ( fadeTime / fadeTimeTarget);

		if (fadeTime >= fadeTimeTarget){fadeState = FadeState::DoneFadingIn;}
		else {this->SetGain(gainMultiplier);}
	}
	else if (fadeState == FadeState::FadingOut)
	{
		fadeTime -= dt;
		gainMultiplier = ( fadeTime / fadeTimeTarget);

		if (fadeTime <= 0.0f) {
			this->SetGain(0.0f);
			fadeState= FadeState::DoneFadingOut;
		}
		else {this->SetGain(gainMultiplier);}
	}

}


