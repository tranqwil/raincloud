#include "SoundDevice.h"
#include "SoundBuffer.h"
#include "SoundSource.h"
#include <stdio.h>


int main() {

	printf("Starting mistAL!\n");

	SoundDevice* device = SoundDevice::Get();
	SoundBuffer* buffer = SoundBuffer::Get();

	ALuint track = buffer->AddSoundEffect("F:\\Ableton Projects\\SMACK Project\\Samples\\Processed\\Consolidate\\22-10-24_TextMyPhone [2024-10-22 193114].wav");


	SoundSource speaker;

	speaker.SetGain(1.0);
	speaker.SetPitch(1.1);
	speaker.Play(track);

}
