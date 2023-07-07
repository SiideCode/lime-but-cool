#include <SDL.h>
#include <system/CFFI.h>

DEFINE_KIND(k_in32);
DEFINE_KIND(k_in16);
DEFINE_KIND(k_in8);

namespace lime
{
	namespace valueConvert
	{
		Uint32 nekoValToUint32(value source);

		Uint16 nekoValToUint16(value source);

		Uint8 nekoValToUint8(value source);
	}
}