#include "SoundSource.h"
#include <stdio.h>
#include <AL/al.h>

using namespace Raincloud;

struct SoundSource::Impl
{
	SoundHandle sourceID = 0;
	float pitch = 1.0f;
	float gain = 1.0f;
	float position[3] = { 0.0f, 0.0f, 0.0f };
	float velocity[3] = { 0.0f, 0.0f, 0.0f };
	bool isLooping = false;
	BufferHandle buffer = 0;

	FadeState fadeState = FadeState::None;
	float fadeTime = 0.0f;
	float fadeTimeTarget = 0.0f;
};

SoundSource::SoundSource()
	:m_Impl(std::make_unique<Impl>())
{
	alGenSources(1, &m_Impl->sourceID);
	alSourcef(m_Impl->sourceID, AL_PITCH, m_Impl->pitch);
	alSourcef(m_Impl->sourceID, AL_GAIN, m_Impl->gain);
	alSource3f(m_Impl->sourceID, AL_POSITION, m_Impl->position[0], m_Impl->position[1], m_Impl->position[2]);
	alSource3f(m_Impl->sourceID, AL_VELOCITY, m_Impl->velocity[0], m_Impl->velocity[1], m_Impl->velocity[2]);
	alSourcei(m_Impl->sourceID, AL_LOOPING, m_Impl->isLooping);
	alSourcei(m_Impl->sourceID, AL_BUFFER, m_Impl->buffer);
}

SoundSource::~SoundSource()
{
	alDeleteSources(1, &m_Impl->sourceID);
}

void SoundSource::Play(ALuint bufferToPlay)
{
	if(bufferToPlay != m_Impl->buffer)
	{
		m_Impl->buffer = bufferToPlay;
		alSourcei(m_Impl->sourceID, AL_BUFFER, (ALint)m_Impl->buffer);
	}

	alSourcePlay(m_Impl->sourceID);
	printf("Playing %i \n", m_Impl->buffer);

	ALint playState;
	alGetSourcei(m_Impl->sourceID, AL_SOURCE_STATE, &playState);

}

void SoundSource::Stop()
{
	alSourceStop(m_Impl->sourceID);
	ALint playState;
	alGetSourcei(m_Impl->sourceID, AL_SOURCE_STATE, &playState);

	if (playState == AL_PLAYING)
	{
		printf("Stopped Playing %i \n", m_Impl->buffer);
	}
}

void SoundSource::Update(float dt)
{


}

void SoundSource::SetIsLooping(bool loop)
{
	m_Impl->isLooping = loop;
	alSourcei(m_Impl->sourceID, AL_LOOPING, m_Impl->isLooping);
}

void SoundSource::SetPitch(float in_pitch)
{
	this->m_Impl->pitch = in_pitch;
	alSourcef(m_Impl->sourceID, AL_PITCH, m_Impl->pitch);
}

void SoundSource::SetGain(float in_gain)
{
	this->m_Impl->gain = in_gain;
	alSourcef(m_Impl->sourceID, AL_GAIN, m_Impl->gain);
}

void SoundSource::SetPosition(float x, float y, float z)
{
  m_Impl->position[0] = x;
  m_Impl->position[1] = y;
  m_Impl->position[2] = z;

  alSource3f(m_Impl->sourceID, AL_POSITION, m_Impl->position[0], m_Impl->position[1], m_Impl->position[2]);

}

void SoundSource::SetVelocity(float x, float y, float z)
{
  m_Impl->velocity[0] = x;
  m_Impl->velocity[1] = y;
  m_Impl->velocity[2] = z;

  alSource3f(m_Impl->sourceID, AL_VELOCITY, m_Impl->velocity[0], m_Impl->velocity[1], m_Impl->velocity[2]);
}

void SoundSource::SetFade(FadeState state, float fadeDuration)
{
	//Ignore calls to nothing, or fade out when we've already faded out, or fade in when we've already faded in.
	if (state == FadeState::None){return;}
	if (state == FadeState::FadingIn && m_Impl->fadeState == FadeState::DoneFadingIn){return;}
	if (state == FadeState::FadingOut && m_Impl->fadeState == FadeState::DoneFadingOut){return;}

	m_Impl->fadeState = state;
	if (m_Impl->fadeState == FadeState::FadingIn)
	{
		m_Impl->fadeTimeTarget = fadeDuration;
		m_Impl->fadeTime = 0.0f;
		printf("Fading in. \n");

	}
	else if (m_Impl->fadeState == FadeState::FadingOut)
	{
		m_Impl->fadeTimeTarget = fadeDuration;
		m_Impl->fadeTime = m_Impl->fadeTimeTarget;

		printf("Fading out. \n");
	}
}




