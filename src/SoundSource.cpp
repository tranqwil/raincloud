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

	while (playState == AL_PLAYING) 
	{
		alGetSourcei(sourceID, AL_SOURCE_STATE, &playState);
	}

	printf("Done Playing %i \n", buffer);


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