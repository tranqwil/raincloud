#include <AL/al.h>
#include <AL/alc.h>
#include <iostream>

int main() {
	ALCdevice* device = alcOpenDevice(nullptr);// open default
	if (!device) {
		std::cerr << "Failed to open OpenAL device." << std::endl;
		return 1;
	}

	ALCcontext* context = alcCreateContext(device, nullptr);
	alcMakeContextCurrent(context);

	std::cout << "OpenAL device initialized!" << std::endl;

	alcDestroyContext(context);
	alcCloseDevice(device);
	return 0;
}
