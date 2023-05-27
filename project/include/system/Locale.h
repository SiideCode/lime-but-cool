#ifndef LIME_SYSTEM_LOCALE_H
#define LIME_SYSTEM_LOCALE_H

#include "SDL_locale.h"

namespace lime {
	class Locale {
		public:
			static SDL_Locale *GetSystemLocale ();
	};
}

#endif