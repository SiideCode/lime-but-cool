#include <SDL_audio.h>
#include <vector>
#include <map>
#include <algorithm>

#include <utils/ValueTools.h>

//unused, HL only
//#include <system/CFFI.h>
//#include <system/CFFIPointer.h>

#include <utils/ArrayBufferView.h>
//TODO: ADD AN ACTUAL GODDAMN AUDIO MIXER AAAAAAAAAA
#include <SDL_mixer.h>

//SDL Audio is somewhat easier to link to haxe than OpenAL honestly. huh.

using namespace std;

using namespace lime::valueConvert;

namespace lime
{
	const char* defaultDeviceName;
	map<int, SDL_AudioSpec> desiredSpec;
	map<int, SDL_AudioSpec> obitanedSpec;

	bool sdl_mixer_open_audio_device_values_by_id()
	{

	}

	bool sdl_mixer_open_audio_device_values_by_name()
	{

	}

	bool sdl_mixer_open_audio_device_spec_by_id(int desiredSpecID, int deviceIndex)
	{
		return Mix_OpenAudioDevice(desiredSpec.at(desiredSpecID).freq, desiredSpec.at(desiredSpecID).format, desiredSpec.at(desiredSpecID).channels, (desiredSpec.at(desiredSpecID).samples / desiredSpec.at(desiredSpecID).channels), SDL_GetAudioDeviceName(deviceIndex, false), 0);
	}

	bool sdl_mixer_open_audio_device_spec_by_name(int desiredSpecID, HxString deviceName)
	{
		return Mix_OpenAudioDevice(desiredSpec.at(desiredSpecID).freq, desiredSpec.at(desiredSpecID).format, desiredSpec.at(desiredSpecID).channels, (desiredSpec.at(desiredSpecID).samples / desiredSpec.at(desiredSpecID).channels), deviceName.__s, 0);
	}

	bool sdl_mixer_open_default_audio_device_spec(int desiredSpecID)
	{
		return Mix_OpenAudio(desiredSpec.at(desiredSpecID).freq, desiredSpec.at(desiredSpecID).format, desiredSpec.at(desiredSpecID).channels, (desiredSpec.at(desiredSpecID).samples / desiredSpec.at(desiredSpecID).channels));
	}

	bool sdl_mixer_open_default_audio_device_values(int freqency, int format, int channels, int samples)
	{
		SDL_AudioFormat rformat;

		switch (rformat)
		{
			case 0:
				rformat = AUDIO_S8;
				break;
			case 1:
				rformat = AUDIO_U8;
				break;
			case 2:
				rformat = AUDIO_S16;
				break;
			case 3:
				rformat = AUDIO_U16;
				break;
			case 4:
				rformat = AUDIO_S32;
				break;
			case 5:
				rformat = AUDIO_F32;
				break;
			default:
				rformat = AUDIO_F32;
		}

		return Mix_OpenAudio(freqency, rformat, channels, (samples / channels));
	}

	bool sdl_mixer_initialise(bool additionalDecoders)
	{
		return Mix_Init(additionalDecoders ? MIX_INIT_MID | MIX_INIT_MOD | MIX_INIT_OPUS : 0);
	}

	void sdl_mixer_quit()
	{
		Mix_Quit();
	}

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

		ArrayBufferView bufferShit(destination);

		ArrayBufferView bufferShitTwoElectricBoogaloo(source);

		SDL_MixAudioFormat(bufferShit.buffer->b, bufferShitTwoElectricBoogaloo.buffer->b, format, nekoValToUint32(length), volume);
	}

	value sdl_audio_get_device_status(value deviceID)
	{

		SDL_AudioStatus meeeem = SDL_GetAudioDeviceStatus(nekoValToUint32(deviceID));
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
	 *	@param length a Uint32 variable
	 */
	int sdl_audio_add_to_queue(value deviceID, value data, value length)
	{
		ArrayBufferView bufferShit(data);
		return SDL_QueueAudio(nekoValToUint32(deviceID), bufferShit.buffer->b, nekoValToUint32(length));
	}

	void sdl_audio_clear_queue(value deviceID)
	{
		SDL_ClearQueuedAudio(nekoValToUint32(deviceID));
	}

	int sdl_audio_remove_from_queue(value deviceID, value data, value length)
	{
		ArrayBufferView bufferShit(data);
		return SDL_DequeueAudio(nekoValToUint32(deviceID), bufferShit.buffer->b, nekoValToUint32(length));
	}

	int sdl_audio_get_queue_size(value deviceID)
	{
		return SDL_GetQueuedAudioSize(nekoValToUint32(deviceID));
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
			obitanedSpec.emplace(deviceIndex, datSpeek);
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
			obitanedSpec.emplace(deviceIndex, datSpeek);
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
			obitanedSpec.emplace(deviceIndex, datSpeek);
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
		SDL_PauseAudioDevice(nekoValToUint32(deviceID), pauseOn);
	}

	int sdl_audio_open_default_device(int desiredSpecID)
	{
		SDL_AudioSpec datSpec;
		int marr;

		marr = SDL_OpenAudio(&desiredSpec.at(desiredSpecID), &datSpec);

		obitanedSpec.emplace(1, datSpec);

		return marr;
	}

	void sdl_audio_close_default_device()
	{
		SDL_CloseAudio();
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
		int devID;
		SDL_AudioSpec datSpec;

		if (deviceIndex)
			deviceName = SDL_GetAudioDeviceName(deviceIndex, isCapture);
		else
			deviceName = NULL;

		devID = SDL_OpenAudioDevice(deviceName, isCapture, &desiredSpec.at(desiredSpecID), &datSpec, 0);

		obitanedSpec.emplace(devID, datSpec);

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
		devID = SDL_OpenAudioDevice(deviceName.c_str(), isCapture, &desiredSpec.at(desiredSpecID), &datSpec, 0);
		obitanedSpec.emplace(devID, datSpec);
		return alloc_int32(devID);
	}

	void sdl_audio_close_device(value deviceID)
	{
		//minimise memory operations lol
		Uint32 m = nekoValToUint32(deviceID);
		obitanedSpec.erase(m);
		SDL_CloseAudioDevice(m);
	}

	/*
	 *  @param specID ID of the SDL_AudioSpec structure to add, 1 by default.
	 *	@param format can be 0 (signed 8 bit), 1 (unsigned 8 bit), 2 (signed 16 bit), 3 (unsigned 16 bit), 4 (signed 32 bit), 5 (float 32 bit), set as 32 bit float by default.
	 */
	int sdl_audio_create_desired_spec(int freq, value format, int channels, value samples, value size)
	{
		SDL_AudioSpec daspec;
		daspec.freq = freq;

		switch (nekoValToUint8(format))
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

		daspec.samples = nekoValToUint16(samples);

		daspec.size = nekoValToUint32(size);

		daspec.userdata = NULL;
		daspec.callback = NULL;
   		desiredSpec.emplace(desiredSpec.size(), daspec);
		return desiredSpec.size();
	}

	bool sdl_audio_update_desired_spec(int specID, int freq, value format, int channels, value samples, value size)
	{
		if (desiredSpec.count(specID))
		{
			desiredSpec.at(specID).freq = freq;

			switch (nekoValToUint8(format))
			{
				case 0:
					desiredSpec.at(specID).format = AUDIO_S8;
					break;
				case 1:
					desiredSpec.at(specID).format = AUDIO_U8;
					break;
				case 2:
					desiredSpec.at(specID).format = AUDIO_S16;
					break;
				case 3:
					desiredSpec.at(specID).format = AUDIO_U16;
					break;
				case 4:
					desiredSpec.at(specID).format = AUDIO_S32;
					break;
				case 5:
					desiredSpec.at(specID).format = AUDIO_F32;
					break;
				default:
					desiredSpec.at(specID).format = AUDIO_F32;
			}

			desiredSpec.at(specID).channels = channels;

			desiredSpec.at(specID).samples = nekoValToUint16(samples);

			desiredSpec.at(specID).size = nekoValToUint32(size);

			return true;
		}
		else
			return false;
	}

	int sdl_audio_remove_desired_spec(int specID)
	{
		if (desiredSpec.count(specID))
		{
			desiredSpec.erase(specID);
			return 1;
		}
		else
			return 0;
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
	DEFINE_PRIME6(sdl_audio_update_desired_spec);
	DEFINE_PRIME1(sdl_audio_remove_desired_spec);
}

extern "C" int lime_sdl_audio_register_prims() {

	return 0;

}