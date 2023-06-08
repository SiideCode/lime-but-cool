#include <SDL_audio.h>
#include <vector>
#include <map>
#include <algorithm>

//unused, HL only
//#include <system/CFFI.h>
//#include <system/CFFIPointer.h>

#include <utils/ArrayBufferView.h>
//TODO: ADD AN ACTUAL GODDAMN AUDIO MIXER AAAAAAAAAA
//#include <SDL_mixer.h>

//SDL Audio is somewhat easier to link to haxe than OpenAL honestly. huh.

using namespace std;

namespace lime
{
	const char* defaultDeviceName;
	vector<SDL_AudioSpec> desiredSpec;
	map<int, SDL_AudioSpec> obitanedSpec;

	// TODO: Make volume be float that is from 0 to 1, but then converted to a closest int in range of 0-128
	// where 0 is 0 float and 128 is 1 float.
	// or just remake it entirely
	// or use SDL_mixer
	// or make like ~64 volume levels that are seamlessly changed
	// or 128 volume levels
	/*
	 * only changes the volume of the audio and mixes destination with source
	 * and some small stuff to meet the desired SDL_AudioSpec.
	 * @param formatID id of a desiredSpec object from the desiredSpec vector.
	 * @param length will be treated as Uint32.
	 * @param volume volume is ranged from 0 to 128 int, not from 0 to 1 float, but pretty much similar.
	 */
	void sdl_audio_mix_audio(value destination, value source, int formatID, value length, int volume)
	{
		SDL_AudioFormat format;

		switch (formatID)
		{
			case 0:
				format = AUDIO_S8;
				break;
			case 1:
				format = AUDIO_U8;
				break;
			case 2:
				format = AUDIO_S16;
				break;
			case 3:
				format = AUDIO_U16;
				break;
			case 4:
				format = AUDIO_S32;
				break;
			case 5:
				format = AUDIO_F32;
				break;
			default:
				format = AUDIO_F32;
		}

		Uint32 realLen;
		memcpy_s(&realLen, sizeof(Uint32), (Uint32*) val_data(length), sizeof(Uint32));

		printf_s("Copied bytes from HXCPP UInt to a Uint32 realLen variable. The copied value is %d", realLen);

		ArrayBufferView bufferShit(destination);

		ArrayBufferView bufferShitTwoElectricBoogaloo(source);

		SDL_MixAudioFormat(bufferShit.buffer->b, bufferShitTwoElectricBoogaloo.buffer->b, format, realLen, volume);
	}

	value sdl_audio_get_device_status(value deviceID)
	{
		Uint32 realId;
		memcpy_s(&realId, sizeof(Uint32), (Uint32*) val_data(deviceID), sizeof(Uint32));

		SDL_AudioStatus meeeem = SDL_GetAudioDeviceStatus(realId);
		switch (meeeem)
		{
			case SDL_AUDIO_PLAYING:
				return alloc_string("playing");
			case SDL_AUDIO_PAUSED:
				return alloc_string("paused");
			case SDL_AUDIO_STOPPED:
				return alloc_string("stopped");
		}
	}

	/*
	 *	@param length will be treated as Uint32
	 */
	int sdl_audio_add_to_queue(value deviceID, value data, value length)
	{
		Uint32 realId;
		memcpy_s(&realId, sizeof(Uint32), (Uint32*) val_data(deviceID), sizeof(Uint32));

		Uint32 realLen;
		memcpy_s(&realLen, sizeof(Uint32), (Uint32*) val_data(length), sizeof(Uint32));

		ArrayBufferView bufferShit(data);
		return SDL_QueueAudio(realId, bufferShit.buffer->b, realLen);
	}

	void sdl_audio_clear_queue(value deviceID)
	{
		Uint32 realId;
		memcpy_s(&realId, sizeof(Uint32), (Uint32*) val_data(deviceID), sizeof(Uint32));

		SDL_ClearQueuedAudio(realId);
	}

	int sdl_audio_remove_from_queue(value deviceID, value data, value length)
	{
		Uint32 realId;
		memcpy_s(&realId, sizeof(Uint32), (Uint32*) val_data(deviceID), sizeof(Uint32));

		Uint32 realLen;
		memcpy_s(&realLen, sizeof(Uint32), (Uint32*) val_data(length), sizeof(Uint32));

		ArrayBufferView bufferShit(data);
		return SDL_DequeueAudio(realId, bufferShit.buffer->b, realLen);
	}

	int sdl_audio_get_queue_size(value deviceID)
	{
		Uint32 realId;
		memcpy_s(&realId, sizeof(Uint32), (Uint32*) val_data(deviceID), sizeof(Uint32));

		return SDL_GetQueuedAudioSize(realId);
	}

	value sdl_audio_get_device_supported_format(int deviceIndex, bool isCapture)
	{
		if (obitanedSpec.count(deviceIndex))
		{
			switch (obitanedSpec.at(deviceIndex).format)
			{
				case AUDIO_S8:
					return alloc_string("Int8");
				case AUDIO_U8:
					return alloc_string("UInt8");
				case AUDIO_S16:
					return alloc_string("Int16");
				case AUDIO_U16:
					return alloc_string("UInt16");
				case AUDIO_S32:
					return alloc_string("Int32");
				case AUDIO_F32:
					return alloc_string("Float");
				default:
					return alloc_string("NaN");
			}
		}
		else
		{
			SDL_AudioSpec datSpeek;
			SDL_GetAudioDeviceSpec(deviceIndex, isCapture, &datSpeek);
			obitanedSpec.at(deviceIndex) = datSpeek;
			switch (datSpeek.format)
			{
				case AUDIO_S8:
					return alloc_string("Int8");
				case AUDIO_U8:
					return alloc_string("UInt8");
				case AUDIO_S16:
					return alloc_string("Int16");
				case AUDIO_U16:
					return alloc_string("UInt16");
				case AUDIO_S32:
					return alloc_string("Int32");
				case AUDIO_F32:
					return alloc_string("Float");
				default:
					return alloc_string("NaN");
			}
		}
	}

	int sdl_audio_get_device_max_channels(int deviceIndex, bool isCapture)
	{
		if (obitanedSpec.count(deviceIndex))
			return obitanedSpec.at(deviceIndex).channels;
		else
		{
			SDL_AudioSpec datSpeek;
			SDL_GetAudioDeviceSpec(deviceIndex, isCapture, &datSpeek);
			obitanedSpec.at(deviceIndex) = datSpeek;
			return datSpeek.channels;
		}
	}

	int sdl_audio_get_device_max_freq(int deviceIndex, bool isCapture)
	{
		if (obitanedSpec.count(deviceIndex))
			return obitanedSpec.at(deviceIndex).freq;
		else
		{
			SDL_AudioSpec datSpeek;
			SDL_GetAudioDeviceSpec(deviceIndex, isCapture, &datSpeek);
			obitanedSpec.at(deviceIndex) = datSpeek;
			return datSpeek.freq;
		}
	}

	int sdl_audio_initialise(HxString backendName)
	{
		return SDL_AudioInit(backendName.c_str());
	}


	void sdl_audio_change_backend(HxString backendName)
	{
		SDL_AudioQuit();
		SDL_AudioInit(backendName.c_str());
	}

	void sdl_audio_quit()
	{
		SDL_AudioQuit();
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
	value sdl_audio_get_default_device_info(bool isCapture, bool forceNameUpdate)
	{
		if (!defaultDeviceName || forceNameUpdate)
		{
			char* mar;
			SDL_GetDefaultAudioInfo(&mar, &obitanedSpec.at(0), isCapture);
			defaultDeviceName = mar;
			return alloc_string(mar);
		}
		else
			return alloc_string(defaultDeviceName);
	}

	value sdl_audio_get_cur_backend()
	{
		return alloc_string(SDL_GetCurrentAudioDriver());
	}

	int sdl_audio_get_available_backends_number()
	{
		return SDL_GetNumAudioDrivers();
	}

	int sdl_audio_get_available_devices_number(bool isCapture)
	{
		return SDL_GetNumAudioDevices(isCapture);
	}

	value sdl_audio_get_device_name(int deviceIndex, bool isCapture)
	{
		return alloc_string(SDL_GetAudioDeviceName(deviceIndex, isCapture));
	}

	void sdl_audio_pause_unpause_device(value deviceID, bool pauseOn)
	{
		Uint32 realId;
		memcpy_s(&realId, sizeof(Uint32), (Uint32*) val_data(deviceID), sizeof(Uint32));

		SDL_PauseAudioDevice(realId, pauseOn);
	}

	/*
	 * obitaned spec is mapped to device ID
	 *
	 * @param deviceID set to NULL/0 to get default device.
	 * @param isCapture only set to non-zero if you're opening a recording device.
	 *
	 * @return opened audio device ID, always >= 2.
	 */
	value sdl_audio_open_device_by_id(int deviceIndex, bool isCapture, int desiredSpecID)
	{
		const char* deviceName;

		if (deviceIndex)
			deviceName = SDL_GetAudioDeviceName(deviceIndex, isCapture);
		else
			deviceName = NULL;

		int devID;
		SDL_AudioSpec datSpec;
		devID = SDL_OpenAudioDevice(deviceName, isCapture, &desiredSpec[desiredSpecID], &datSpec, 0);
		obitanedSpec.at(devID) = datSpec;
		return alloc_int32(devID);
	}

	/*
	 * obitaned spec is mapped to device ID
	 *
	 * @param deviceName set to NULL/0 to get default device.
	 * @param isCapture only set to non-zero if you're opening a recording device.
	 *
	 * @return opened audio device ID, always >= 2.
	 */
	value sdl_audio_open_device_by_name(HxString deviceName, bool isCapture, int desiredSpecID)
	{
		int devID;
		SDL_AudioSpec datSpec;
		devID = SDL_OpenAudioDevice(deviceName.c_str(), isCapture, &desiredSpec[desiredSpecID], &datSpec, 0);
		obitanedSpec.at(devID) = datSpec;
		return alloc_int32(devID);
	}

	void sdl_audio_close_device(value deviceID)
	{
		Uint32 realId;
		memcpy_s(&realId, sizeof(Uint32), (Uint32*) val_data(deviceID), sizeof(Uint32));

		obitanedSpec.erase(realId);
		SDL_CloseAudioDevice(realId);
	}

	/*
	 *  @param specID ID of the SDL_AudioSpec structure to add, 1 by default.
	 *	@param format can be 0 (signed 8 bit), 1 (unsigned 8 bit), 2 (signed 16 bit), 3 (unsigned 16 bit), 4 (signed 32 bit), 5 (float 32 bit), set as 32 bit float by default.
	 */
	int sdl_audio_create_desired_spec(int freq, value format, bool channels, value samples, value size)
	{
		SDL_AudioSpec daspec;
		daspec.freq = freq;

		Uint8 daFormat;
		memcpy_s(&daFormat, sizeof(Uint8), (Uint8*) val_data(format), sizeof(Uint8));

		switch (daFormat)
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

		if (channels)
			daspec.channels = 2;
		else
			daspec.channels = 1;

		Uint16 daSamples;
		memcpy_s(&daSamples, sizeof(Uint16), (Uint16*) val_data(samples), sizeof(Uint16));

		daspec.samples = daSamples;

		Uint32 daSize;
		memcpy_s(&daSize, sizeof(Uint32), (Uint32*) val_data(size), sizeof(Uint32));

		daspec.size = daSize;

		daspec.userdata = NULL;
		daspec.callback = NULL;
   		desiredSpec.push_back(daspec);
		return desiredSpec.size();
	}

	void sdl_audio_update_desired_spec(int specID, int freq, value format, bool channels, value samples, value size)
	{
		if (desiredSpec[specID].samples)
		{
			desiredSpec[specID].freq = freq;

			Uint8 daFormat;
			memcpy_s(&daFormat, sizeof(Uint8), (Uint8*) val_data(format), sizeof(Uint8));

			switch (daFormat)
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

			if (channels)
				desiredSpec[specID].channels = 2;
			else
				desiredSpec[specID].channels = 1;

			Uint16 daSamples;
			memcpy_s(&daSamples, sizeof(Uint16), (Uint16*) val_data(samples), sizeof(Uint16));

			desiredSpec[specID].samples = daSamples;

			Uint32 daSize;
			memcpy_s(&daSize, sizeof(Uint32), (Uint32*) val_data(size), sizeof(Uint32));

			desiredSpec[specID].size = daSize;
		}
	}

	int sdl_audio_remove_desired_spec(int specID)
	{
		if (desiredSpec[specID].samples)
		{
			desiredSpec.erase(desiredSpec.begin() + specID);
			return desiredSpec.size();
		}
		else
			return NULL;
	}

	DEFINE_PRIME5v(sdl_audio_mix_audio);
	DEFINE_PRIME1(sdl_audio_get_device_status);
	DEFINE_PRIME3(sdl_audio_add_to_queue);
    DEFINE_PRIME1v(sdl_audio_clear_queue);
	DEFINE_PRIME3(sdl_audio_remove_from_queue);
	DEFINE_PRIME1(sdl_audio_get_queue_size);
	DEFINE_PRIME2(sdl_audio_get_device_supported_format);
	DEFINE_PRIME2(sdl_audio_get_device_max_channels);
	DEFINE_PRIME2(sdl_audio_get_device_max_freq);
	DEFINE_PRIME1(sdl_audio_initialise);
	DEFINE_PRIME1v(sdl_audio_change_backend);
	DEFINE_PRIME0v(sdl_audio_quit);
	DEFINE_PRIME0v(sdl_audio_clear_default_device_name);
	DEFINE_PRIME2(sdl_audio_get_default_device_info);
	DEFINE_PRIME0(sdl_audio_get_cur_backend);
	DEFINE_PRIME0(sdl_audio_get_available_backends_number);
	DEFINE_PRIME1(sdl_audio_get_available_devices_number);
	DEFINE_PRIME2(sdl_audio_get_device_name);
	DEFINE_PRIME2v(sdl_audio_pause_unpause_device);
	DEFINE_PRIME3(sdl_audio_open_device_by_id);
	DEFINE_PRIME3(sdl_audio_open_device_by_name);
	DEFINE_PRIME1v(sdl_audio_close_device);
	DEFINE_PRIME5(sdl_audio_create_desired_spec);
	DEFINE_PRIME6v(sdl_audio_update_desired_spec);
	DEFINE_PRIME1(sdl_audio_remove_desired_spec);
}

extern "C" int lime_sdl_audio_register_prims() {

	return 0;

}