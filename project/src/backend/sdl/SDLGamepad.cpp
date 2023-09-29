#include "SDLGamepad.h"


namespace lime {


	std::map<int, SDL_GameController*> gameControllers = std::map<int, SDL_GameController*> ();
	std::map<int, int> gameControllerIDs = std::map<int, int> ();


	bool SDLGamepad::Connect (int deviceID) {

		if (SDL_IsGameController (deviceID)) {

			SDL_GameController *gameController = SDL_GameControllerOpen (deviceID);

			if (gameController) {

				SDL_Joystick *joystick = SDL_GameControllerGetJoystick (gameController);
				int id = SDL_JoystickInstanceID (joystick);

				gameControllers[id] = gameController;
				gameControllerIDs[deviceID] = id;

				return true;

			}

		}

		return false;

	}


	bool SDLGamepad::Disconnect (int id) {

		if (gameControllers.find (id) != gameControllers.end ()) {

			SDL_GameController *gameController = gameControllers[id];
			SDL_GameControllerClose (gameController);
			gameControllers.erase (id);

			return true;

		}

		return false;

	}


	int SDLGamepad::GetInstanceID (int deviceID) {

		return gameControllerIDs[deviceID];

	}


	void Gamepad::AddMapping (const char* content) {

		SDL_GameControllerAddMapping (content);

	}


	const char* Gamepad::GetDeviceGUID (int id) {

		SDL_Joystick* joystick = SDL_GameControllerGetJoystick (gameControllers[id]);

		if (joystick) {

			char* guid = new char[64];
			SDL_JoystickGetGUIDString (SDL_JoystickGetGUID (joystick), guid, 64);
			return guid;

		}

		return 0;

	}


	const char* Gamepad::GetDeviceName (int id) {

		return SDL_GameControllerName (gameControllers[id]);

	}

	//TODO: сделать чтобы это говно нормально выдавало строки, а не числа
	int Gamepad::GetTypeID(int id)
	{
		return SDL_GameControllerGetType(gameControllers[id]);
	}

	int Gamepad::GetPlayerIndex(int id)
	{
		return SDL_GameControllerGetPlayerIndex(gameControllers[id]);
	}

	void Gamepad::SetPlayerIndex(int id, int index)
	{
		SDL_GameControllerSetPlayerIndex(gameControllers[id], index);
	}

	int Gamepad::GetProductID(int id)
	{
		return SDL_GameControllerGetProduct(gameControllers[id]);
	}

	int Gamepad::GetProductVersion(int id)
	{
		return SDL_GameControllerGetProductVersion(gameControllers[id]);
	}

	int Gamepad::GetVendorID(int id)
	{
		return SDL_GameControllerGetVendor(gameControllers[id]);
	}

	int Gamepad::GetFirmwareVersion(int id)
	{
		return SDL_GameControllerGetFirmwareVersion(gameControllers[id]);
	}

	const char* Gamepad::GetSerialNumber(int id)
	{
		return SDL_GameControllerGetSerial(gameControllers[id]);
	}

	int Gamepad::GetNumTouchpads(int id)
	{
		return SDL_GameControllerGetNumTouchpads(gameControllers[id]);
	}

	int Gamepad::GetMaxTouchpadFingers(int id, int touchpad)
	{
		return SDL_GameControllerGetNumTouchpadFingers(gameControllers[id], touchpad);
	}

	/*someobj Gamepad::GetTouchpadFingerData(int id, int touchpad, int finger, int* state, float* x, float* y, float* pressure)
	{
		return SDL_GameControllerGetTouchpadFinger(gameControllers[id], int touchpad, int finger);
	}*/

	bool Gamepad::HasSensor(int id, int sensor)
	{
		SDL_SensorType realsens;
		switch (sensor)
		{
			case -1:
				realsens = SDL_SENSOR_INVALID;
			case 0:
				realsens = SDL_SENSOR_UNKNOWN;
			case 1:
				realsens = SDL_SENSOR_ACCEL;
			case 2:
				realsens = SDL_SENSOR_GYRO;
		}

		return SDL_GameControllerHasSensor(gameControllers[id], realsens);
	}

	bool Gamepad::EnableSensor(int id, int sensor, bool enable)
	{
		SDL_SensorType realsens;
		switch (sensor)
		{
			case -1:
				realsens = SDL_SENSOR_INVALID;
			case 0:
				realsens = SDL_SENSOR_UNKNOWN;
			case 1:
				realsens = SDL_SENSOR_ACCEL;
			case 2:
				realsens = SDL_SENSOR_GYRO;
		}

		SDL_bool bll;
		if (enable == true)
			bll = SDL_TRUE;
		else
			bll = SDL_FALSE;

		return SDL_GameControllerSetSensorEnabled(gameControllers[id], realsens, bll);
	}

	bool Gamepad::IsSensorEnabled(int id, int sensor)
	{
		SDL_SensorType realsens;
		switch (sensor)
		{
			case -1:
				realsens = SDL_SENSOR_INVALID;
			case 0:
				realsens = SDL_SENSOR_UNKNOWN;
			case 1:
				realsens = SDL_SENSOR_ACCEL;
			case 2:
				realsens = SDL_SENSOR_GYRO;
		}

		return SDL_GameControllerIsSensorEnabled(gameControllers[id], realsens);
	}

	//float Gamepad::GetSensorDataRate(int id, int sensor)

	//float Gamepad::GetSensorData(int id, int type, int num_values)

	bool Gamepad::StartRumble(int id, int lowFreq, int highFreq, int duration)
	{
		return SDL_GameControllerRumble(gameControllers[id], lowFreq, highFreq, duration);
	}

	//bool Gamepad::StartTriggerRumble(int id, int lowFreq, int highFreq, int duration)

	bool Gamepad::HasLED(int id)
	{
		return SDL_GameControllerHasLED(gameControllers[id]);
	}

	bool Gamepad::HasRumble(int id)
	{
		return SDL_GameControllerHasRumble(gameControllers[id]);
	}

	bool Gamepad::HasTriggerRumble(int id)
	{
		return SDL_GameControllerHasRumbleTriggers(gameControllers[id]);
	}

	bool Gamepad::SetLED(int id, int red, int green, int blue)
	{
		return SDL_GameControllerSetLED(gameControllers[id], red, green, blue);
	}

	//bool Gamepad::StartEffect(noclue) //SDL_GameControllerSendEffect
}