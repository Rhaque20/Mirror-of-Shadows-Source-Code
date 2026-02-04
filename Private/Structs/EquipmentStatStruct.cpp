#include "Structs/EquipmentStatStruct.h"


uint32 GetTypeHash(const FEquipmentStat& Thing)
{
	return GetTypeHash(Thing.StatTag);
}