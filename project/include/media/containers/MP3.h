#pragma once
#include <media/AudioBuffer.h>
#include <utils/Resource.h>

namespace lime
{
	class MP3
	{
		public:
			static bool MP3::Decode(Resource *resource, AudioBuffer *audioBuffer);
	};
}