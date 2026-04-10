//
// Created by Will on 12/04/2025.
//

#ifndef STREAM_H
#define STREAM_H
#include <cstddef>
#include <AL/al.h>
#include <AL/alc.h>
#include <sndfile.h>


class Stream {

public:
    void Play();

    void Update();

    Stream(const char* filename);
    ~Stream();



private:

    static const int NUM_BUFFERS = 4; //Four sequential buffers.
    static const int BUFFER_SIZE = 65536; // 32kb of data in each buffer.

    //Where (in the game) the stream will play from.
    ALuint source;

    //The array of  buffers that we loop through when streaming
    ALuint buffers[NUM_BUFFERS];

    //Location of the audio data to be streamed
    SNDFILE* file =nullptr;

    //Info about the audio file we are streaming
    SF_INFO info;

    //Pointer to the buffer we are currently streaming
    short* memBuf;

    //Info about the audio file we are streaming that OpenAL can understand.
    ALenum format;

    //No default constructor.
    Stream() = delete;

};



#endif //STREAM_H
