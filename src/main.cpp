#include "SoundDevice.h"
#include "SoundBuffer.h"
#include "SoundSource.h"
#include "Stream.h"
#include <stdio.h>


int main() {

	printf("Starting RainCloud!\n");

	SoundDevice* device = SoundDevice::Get();
	Stream stream ("../samples/song.wav");

	stream.Play();


	while (true)
	{
		stream.Update();
	}



	return 0;

}
