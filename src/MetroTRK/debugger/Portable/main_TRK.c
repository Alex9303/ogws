#include <MetroTRK.h>

static int gap_09_8040DC24_bss;
static s32 TRK_mainError;

int TRK_main(void) {
    TRK_mainError = TRKInitializeNub();

    if (!TRK_mainError) {
        TRKNubWelcome();
        TRKNubMainLoop();
    }

    TRK_mainError = TRKTerminateNub();

    return TRK_mainError;
}
