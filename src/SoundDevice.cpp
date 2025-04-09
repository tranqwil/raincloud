#include "SoundDevice.h"
#include <AL/al.h>
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

	//Create ALC Context
	pALCcontext = alcCreateContext(pALCdevice, nullptr);
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

