#include <system/Locale.h>

namespace lime
{
	SDL_Locale *Locale::GetSystemLocale()
	{
		return SDL_GetPreferredLocales();
	}
}