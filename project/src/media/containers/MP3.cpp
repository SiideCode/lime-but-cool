//#define MINIMP3_NO_STDIO
//#define MINIMP3_FLOAT_OUTPUT
#define MINIMP3_ONLY_MP3
#define MINIMP3_IMPLEMENTATION
#ifdef MINIMP3_FLOAT_OUTPUT
#define SAMPLE_BITS 32
#else
#define SAMPLE_BITS 16
#endif
#include <minimp3_ex.h>
#include <media/containers/MP3.h>
//no std i guess.
#include <system/System.h>

namespace lime
{
	bool MP3::Decode (Resource *resource, AudioBuffer *audioBuffer)
	{
		mp3dec_t mp3dec;
		mp3dec_file_info_t mp3info;
		if (resource->data)
		{
			int result = mp3dec_load_buf(&mp3dec, resource->data->b, resource->data->length/*-1*/, &mp3info, NULL, NULL);
			LOG_SOUND("well, it loaded, the return code is %d ", result);
			if (result < 0)
			{
				LOG_SOUND("there was an error, error code %d ", result);
				return false;
			}
		}
		else
		if (resource->path)
		{
			int result = mp3dec_load(&mp3dec, resource->path, &mp3info, NULL, NULL);
			if (result < 0)
			{
				LOG_SOUND("there was an error, error code %d ", result);
				return false;
			}
		}
		else
		{
			LOG_SOUND("why there no data... ");
			return false;
		}

		audioBuffer->bitrate = mp3info.avg_bitrate_kbps;
		audioBuffer->sampleRate = mp3info.hz;
		audioBuffer->bitsPerSample = SAMPLE_BITS;
		audioBuffer->channels = mp3info.channels;

		audioBuffer->data->Resize(mp3info.samples * sizeof(mp3d_sample_t));

		LOG_SOUND("resized data buffer to %zd ", mp3info.samples * sizeof(mp3d_sample_t));

		memcpy_s(audioBuffer->data->buffer->b, mp3info.samples * sizeof(mp3d_sample_t), mp3info.buffer, mp3info.samples * sizeof(mp3d_sample_t));

		LOG_SOUND("moved %zd bytes of data to %d length buffer, the amount of samples was %zd, bits amount %zd ", sizeof(mp3d_sample_t) * mp3info.samples, audioBuffer->data->buffer->length, mp3info.samples, SAMPLE_BITS * mp3info.samples);

		delete mp3info.buffer;

		LOG_SOUND("freed audio buffer allocated memory");

		return true;
	}
}