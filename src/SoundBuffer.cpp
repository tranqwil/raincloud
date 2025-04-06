#include "SoundBuffer.h"
#include <sndfile.h>
#include <inttypes.h>
#include <AL/alext.h>
#include <cstdlib>

SoundBuffer* SoundBuffer::Get()
{
	static SoundBuffer* soundbuffer = new SoundBuffer();
	return soundbuffer;
}

SoundBuffer::SoundBuffer()
{
	pSoundEffectBuffers.clear();
}

SoundBuffer::~SoundBuffer()
{
	alDeleteBuffers(pSoundEffectBuffers.size(), pSoundEffectBuffers.data());
	pSoundEffectBuffers.clear();
}

ALuint SoundBuffer::AddSoundEffect(const char* filename)
{	
	//Attributes for new sound file
	ALenum err; 
	ALenum format;
	ALuint buffer;
	SNDFILE* sndfile;
	SF_INFO sfinfo;
	short* membuf;
	sf_count_t num_frames;
	ALsizei num_bytes;
    
	//Open the sound file and check we can use it.
	sndfile = sf_open(filename, SFM_READ, &sfinfo);
	if (!sndfile)
	{
		fprintf(stderr, "Failed to open sound file %s\n", filename);
		return 0;
	}
	if (sfinfo.frames < 1 || sfinfo.frames >(sf_count_t)(INT_MAX / sizeof(short)) / sfinfo.channels)
	{
		fprintf(stderr, "Bad sample count %d\n", (int)sfinfo.frames);
		sf_close(sndfile);
		return 0;
	}

	/* Get the sound format, and figure out the OpenAL format */
	format = AL_NONE;
	if (sfinfo.channels == 1)
		format = AL_FORMAT_MONO16;
	else if (sfinfo.channels == 2)
		format = AL_FORMAT_STEREO16;
	else if (sfinfo.channels == 3)
	{
		if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
			format = AL_FORMAT_BFORMAT2D_16;
	}
	else if (sfinfo.channels == 4)
	{
		if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
			format = AL_FORMAT_BFORMAT3D_16;
	}
	if (!format)
	{
		fprintf(stderr, "Unsupported channel count: %d\n", sfinfo.channels);
		sf_close(sndfile);
		return 0;
	}

	/* Decode audio file to a buffer*/

	//Allocate enough memory for the sound file
	membuf = static_cast<short*>(malloc((size_t)(sfinfo.frames * sfinfo.channels) * sizeof(short)));

	//Read the sound file into frames and bytes
	num_frames = sf_readf_short(sndfile, membuf, sfinfo.frames);
	if (num_frames < 1)
	{
		free(membuf);
		sf_close(sndfile);
		fprintf(stderr, "Failed to read samples in %s (%" PRId64 ")\n", filename, num_frames);
		return 0;
	}

	num_bytes = (ALsizei)(num_frames * sfinfo.channels) * (ALsizei)sizeof(short);

	//Buffer the newly read sound file into an AL Buffer
	buffer = 0; 
	alGenBuffers(1, &buffer);
	alBufferData(buffer, format, membuf, num_bytes, sfinfo.samplerate);

	//now that we have buffered into OpenAL, we can free the memory from sndfile and close it in sndfile.
	free(membuf);
	sf_close(sndfile);

	//Check for errors. If there was one, delete the buffers. 
	err = alGetError();
	if (err != AL_NO_ERROR)
	{
		fprintf(stderr, "OpenAL Error: %s\n", alGetString(err));
		if (buffer && alIsBuffer(buffer))
			alDeleteBuffers(1, &buffer);
		return 0;
	}

	//add the newly created buffer to the vector of buffers
	pSoundEffectBuffers.push_back(buffer);  

	return buffer;
}

bool SoundBuffer::RemoveSoundEffect(ALuint& buffer)
{
	//Create a iterator to find the buffer in the vector
	auto it = pSoundEffectBuffers.begin();

	//While the iterator hasn't reached the end
	while (it != pSoundEffectBuffers.end())
	{
		//if we found the buffer we're looking for
		if (*it == buffer) 
		{
			//delete the buffer at the iterator
			alDeleteBuffers(1, &*it);

			it = pSoundEffectBuffers.erase(it);

			return true;
		}
		else
		{
			//otherwise move to next item in the vector
			++it;
		}
	
	}
	//Buffer was not found in the vector.
	return false;
}
