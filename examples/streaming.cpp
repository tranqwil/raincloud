//
// Created by WillChapman on 10/04/2026.
//

#include <iostream>
#include <thread>

#include "Raincloud.h"



int main()
{
    Raincloud::SoundDevice* device = Raincloud::SoundDevice::Get();

    std::cout << "Raincloud Example Streaming.\n";

    Raincloud::Stream stream ("resources/song.wav");
    stream.Play();

    while (true) {
        stream.Update();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }




}