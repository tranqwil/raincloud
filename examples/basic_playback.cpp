//
// Created by WillChapman on 10/04/2026.
//

#include <iostream>
#include <thread>

#include "Raincloud.h"



int main()
{
    std::cout << "Hello World!\n";

    Raincloud::SoundDevice* device = Raincloud::SoundDevice::Get();
    Raincloud::SoundBuffer* buffer = Raincloud::SoundBuffer::Get();
    auto track = buffer->AddSoundEffect("resources/song.wav");

    Raincloud::SoundSource speaker;
    speaker.SetGain(1.0f);
    speaker.SetPitch(1.1f);
    speaker.SetIsLooping(true);
    speaker.Play(track);

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    return 0;

}