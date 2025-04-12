//
// Created by Will on 12/04/2025.
//

#include "Stream.h"

#include <iostream>
#include <ostream>
#include <stdio.h>
#include <AL/alext.h>

Stream::Stream(const char *filename) {

    //We're telling OpenAL where (in the game) we're going to play this stream from.
    alGenSources(1, &source);

    //Give OpenAL our buffers to set them up.
    alGenBuffers(NUM_BUFFERS, buffers);

    //To allocate enough memory for our buffers.
    std::size_t frame_size;

    //Open the file on disk. Error if this fails.
    file = sf_open(filename, SFM_READ, &info);
    if (!file) {
        std::cerr << "Error opening streaming file " << filename << std::endl;
    }

    //Derive the OpenAL format from the sfinfo. Error if unsure.
    if (info.channels == 1)
        format = AL_FORMAT_MONO16;
    else if (info.channels == 2)
        format = AL_FORMAT_STEREO16;
    else if (info.channels == 3)
    {
        if (sf_command(file, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
            format = AL_FORMAT_BFORMAT2D_16;
    }
    else if (info.channels == 4)
    {
        if (sf_command(file, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
            format = AL_FORMAT_BFORMAT3D_16;
    }
    if (!format)
    {
        sf_close(file);
        file = NULL;
        std::cerr << ("Unsupported channel count from file") << std::endl;
    }

    //Calculate how much memory we need for our buffer. Allocate it and store the location for later.
    //At this point, we have a pointer to a block of memory that is large enough for exactly one buffer.
    frame_size = ((size_t)BUFFER_SIZE * (size_t)info.channels) * sizeof(short);
    memBuf = static_cast<short*>(malloc(frame_size));




}

Stream::~Stream() {

    alDeleteSources(1, &source);

    if (file)
    {
        sf_close(file);
        file = nullptr;
    }

    free(memBuf);

    alDeleteBuffers(NUM_BUFFERS, buffers);
}

void Stream::Play() {

    //we increment this each time we buffer in some data
    //at the end, we use it to tell OpenAL how many buffers to queue up.
    ALsizei i;

    //Clear our errors.
    alGetError();

    //Rewind the source and clear the buffer queue.
    alSourcei(source, AL_BUFFER, 0);

    //Fill all of our buffers with data from the streamed file.
    for (i = 0; i < NUM_BUFFERS; i++)
    {
        //Fill our portable memory buffer (memBuf) with exactly the right amount of data from the sound file.
        //the soundfile file knows where it's read position is; so each time we do this, it buffers the next chunk of
        //data.
        //slen is equal to the number of audio frames we read.
        sf_count_t slen = sf_readf_short(file, memBuf,  BUFFER_SIZE);

        //for if we reach the end of the file.
        if (slen < 1){break;}

        //Calculate how much memory we buffered (in bytes). We need to pass this to OpenAL when we hand off the data from memBuf
        //and into OpenAL.
        slen *= info.channels * (sf_count_t)sizeof(short);

        //Buffer the data into OpenAl.
        alBufferData(buffers[i], format, memBuf, (ALsizei)slen, info.samplerate);

    }
    if (alGetError() != AL_NO_ERROR) {
        std::cerr << "Error buffering for data before playback. " << std::endl;
    }

    //At this point, all of our buffers should have been sequentially filled with the first chunks of the audio file
    //, and passed over to openAL. We also checked that nothing went wrong.

    //Queue the buffers, and begin playback.
    alSourceQueueBuffers(source, i, buffers);
    alSourcePlay(source);
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
    alGetSourcei(source, AL_SOURCE_STATE, &state);
    alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);
    if (alGetError() != AL_NO_ERROR)
    {
        throw("error checking music source state");
    }

    /* Unqueue and handle each processed buffer */
    while (processed > 0)
    {
        ALuint bufid;
        sf_count_t slen;

        alSourceUnqueueBuffers(source, 1, &bufid);
        processed--;

        /* Read the next chunk of data, refill the buffer, and queue it
         * back on the source */
        slen = sf_readf_short(file, memBuf, BUFFER_SIZE);
        if (slen > 0)
        {
            slen *= info.channels * (sf_count_t)sizeof(short);
            alBufferData(bufid, format, memBuf, (ALsizei)slen,
                info.samplerate);
            alSourceQueueBuffers(source, 1, &bufid);
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
        alGetSourcei(source, AL_BUFFERS_QUEUED, &queued);
        if (queued == 0)
            return;

        alSourcePlay(source);
        if (alGetError() != AL_NO_ERROR)
        {
            throw("error restarting music playback");
        }
    }

}



