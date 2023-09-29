#ifndef LIME_UI_GAMEPAD_H
#define LIME_UI_GAMEPAD_H


namespace lime {


	class Gamepad {

		public:

			static void AddMapping (const char* content);
			static const char* GetDeviceGUID (int id);
			static const char* GetDeviceName (int id);
			static int GetTypeID(int id);
			static int GetPlayerIndex(int id);
			static void SetPlayerIndex(int id, int index);
			static int GetProductID(int id);
			static int GetProductVersion(int id);
			static int GetVendorID(int id);
			static int GetFirmwareVersion(int id);
			static const char *GetSerialNumber(int id);
			static int GetNumTouchpads(int id);
			static int GetMaxTouchpadFingers(int id, int touchpad);
			static bool HasSensor(int id, int sensor);
			static bool EnableSensor(int id, int sensor, bool enable);
			static bool IsSensorEnabled(int id, int sensor);
			static bool StartRumble(int id, int lowFreq, int highFreq, int duration);
			static bool HasLED(int id);
			static bool HasRumble(int id);
			static bool HasTriggerRumble(int id);
			static bool SetLED(int id, int red, int green, int blue);
	};
}


#endif