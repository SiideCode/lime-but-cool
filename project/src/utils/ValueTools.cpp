#include "ValueTools.h"

namespace lime
{
	namespace valueConvert
	{
		Uint32 nekoValToUint32(value source)
		{
			Uint32 ge;
			memcpy_s(&ge, sizeof(Uint32), (Uint32*) val_data(source), sizeof(Uint32));
			return ge;
		}

		Uint16 nekoValToUint16(value source)
		{
			Uint16 ge;
			memcpy_s(&ge, sizeof(Uint16), (Uint16*) val_data(source), sizeof(Uint16));
			return ge;
		}

		Uint8 nekoValToUint8(value source)
		{
			Uint8 ge;
			memcpy_s(&ge, sizeof(Uint8), (Uint8*) val_data(source), sizeof(Uint8));
			return ge;
		}
	}
}