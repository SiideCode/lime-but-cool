#if HX_WINDOWS | HX_WINRT

#if HX_WINDOWS
#include <d2d1.h>
#include <d2d1helper.h>
#endif

#include <VersionHelpers.h>

#if HX_WINRT
#include <d2d1_1.h>
#include <d2d1_1helper.h>
#include <d2d1effects.h>
#include <d2d1effecthelpers.h>
#endif

bool lime_d2d_create_factory(bool isMultithreaded, int debugLevel)
{
	D2D1_FACTORY_TYPE factorytype = isMultithreaded ? D2D1_FACTORY_TYPE_MULTI_THREADED : D2D1_FACTORY_TYPE_SINGLE_THREADED;
	D2D1_FACTORY_OPTIONS factopt;
	D2D1_DEBUG_LEVEL debuglvl;
	void** factory;

	//why not lol
	if (debugLevel == false)
		debuglvl = D2D1_DEBUG_LEVEL_NONE;
	else if (debugLevel == 1)
		debuglvl = D2D1_DEBUG_LEVEL_ERROR;
	else if (debugLevel == 2)
		debuglvl = D2D1_DEBUG_LEVEL_WARNING;
	else if (debugLevel == 3)
		debuglvl = D2D1_DEBUG_LEVEL_INFORMATION;

	factopt.debugLevel = debuglvl;

	D2D1CreateFactory(factorytype, factopt, factory);
}

#endif