#include <MetroTRK.h>

int TRKDoNotifyStopped(int notificationType) {
    int result;
    int bufferId;
    struct TRKMessageBuffer* bufferHandle;
    u32 responseBuffers[2];

    result = TRKGetFreeBuffer(&bufferId, &bufferHandle);
    if (result == 0) {
        if (result == 0) {
            if (notificationType == 0x90) {
                TRKTargetAddStopInfo(bufferHandle);
            } else {
                TRKTargetAddExceptionInfo(bufferHandle);
            }
        }
        result = TRKRequestSend(bufferHandle, responseBuffers, 2, 3, 1);
        if (result == 0) {
            TRKReleaseBuffer(responseBuffers[0]);
        }
        TRKReleaseBuffer(bufferId);
    }
    return result;
}
