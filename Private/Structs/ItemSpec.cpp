#include "Structs/ItemSpec.h"

uint32 GetTypeHash(const FItemSpec& Thing)
{
    return FCrc::MemCrc32(&Thing, sizeof(FItemSpec));
}