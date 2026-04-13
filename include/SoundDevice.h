#ifndef SOUNDDEVICE_H
#define SOUNDDEVICE_H
#include <memory>

#include "RaincloudAPI.h"

namespace Raincloud
{
	class RAINCLOUD_API SoundDevice
	{

	public:



		static SoundDevice* Get();

		int GetSampleRate() const;

	private:
		SoundDevice();
		~SoundDevice();

		struct Impl;
		std::unique_ptr<Impl> m_Impl;

	};

}



#endif

