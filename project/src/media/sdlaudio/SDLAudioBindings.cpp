#include <SDL_audio.h>
#include <vector>
#include <map>
#include <algorithm>
//TODO: ADD AN ACTUAL GODDAMN AUDIO MIXER AAAAAAAAAA
//#include <SDL_mixer.h>

using namespace std;

namespace lime
{
	bool isInitialised;
	const char* defaultDeviceName;
	vector<SDL_AudioSpec> desiredSpec;
	map<int, SDL_AudioSpec> obitanedSpec;

	/*
	 * only changes the volume of the audio and mixes destination with source
	 * and some small stuff to meet the desired SDL_AudioSpec.
	 * @param formatID id of a desiredSpec object from the desiredSpec vector.
	 * @param volume volume is ranged from 0 to 128 int, not from 0 to 1 float, but pretty much similar.
	 */
	void sdl_audio_mix_audio(Uint8* destination, const Uint8* source, int formatID, Uint32 length, int volume)
	{
		SDL_AudioFormat format;
		SDL_MixAudioFormat(destination, source, format, length, volume);
	}

	const char* sdl_audio_get_device_status(int deviceID)
	{
		SDL_AudioStatus meeeem = SDL_GetAudioDeviceStatus(deviceID);
		switch (meeeem)
		{
			case SDL_AUDIO_PLAYING:
				return "playing";
			case SDL_AUDIO_PAUSED:
				return "paused";
			case SDL_AUDIO_STOPPED:
				return "stopped";
		}
	}

	int sdl_audio_add_to_queue(int deviceID, const void* data, Uint32 length)
	{
		return SDL_QueueAudio(deviceID, data, length);
	}

	void sdl_audio_clear_queue(int deviceID)
	{
		SDL_ClearQueuedAudio(deviceID);
	}

	Uint32 sdl_audio_remove_from_queue(int deviceID, void* data, Uint32 length)
	{
		return SDL_DequeueAudio(deviceID, data, length);
	}

	Uint32 sdl_audio_get_queue_size(int deviceID)
	{
		return SDL_GetQueuedAudioSize(deviceID);
	}

	const char* sdl_audio_get_device_supported_format(int deviceID, bool isCapture)
	{
		if (obitanedSpec.count(deviceID))
		{
			switch (obitanedSpec.at(deviceID).format)
			{
				case AUDIO_S8:
					return "SInt8";
				case AUDIO_U8:
					return "UInt8";
				case AUDIO_S16:
					return "SInt16";
				case AUDIO_U16:
					return "UInt16";
				case AUDIO_S32:
					return "SInt32";
				case AUDIO_F32:
					return "Float";
				default:
					return "NaN";
			}
		}
		else
		{
			SDL_AudioSpec datSpeek;
			SDL_GetAudioDeviceSpec(deviceID, isCapture, &datSpeek);
			obitanedSpec.at(deviceID) = datSpeek;
			switch (datSpeek.format)
			{
				case AUDIO_S8:
					return "SInt8";
				case AUDIO_U8:
					return "UInt8";
				case AUDIO_S16:
					return "SInt16";
				case AUDIO_U16:
					return "UInt16";
				case AUDIO_S32:
					return "SInt32";
				case AUDIO_F32:
					return "Float";
				default:
					return "NaN";
			}
		}
	}

	int sdl_audio_get_device_max_channels(int deviceID, bool isCapture)
	{
		if (obitanedSpec.count(deviceID))
			return obitanedSpec.at(deviceID).channels;
		else
		{
			SDL_AudioSpec datSpeek;
			SDL_GetAudioDeviceSpec(deviceID, isCapture, &datSpeek);
			obitanedSpec.at(deviceID) = datSpeek;
			return datSpeek.channels;
		}
	}

	int sdl_audio_get_device_max_freq(int deviceID, bool isCapture)
	{
		if (obitanedSpec.count(deviceID))
			return obitanedSpec.at(deviceID).freq;
		else
		{
			SDL_AudioSpec datSpeek;
			SDL_GetAudioDeviceSpec(deviceID, isCapture, &datSpeek);
			obitanedSpec.at(deviceID) = datSpeek;
			return datSpeek.freq;
		}
	}

	int sdl_audio_initialise(const char* backendName)
	{
		return SDL_AudioInit(backendName);
	}


	void sdl_audio_change_backend(const char* backendName)
	{
		if (!isInitialised) {}
		else
		{
			SDL_AudioQuit();
			SDL_AudioInit(backendName);
		}
	}

	void sdl_audio_clear_default_device_name()
	{
		SDL_free((void*)defaultDeviceName);
	}

	/*
	 * !!! Fills the obitanedSpec of ID 0 with specs of the default device !!!
	 * ALSO don't forget to call sdl_audio_clear_default_device_name after you don't need it.
	 * it doesn't get removed automatically, it's a pointer.
	 */
	const char* sdl_audio_get_default_device_info(bool isCapture, bool forceNameUpdate)
	{
		if (!defaultDeviceName || forceNameUpdate)
		{
			char* mar;
			SDL_GetDefaultAudioInfo(&mar, &obitanedSpec.at(0), isCapture);
			defaultDeviceName = mar;
			return mar;
		}
		else
			return defaultDeviceName;
	}

	const char* sdl_audio_get_cur_backend()
	{
		return SDL_GetCurrentAudioDriver();
	}

	int sdl_audio_get_available_backend_number()
	{
		return SDL_GetNumAudioDrivers();
	}

	int sdl_audio_get_available_device_number(bool isCapture)
	{
		return SDL_GetNumAudioDevices(isCapture);
	}

	const char* sdl_audio_get_device_name(int deviceID, bool isCapture)
	{
		return SDL_GetAudioDeviceName(deviceID, isCapture);
	}

	void sdl_audio_pause_unpause_device(int deviceID, bool pauseOn)
	{
		SDL_PauseAudioDevice(deviceID, pauseOn);
	}

	/*
	 * obitaned spec is mapped to device ID
	 *
	 * @param deviceID set to NULL/0 to get default device.
	 * @param isCapture only set to non-zero if you're opening a recording device.
	 *
	 * @return opened audio device ID, always >= 2.
	 */
	int sdl_audio_open_device_by_id(int deviceID, bool isCapture, int desiredSpecID = 1)
	{
		const char* deviceName;

		if (deviceID)
			deviceName = SDL_GetAudioDeviceName(deviceID, isCapture);
		else
			deviceName = NULL;
		return sdl_audio_open_device_by_name(deviceName, isCapture, desiredSpecID);
	}

	/*
	 * obitaned spec is mapped to device ID
	 *
	 * @param deviceName set to NULL/0 to get default device.
	 * @param isCapture only set to non-zero if you're opening a recording device.
	 *
	 * @return opened audio device ID, always >= 2.
	 */
	int sdl_audio_open_device_by_name(const char* deviceName, bool isCapture, int desiredSpecID = 1)
	{
		int devID;
		SDL_AudioSpec datSpec;
		devID = SDL_OpenAudioDevice(deviceName, isCapture, &desiredSpec[desiredSpecID], &datSpec, 0);
		obitanedSpec.at(devID) = datSpec;
		return devID;
	}

	void sdl_audio_close_device(int deviceID)
	{
		obitanedSpec.erase(deviceID);
		SDL_CloseAudioDevice(deviceID);
	}

	/*
	 *  @param specID ID of the SDL_AudioSpec structure to add/change, 1 by default.
	 *	@param format can be 0 (signed 8 bit), 1 (unsigned 8 bit), 2 (signed 16 bit), 3 (unsigned 16 bit), 4 (signed 32 bit), 5 (float 32 bit), set as 32 bit float by default.
	 */
	void sdl_audio_create_desired_audio_spec(int freq, Uint8 format, Uint8 channels, Uint16 samples, Uint32 size)
	{
		SDL_AudioSpec daspec;
		daspec.freq = freq;
		switch (format)
		{
			case 0:
				daspec.format = AUDIO_S8;
				break;
			case 1:
				daspec.format = AUDIO_U8;
				break;
			case 2:
				daspec.format = AUDIO_S16;
				break;
			case 3:
				daspec.format = AUDIO_U16;
				break;
			case 4:
				daspec.format = AUDIO_S32;
				break;
			case 5:
				daspec.format = AUDIO_F32;
				break;
			default:
				daspec.format = AUDIO_F32;
		}
		daspec.channels = channels;
		daspec.samples = samples;
		daspec.size = size;
		daspec.userdata = NULL;
		daspec.callback = NULL;
   		desiredSpec.push_back(daspec);
	}

	void sdl_audio_update_desired_spec(int specID, int freq, Uint8 format, Uint8 channels, Uint16 samples, Uint32 size)
	{
		if (find(desiredSpec.begin(), desiredSpec.end(), specID) != desiredSpec.end())
		{
			desiredSpec[specID].freq = freq;
			switch (format)
			{
				case 0:
					desiredSpec[specID].format = AUDIO_S8;
					break;
				case 1:
					desiredSpec[specID].format = AUDIO_U8;
					break;
				case 2:
					desiredSpec[specID].format = AUDIO_S16;
					break;
				case 3:
					desiredSpec[specID].format = AUDIO_U16;
					break;
				case 4:
					desiredSpec[specID].format = AUDIO_S32;
					break;
				case 5:
					desiredSpec[specID].format = AUDIO_F32;
					break;
				default:
					desiredSpec[specID].format = AUDIO_F32;
			}
			desiredSpec[specID].channels = channels;
			desiredSpec[specID].samples = samples;
			desiredSpec[specID].size = size;
			desiredSpec[specID].userdata = NULL;
			desiredSpec[specID].callback = NULL;
		}
	}

	void sdl_audio_remove_desired_spec(int specID)
	{
		if (specID < desiredSpec.size())
			desiredSpec.erase(desiredSpec.begin() + specID);
	}
}

extern "C" int lime_sdl_audio_register_prims() {

	return 0;

}