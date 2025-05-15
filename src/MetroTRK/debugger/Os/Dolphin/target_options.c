#include <MetroTRK.h>

static u8 bUseSerialIO;

void SetUseSerialIO(u8 useSerial) {
    bUseSerialIO = useSerial;
    return;
}

u8 GetUseSerialIO(void) {
    return bUseSerialIO;
}
