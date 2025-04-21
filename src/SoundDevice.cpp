#include "SoundDevice.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#include <stdio.h>
#include <iostream>

SoundDevice* SoundDevice::Get()
{
	static SoundDevice* soundDevice = new SoundDevice();
	return soundDevice;
}

SoundDevice::SoundDevice()
{
	//Get Default Windows Device
	pALCdevice = alcOpenDevice(nullptr);
	if(!pALCdevice)
	{
		std::cerr << "SoundDevice::SoundDevice() - Failed to get default device";
	}

	if (alcIsExtensionPresent(pALCdevice, "ALC_SOFT_HRTF") == AL_FALSE) {
        std::cout << "HRTF not supported.\n";}

	//HRTF
	ALCint attrs[] = {
    ALC_HRTF_SOFT, ALC_TRUE,
    0};


	//Create ALC Context
	pALCcontext = alcCreateContext(pALCdevice, attrs);
	if (!pALCcontext)
	{
		std::cerr << "SoundDevice::SoundDevice() - Failed to create context";
	}

	//Make context the current context
	if (!alcMakeContextCurrent(pALCcontext))
	{
		std::cerr << "SoundDevice::SoundDevice() - Failed to make context current";
	}

	const ALCchar* name = nullptr;
	if (alcIsExtensionPresent(pALCdevice, "ALC_ENUMERATE_ALL_EXT"))
	{
		name = alcGetString(pALCdevice, ALC_ALL_DEVICES_SPECIFIER);
	}
	if (!name || alcGetError(pALCdevice) != ALC_NO_ERROR)
	{
		name = alcGetString(pALCdevice, ALC_DEVICE_SPECIFIER);
	}
	printf("Opened \"%s\"\n", name);

	ALCint hrtfEnabled;
    alcGetIntegerv(pALCdevice, ALC_HRTF_SOFT, 1, &hrtfEnabled);
    std::cout << "HRTF enabled: " << (hrtfEnabled ? "Yes" : "No") << std::endl;





}

SoundDevice::~SoundDevice()
{
	if (!alcMakeContextCurrent(nullptr))
	{
		std::cerr << "SoundDevice::~SoundDevice() - Failed to make context current";
	}

	alcDestroyContext(pALCcontext);
	if (pALCcontext) 
	{
		std::cerr << "SoundDevice::~SoundDevice() - Failed to destroy context";
	}

	if (!alcCloseDevice(pALCdevice))
	{
		std::cerr << "SoundDevice::~SoundDevice() - Failed to close device";
	}
}

