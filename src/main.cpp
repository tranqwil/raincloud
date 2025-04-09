#include "SoundDevice.h"
#include "SoundBuffer.h"
#include "SoundSource.h"
#include <stdio.h>


int main() {

	printf("Starting RainCloud!\n");

	SoundDevice* device = SoundDevice::Get();
	SoundBuffer* buffer = SoundBuffer::Get();

	ALuint track = buffer->AddSoundEffect("../samples/song.wav");

	SoundSource speaker;

	speaker.SetGain(1.0);
	speaker.SetPitch(.9f);
	speaker.SetPosition(0.0f, 0.0f, 0.0f);
	speaker.Play(track);

	while (true)
	{
		//loop
	}



	return 0;

}
