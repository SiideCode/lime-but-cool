#ifdef HX_WINRT
#include "DesktopNotificationManagerCompat.h"
#include <NotificationActivationCallback.h>
#include <windows.ui.notifications.h>
using namespace ABI::Windows::Data::Xml::Dom;
using namespace ABI::Windows::UI::Notifications;
using namespace Microsoft::WRL;
using namespace DesktopNotificationManagerCompat;

#ifndef NOTIF_H_CPP_STD
#define NOTIF_H_CPP_STD
using namespace std;
#endif
#endif

#if defined(HX_WINDOWS) && !defined(HX_WINRT)
#include "Windows.h"
#include "shellapi.h"
//#include "SDL_syswm.h"
#include "psapi.h"
#include "shobjidl_core.h"
#include "combaseapi.h"
#include "propvarutil.h"
//#include "SDL.h"
#include "propkey.h"

#ifndef NOTIF_H_CPP_STD
#define NOTIF_H_CPP_STD
using namespace std;
#endif
#endif

#ifdef HXCPP_DEBUG
//Not defining it twice i guess...
#ifndef NOTIF_H_CPP_STD
#define NOTIF_H_CPP_STD
using namespace std;
#endif
#include "iostream"
#endif


namespace lime
{
	class Notifications
	{
		public:
			bool initialisedToast;

			virtual bool initWinRTToast(const char* AUMID);
			virtual bool createWinRTToast();
			//virtual bool sendWinRTToast();

			virtual bool createWin32ToastShortcut(const char* projectName);
			virtual bool sendWin32Toast();

			virtual int showAndroidToast(const char* message, int duration, int gravity, int offsetx, int offsety);
		private:
			#if HX_WINDOWS
			virtual bool setupShortcut(_In_z_ wchar_t* shortcutPath/*, SDL_Window* sdlWindowInstance*/);
			#endif
	};
};

//#ifdef ANDROID
/*namespace AndroidGravity
{
	//Mostly for comparing values or smthn
	enum Gravity
	{
		//GRAVITY_AXIS_CLIP = 0x00000008,
		//GRAVITY_AXIS_PULL_AFTER = 0x00000004,
		//GRAVITY_AXIS_PULL_BEFORE = 0x00000002,
		//GRAVITY_AXIS_SPECIFIED = 0x00000001,
		//GRAVITY_AXIS_X_SHIFT = 0x00000000,
		//GRAVITY_AXIS_Y_SHIFT = 0x00000004,
		GRAVITY_TOP = 0x00000030,
		GRAVITY_BOTTOM = 0x00000050,
		GRAVITY_LEFT = 0x00000003,
		GRAVITY_RIGHT = 0x00000005,
		GRAVITY_CENTER = 0x00000011,
		GRAVITY_CENTER_HORIZONTAL = 0x00000001,
		GRAVITY_CENTER_VERTICAL = 0x00000010,
		//GRAVITY_CLIP_HORISONTAL = 0x00000008,
		//GRAVITY_CLIP_VERTICAL = 0x00000080,
		//GRAVITY_DISPLAY_CLIP_HORIZONTAL = 0x01000000,
		//GRAVITY_DISPLAY_CLIP_VERTICAL = 0x10000000,
		GRAVITY_START = 0x00800003,
		GRAVITY_END = 0x00800005,
		//GRAVITY_FILL = 0x00000077,
		//GRAVITY_FILL_HORIZONTAL = 0x00000007,
		//GRAVITY_FILL_VERTICAL = 0x00000070,
		//GRAVITY_HORIZONTAL_GRAVITY_MASK = 0x00000007,
		GRAVITY_NO_GRAVITY = 0x00000000,
		//GRAVITY_RELATIVE_HORIZONTAL_GRAVITY_MASK = 0x00800007,
		//GRAVITY_RELATIVE_LAYOUT_DIRECTION = 0x00800000,
		//GRAVITY_VERTICAL_GRAVITY_MASK = 0x00000070
		};
};*/
//#endif