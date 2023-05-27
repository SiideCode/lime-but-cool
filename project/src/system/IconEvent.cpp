#ifdef HX_WINDOWS
#include <system/IconEvent.h>
#include <system/CFFI.h>

namespace lime {
	ValuePointer* IconEvent::callback = 0;
	ValuePointer* IconEvent::eventObject = 0;

	static int id_xClick;
	static int id_yClick;
	static int id_type;
	static bool init = false;

	IconEvent::IconEvent () {
		xClick = 0;
		yClick = 0;
		type = CONTEXTMENUOPEN;
	}

	void IconEvent::Dispatch (IconEvent* event) {
		if (IconEvent::callback) {
			if (IconEvent::eventObject->IsCFFIValue ()) {
				if (!init) {
					id_xClick = val_id ("x");
					id_yClick = val_id ("y");
					id_type = val_id ("type");
					init = true;
				}

				value object = (value)IconEvent::eventObject->Get ();

				alloc_field (object, id_xClick, alloc_float (event->xClick));
				alloc_field (object, id_yClick, alloc_float (event->yClick));
				alloc_field (object, id_type, alloc_int (event->type));
			} else {
				IconEvent* eventObject = (IconEvent*)IconEvent::eventObject->Get ();

				eventObject->xClick = event->xClick;
				eventObject->yClick = event->yClick;
				eventObject->type = event->type;
			}
			IconEvent::callback->Call ();
		}
	}
}
#endif