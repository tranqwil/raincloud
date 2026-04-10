//
// Created by Will on 12/04/2025.
//

#include "Stream.h"

#include <iostream>
#include <ostream>
#include <SoundDevice.h>
#include <stdio.h>
#include <AL/alext.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <sndfile.h>

using namespace Raincloud;

struct Stream::Impl
{
    //Location of the audio data to be streamed
    SNDFILE* file =nullptr;

    //Info about the audio file we are streaming
    SF_INFO info;

    //Info about the audio file we are streaming that OpenAL can understand.
    ALenum format;

    static const int NUM_BUFFERS = 4; //Four sequential buffers.
    static const int BUFFER_SIZE = 65536; // 32kb of data in each buffer.

    //Where (in the game) the stream will play from.
    SoundHandle source;

    //The array of  buffers that we loop through when streaming
    BufferHandle buffers[NUM_BUFFERS];


    //Pointer to the buffer we are currently streaming
    short* memBuf;
};

Stream::Stream(const char *filename)
    :m_Impl(std::make_unique<Impl>())
{

    //We're telling OpenAL where (in the game) we're going to play this stream from.
    alGenSources(1, &m_Impl->source);

    //Give OpenAL our buffers to set them up.
    alGenBuffers(m_Impl->NUM_BUFFERS, m_Impl->buffers);

    //To allocate enough memory for our buffers.
    std::size_t frame_size;

    //Open the file on disk. Error if this fails.
    m_Impl->file = sf_open(filename, SFM_READ, &m_Impl->info);
    if (!m_Impl->file) {
        std::cerr << "Error opening streaming file " << filename << std::endl;
    }

    //Derive the OpenAL format from the sfinfo. Error if unsure.
    if (m_Impl->info.channels == 1)
        m_Impl->format = AL_FORMAT_MONO16;
    else if (m_Impl->info.channels == 2)
        m_Impl->format = AL_FORMAT_STEREO16;
    else if (m_Impl->info.channels == 3)
    {
        if (sf_command(m_Impl->file, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
            m_Impl->format = AL_FORMAT_BFORMAT2D_16;
    }
    else if (m_Impl->info.channels == 4)
    {
        if (sf_command(m_Impl->file, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
            m_Impl->format = AL_FORMAT_BFORMAT3D_16;
    }
    if (!m_Impl->format)
    {
        sf_close(m_Impl->file);
        m_Impl->file = NULL;
        std::cerr << ("Unsupported channel count from file") << std::endl;
    }

    //Calculate how much memory we need for our buffer. Allocate it and store the location for later.
    //At this point, we have a pointer to a block of memory that is large enough for exactly one buffer.
    frame_size = ((size_t)m_Impl->BUFFER_SIZE * (size_t)m_Impl->info.channels) * sizeof(short);
    m_Impl->memBuf = static_cast<short*>(malloc(frame_size));




}

Stream::~Stream() {

    alDeleteSources(1, &m_Impl->source);

    if (m_Impl->file)
    {
        sf_close(m_Impl->file);
        m_Impl->file = nullptr;
    }

    free(m_Impl->memBuf);

    alDeleteBuffers(m_Impl->NUM_BUFFERS, m_Impl->buffers);
}

void Stream::Play() {

    //we increment this each time we buffer in some data
    //at the end, we use it to tell OpenAL how many buffers to queue up.
    ALsizei i;

    //Clear our errors.
    alGetError();

    //Rewind the source and clear the buffer queue.
    alSourcei(m_Impl->source, AL_BUFFER, 0);

    //Fill all of our buffers with data from the streamed file.
    for (i = 0; i < m_Impl->NUM_BUFFERS; i++)
    {
        //Fill our portable memory buffer (memBuf) with exactly the right amount of data from the sound file.
        //the soundfile file knows where it's read position is; so each time we do this, it buffers the next chunk of
        //data.
        //slen is equal to the number of audio frames we read.
        sf_count_t slen = sf_readf_short(m_Impl->file, m_Impl->memBuf,  m_Impl->BUFFER_SIZE);

        //for if we reach the end of the file.
        if (slen < 1){break;}

        //Calculate how much memory we buffered (in bytes). We need to pass this to OpenAL when we hand off the data from memBuf
        //and into OpenAL.
        slen *= m_Impl->info.channels * (sf_count_t)sizeof(short);

        //Buffer the data into OpenAl.
        alBufferData(m_Impl->buffers[i], m_Impl->format, m_Impl->memBuf, (ALsizei)slen, m_Impl->info.samplerate);

    }
    if (alGetError() != AL_NO_ERROR) {
        std::cerr << "Error buffering for data before playback. " << std::endl;
    }

    //At this point, all of our buffers should have been sequentially filled with the first chunks of the audio file
    //, and passed over to openAL. We also checked that nothing went wrong.

    //Queue the buffers, and begin playback.
    alSourceQueueBuffers(m_Impl->source, i, m_Impl->buffers);
    alSourcePlay(m_Impl->source);
    if (alGetError() != AL_NO_ERROR)
    {
        std::cerr << ("Error starting playback") << std::endl;
    }
}

void Stream::Update() {

    ALint processed, state;

    // clear error
    alGetError();
    /* Get relevant source info */
    alGetSourcei(m_Impl->source, AL_SOURCE_STATE, &state);
    alGetSourcei(m_Impl->source, AL_BUFFERS_PROCESSED, &processed);
    if (alGetError() != AL_NO_ERROR)
    {
        throw("error checking music source state");
    }

    /* Unqueue and handle each processed buffer */
    while (processed > 0)
    {
        ALuint bufid;
        sf_count_t slen;

        alSourceUnqueueBuffers(m_Impl->source, 1, &bufid);
        processed--;

        /* Read the next chunk of data, refill the buffer, and queue it
         * back on the source */
        slen = sf_readf_short(m_Impl->file, m_Impl->memBuf, m_Impl->BUFFER_SIZE);
        if (slen > 0)
        {
            slen *= m_Impl->info.channels * (sf_count_t)sizeof(short);
            alBufferData(bufid, m_Impl->format, m_Impl->memBuf, (ALsizei)slen,
                m_Impl->info.samplerate);
            alSourceQueueBuffers(m_Impl->source, 1, &bufid);
        }
        if (alGetError() != AL_NO_ERROR)
        {
            throw("error buffering music data");
        }
    }

    /* Make sure the source hasn't underrun */
    if (state != AL_PLAYING && state != AL_PAUSED)
    {
        ALint queued;

        /* If no buffers are queued, playback is finished */
        alGetSourcei(m_Impl->source, AL_BUFFERS_QUEUED, &queued);
        if (queued == 0)
            return;

        alSourcePlay(m_Impl->source);
        if (alGetError() != AL_NO_ERROR)
        {
            throw("error restarting music playback");
        }
    }

}



