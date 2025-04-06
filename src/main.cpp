#include "SoundDevice.h"
#include "SoundBuffer.h"
#include "SoundSource.h"
#include <stdio.h>


int main() {

	printf("Starting mistAL!\n");

	SoundDevice* device = SoundDevice::Get();
	SoundBuffer* buffer = SoundBuffer::Get();

	ALuint track = buffer->AddSoundEffect("../Samples/song.wav");

	SoundSource speaker;

	speaker.SetGain(1.0);
	speaker.SetPitch(1.1);
	speaker.SetPosition(-2.0f, 5.0f, 0.0f);
	speaker.Play(track);



	return 0;

}
