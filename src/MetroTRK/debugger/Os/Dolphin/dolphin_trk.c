#include <MetroTRK.h>


typedef unsigned int uint;

#define TARGET_DB_EXCEPTION_OFFSET_LOW 0x44U
#define TARGET_DB_EXCEPTION_OFFSET_HIGH 0x80000000U
#define TARGET_MEM_END_ROM 0x80000000U
#define TARGET_MEM_START_RAM 0x7E000000U

#define PHYSICAL_MEM_MASK 0x3FFFFFFFU
#define CACHED_MEM_BASE 0x80000000U

static u8 TRK_ISR_OFFSETS[64] = {
    0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03,
    0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00,
    0x06, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00,
    0x00, 0x09, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x0d, 0x00,
    0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00, 0x14,
    0x00, 0x00, 0x00, 0x17, 0x00, 0x00, 0x00, 0x00, 0x00};

typedef struct TRKState {
    u32 GPR[32];                  // 0x00
    u32 LR;                       // 0x80
    u32 CTR;                      // 0x84
    u32 XER;                      // 0x88
    u32 MSR;                      // 0x8C
    u32 DAR;                      // 0x90
    u32 DSISR;                    // 0x94
    BOOL stopped;                 // 0x98
    BOOL inputActivated;          // 0x9C
    volatile u8* inputPendingPtr; // 0xA0
} TRKState;

typedef struct {
    u8 _padding[0x238];
    uint field_at_0x238;
} TRKCPUStateType;

typedef struct LC_BASE {
    uint x00;
    uint x04;
} LC_BASE;

extern TRKCPUStateType gTRKCPUState;
extern TRKState gTRKState;
extern u8 gTRKInterruptVectorTable[];

LC_BASE lc_base;

DECL_SECTION(".init") void __TRK_reset(void) {
    OSResetSystem(0, 0, 0);
    return;
}

void EnableMetroTRKInterrupts(void) {
    EnableEXI2Interrupts();
    return;
}

uint TRKTargetTranslate(uint param_1) {
    uint lc_base_val;

    lc_base_val = lc_base.x00;

    if (param_1 >= lc_base_val) {
        if (param_1 < (lc_base_val + 0x4000)) {

            if ((gTRKCPUState.field_at_0x238 & 3) != 0) {
                return param_1;
            }
        }
    }

    if (param_1 >= 0x7E000000U && param_1 <= 0x80000000U) {
        return param_1;
    }

    return (param_1 & 0x3FFFFFFF) | 0x80000000;
}

void __TRK_copy_vectors(void) {
    u32 translatedDestAddress;

    u32 bitmaskValue;
    s32 loopCounter;
    u32* isrOffsetPtr;

    u32 bitmaskLoadAddress;
    u32 gprValue;
    u32 arithmeticTemp;
    u32 currentIsrOffset;
    u32 bitCheckTemp;

    if (lc_base.x00 > TARGET_DB_EXCEPTION_OFFSET_LOW) {
        goto setBitmaskLoadAddressHigh;
    }
    arithmeticTemp = lc_base.x00 + 0x4000U;
    if (arithmeticTemp <= TARGET_DB_EXCEPTION_OFFSET_LOW) {
        goto setBitmaskLoadAddressHigh;
    }

    gprValue = gTRKCPUState.field_at_0x238;
    gprValue &= 3;
    if (gprValue == 0) {
        goto setBitmaskLoadAddressHigh;
    }

    bitmaskLoadAddress = TARGET_DB_EXCEPTION_OFFSET_LOW;
    goto bitmaskLoadAddressSet;

setBitmaskLoadAddressHigh:
    bitmaskLoadAddress =
        TARGET_DB_EXCEPTION_OFFSET_HIGH + TARGET_DB_EXCEPTION_OFFSET_LOW;

bitmaskLoadAddressSet:
    bitmaskValue = *(u32*)bitmaskLoadAddress;
    isrOffsetPtr = (u32*)TRK_ISR_OFFSETS;
    loopCounter = 0;

loopStart:
    arithmeticTemp = 1U << loopCounter;
    bitCheckTemp = bitmaskValue & arithmeticTemp;

    if (bitCheckTemp == 0) {
        goto loopIncrement;
    }
    if (loopCounter == 4) {
        goto loopIncrement;
    }

    currentIsrOffset = *isrOffsetPtr;

    if (currentIsrOffset >= lc_base.x00 &&
        currentIsrOffset < (lc_base.x00 + 0x4000U) &&
        (gTRKCPUState.field_at_0x238 & 3) != 0) {
        translatedDestAddress = currentIsrOffset;
    } else if (currentIsrOffset >= TARGET_MEM_START_RAM &&
               currentIsrOffset <= TARGET_MEM_END_ROM) {
        translatedDestAddress = currentIsrOffset;
    } else {
        translatedDestAddress =
            (currentIsrOffset & PHYSICAL_MEM_MASK) | CACHED_MEM_BASE;
    }

    TRK_memcpy((void*)translatedDestAddress,
               (void*)((u8*)gTRKInterruptVectorTable + currentIsrOffset),
               0x100U);
    TRK_flush_cache((void*)translatedDestAddress, 0x100U);

loopIncrement:
    loopCounter++;
    isrOffsetPtr++;

    if (loopCounter <= 14) {
        goto loopStart;
    }

    return;
}

uint TRKInitializeTarget(void) {
    gTRKState.stopped = 1;
    gTRKState.MSR = __TRK_get_MSR();
    lc_base.x00 = 0xE0000000;
    return 0;
}

void gap_03_800C5A6C_text(void) {
    return;
}
