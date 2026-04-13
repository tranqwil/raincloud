#ifndef SOUNDSOURCE_H
#define SOUNDSOURCE_H

#include <memory>
#include <RaincloudTypes.h>
#include "RaincloudAPI.h"


namespace Raincloud
{
	 enum class FadeState {
		None,
		FadingIn,
		FadingOut,
		DoneFadingIn,
		DoneFadingOut
	};


	class RAINCLOUD_API SoundSource
	{
	public:
		SoundSource();
		~SoundSource();

		void Play(BufferHandle bufferToPlay);

		void Stop();

		void Update(float dt);

		void SetIsLooping(bool loop);

		void SetPitch(float pitch);

		void SetGain(float gain);

		void SetPosition(float x, float y, float z);

		void SetVelocity(float x, float y, float z);

		void SetFade(FadeState state, float fadeDuration);



	private:

		struct Impl;
		std::unique_ptr<Impl> m_Impl;

	};

}




#endif


