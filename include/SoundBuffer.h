#ifndef SOUNDBUFFER_H
#define SOUNDBUFFER_H




#include <vector>
#include "RaincloudAPI.h"
#include <RaincloudTypes.h>

namespace Raincloud
{
	class RAINCLOUD_API SoundBuffer
	{
	public:

		static SoundBuffer* Get();

		BufferHandle AddSoundEffect(const char* filename);

		bool RemoveSoundEffect(BufferHandle& buffer);

	private:
		SoundBuffer();
		~SoundBuffer();

		std::vector<BufferHandle> pSoundEffectBuffers;

	};

}


#endif
