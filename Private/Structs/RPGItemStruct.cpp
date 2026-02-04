#include "Structs/RPGItemStruct.h"

uint32 GetTypeHash(const FRPGItemData& Thing)
{
	FGameplayTag TagTest;
	return FCrc::MemCrc32(&Thing, sizeof(FRPGItemData));
}
