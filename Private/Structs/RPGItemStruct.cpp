#include "Structs/RPGItemStruct.h"

uint32 GetTypeHash(const FRPGItemData& Thing)
{
	return FCrc::MemCrc32(&Thing, sizeof(FRPGItemData));
}
