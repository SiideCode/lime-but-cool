#include "system/notify/Notifications.h"

#ifdef ANDROID
#include "SDL_system.h"
#endif

//using namespace AndroidGravity;

namespace lime
{
	int Notifications::showAndroidToast(const char* message, int duration, int gravity, int offsetx, int offsety)
	{
		#ifdef ANDROID
		if (duration == 0) duration = 10;
		return SDL_AndroidShowToast(message, duration, gravity, offsetx, offsety);
		#else
		return 0;
		#endif
	}

	bool Notifications::createWin32ToastShortcut(const char* projectName/*, SDL_Window* sdlwindowreal*/)
	{
		#ifdef HX_WINDOWS
		//TODO: setup a 216 symbol limit for projectName
		wchar_t* helpme = L"\\Microsoft\\Windows\\Start Menu\\Programs\\";

		wchar_t* prjName = new wchar_t[strlen(projectName) + 1];

		// Convert char* to wchar_t*
		size_t convertedChars = 0;
		mbstowcs_s(&convertedChars, prjName, strlen(projectName) + 1, projectName, _TRUNCATE);

		wcsncat_s(helpme, wcslen(helpme),prjName, wcslen(prjName));

		#ifdef HXCPP_DEBUG
		cout << helpme;
		#endif

		wchar_t shortcutPath[MAX_PATH];
		DWORD charWritten = GetEnvironmentVariableW(L"APPDATA", shortcutPath, MAX_PATH);

		if (charWritten > 0)
		{
			if (SUCCEEDED(wcscat_s(shortcutPath, ARRAYSIZE(shortcutPath), helpme)))
			{
				delete[] prjName;
				DWORD attributes = GetFileAttributesW(shortcutPath);
				bool fileExists = attributes < 0xFFFFFFF;

				if (!fileExists)
				{
					setupShortcut(shortcutPath/*, sdlwindowreal*/);
					return true;
				}
				else
				{
					return false;
				}
			}
		}
		delete[] prjName;
		return false;
		#else
		return false;
		#endif
	}

	bool Notifications::sendWin32Toast()
	{
		return false;
	}

	#ifdef HX_WINDOWS
	bool Notifications::setupShortcut(_In_z_ wchar_t* shortcutPath/*, SDL_Window* sdlWindowInstance*/)
	{
		//SDL_SysWMinfo inflol;

		wchar_t exePath[MAX_PATH];

		DWORD charWritten = K32GetModuleFileNameExW(GetCurrentProcess(), nullptr, exePath, ARRAYSIZE(exePath));

		if (charWritten > 0)
		{
			CoInitializeEx(NULL, 0);
			IShellLinkW* shellLnk;
			if (SUCCEEDED(CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLinkW, reinterpret_cast<LPVOID*>(&shellLnk))))
			{
				/*cout <<*/ shellLnk->AddRef();
				if (SUCCEEDED(shellLnk->SetPath(exePath)))
				{
					if (SUCCEEDED(shellLnk->SetArguments(L"")))
					{
						IPropertyStore* propStore;
						if (SUCCEEDED(shellLnk->QueryInterface(IID_IPropertyStore, (void**) &propStore)))
						{
							/*cout <<*/ propStore->AddRef();
							//if(SDL_GetWindowWMInfo(sdlWindowInstance, &inflol))
							//{
								//IPropertyStore* wndPS;
								//SHGetPropertyStoreForWindow(inflol.info.win.window, IID_IPropertyStore, (void**) &wndPS);
								PROPVARIANT appIdPropVar;
								if (SUCCEEDED(InitPropVariantFromString(L"525f2256-39ce-4ad3-9003-d2d57e31d9fe", &appIdPropVar)))
								{
									if (SUCCEEDED(propStore->SetValue(PKEY_AppUserModel_ID, appIdPropVar)))
									{
										if (SUCCEEDED(propStore->Commit()))
										{
											/*cout <<*/ propStore->Release();
											IPersistFile* persFile;
											if (SUCCEEDED(shellLnk->QueryInterface(IID_IPersistFile, (void**) &persFile)))
											{
												/*cout <<*/ persFile->AddRef();
												bool mama = SUCCEEDED(persFile->Save(shortcutPath, TRUE));
												/*cout <<*/ persFile->Release();
												/*cout <<*/ shellLnk->Release();
												PropVariantClear(&appIdPropVar);
												return mama;
											}
										}
									}
								}
								PropVariantClear(&appIdPropVar);
							//}
						}
					}
				}
			}
		}
		return false;
	}
	#endif
}

namespace lime
{
	#ifdef HX_WINRT
	bool initialisedToast;
	#endif

	bool Notifications::initWinRTToast(const char* AUMID)
	{
		#ifdef HX_WINRT
		size_t realsiz = strlen(AUMID) + 1;

		wchar_t* wideAUMID = new wchar_t[realsiz];

		// Convert char* to wchar_t*
		size_t convertedChars = 0;
		mbstowcs_s(&convertedChars, wideAUMID, realsiz, AUMID, _TRUNCATE);

		if (SUCCEEDED(RegisterAumidAndComServer(wideAUMID, __uuidof(NotificationActivator))) && !initialisedToast)
		{
			if (SUCCEEDED(RegisterActivator()))
			{
				initialisedToast = true;
				delete[] wideAUMID;
				return true;
			}
			else
				return false;
		}
		else
			return false;
		#else
		return false;
		#endif
	}

	bool Notifications::createWinRTToast()
	{
		#ifdef HX_WINRT
		#endif
		return false;
	}
}

#ifdef HX_WINRT
//TODO: Сделать UUID генератор
class DECLSPEC_UUID("525f2256-39ce-4ad3-9003-d2d57e31d9fe") NotificationActivator WrlSealed WrlFinal
    : public RuntimeClass<RuntimeClassFlags<ClassicCom>, INotificationActivationCallback>
{
public:
    virtual HRESULT STDMETHODCALLTYPE Activate(
        _In_ LPCWSTR appUserModelId,
        _In_ LPCWSTR invokedArgs,
        _In_reads_(dataCount) const NOTIFICATION_USER_INPUT_DATA* data,
        ULONG dataCount) override
    {
        // TODO: Handle activation
		wstring arguments(invokedArgs);
    }
};

CoCreatableClass(NotificationActivator);
#endif