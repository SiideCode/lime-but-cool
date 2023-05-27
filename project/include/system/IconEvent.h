#ifdef HX_WINDOWS
#ifndef LIME_APP_SYSTEM_EVENT_H
#define LIME_APP_SYTEM_EVENT_H

#include <system/CFFI.h>
#include <system/ValuePointer.h>

namespace lime
{
	enum IconEventType
	{
		CONTEXTMENUOPEN,
		ICONCLICK
	};

	struct IconEvent
	{
		hl_type* t;
		float xClick;
		float yClick;
		IconEventType type;

		static ValuePointer* callback;
		static ValuePointer* eventObject;

		IconEvent ();

		static void Dispatch (IconEvent* event);
	};
}
#endif


#endif