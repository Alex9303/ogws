#ifndef RVL_SDK_KPAD_H
#define RVL_SDK_KPAD_H
#include <types.h>

#include <revolution/MTX.h>
#include <revolution/WPAD.h>
#ifdef __cplusplus
extern "C" {
#endif

#define KPAD_MAX_SAMPLES 16

#define KPAD_BUTTON_RPT 0x80000000

#define KPAD_BTN_RPT_TIME_MAX 20000
#define KPAD_BTN_NO_RPT_DELAY 40000

#define KPAD_DPD_IMG_RESO_WX 1024 // WPAD_DPD_IMG_RESO_WX
#define KPAD_DPD_IMG_RESO_WY 768 // WPAD_DPD_IMG_RESO_WY

#define KPAD_USE_OBJECTS 2

typedef union KPADEXStatus {
    struct {
        // total size: 0x1C
        Vec2 stick;     // offset 0x0, size 0x8
        Vec acc;        // offset 0x8, size 0xC
        f32 acc_value;  // offset 0x14, size 0x4
        f32 acc_speed;  // offset 0x18, size 0x4
    } fs;               // offset 0x0, size 0x1C

    struct {
        // total size: 0x24
        u32 hold;      // offset 0x0, size 0x4
        u32 trig;      // offset 0x4, size 0x4
        u32 release;   // offset 0x8, size 0x4
        Vec2 lstick;   // offset 0xC, size 0x8
        Vec2 rstick;   // offset 0x14, size 0x8
        f32 ltrigger;  // offset 0x1C, size 0x4
        f32 rtrigger;  // offset 0x20, size 0x4
    } cl;              // offset 0x0, size 0x24
} KPADEXStatus;

typedef struct KPADStatus {
    // total size: 0x84
    u32 hold;                // offset 0x0, size 0x4
    u32 trig;                // offset 0x4, size 0x4
    u32 release;             // offset 0x8, size 0x4
    Vec acc;                 // offset 0xC, size 0xC
    f32 acc_value;           // offset 0x18, size 0x4
    f32 acc_speed;           // offset 0x1C, size 0x4
    Vec2 pos;                // offset 0x20, size 0x8
    Vec2 vec;                // offset 0x28, size 0x8
    f32 speed;               // offset 0x30, size 0x4
    Vec2 horizon;            // offset 0x34, size 0x8
    Vec2 hori_vec;           // offset 0x3C, size 0x8
    f32 hori_speed;          // offset 0x44, size 0x4
    f32 dist;                // offset 0x48, size 0x4
    f32 dist_vec;            // offset 0x4C, size 0x4
    f32 dist_speed;          // offset 0x50, size 0x4
    Vec2 acc_vertical;       // offset 0x54, size 0x8
    u8 dev_type;             // offset 0x5C, size 0x1
    s8 wpad_err;             // offset 0x5D, size 0x1
    s8 dpd_valid_fg;         // offset 0x5E, size 0x1
    u8 data_format;          // offset 0x5F, size 0x1
    KPADEXStatus ex_status;  // offset 0x60, size 0x24
} KPADStatus;

typedef struct KPADUnifiedWpadStatus {
    // total size: 0x38
    union {
        WPADStatus core;  // size 0x2A
        WPADFSStatus fs;  // size 0x36
        WPADCLStatus cl;  // size 0x36
    } u;                  // offset 0x0, size 0x36
    u8 fmt;               // offset 0x36, size 0x1
    u8 padding;           // offset 0x37, size 0x1
} KPADUnifiedWpadStatus;

typedef struct KPADObject {
    // total size: 0xC
    Vec2 center;  // offset 0x0, size 0x8
    s8 error_fg;  // offset 0x8, size 0x1
    s8 state_fg;  // offset 0x9, size 0x1
    s8 _fg_1;     // offset 0xA, size 0x1
    s8 _fg_2;     // offset 0xB, size 0x1
} KPADObject;

typedef enum KPADPlayMode {
    KPAD_PLAY_MODE_LOOSE,
    KPAD_PLAY_MODE_TIGHT
} KPADPlayMode;

typedef union {
    // total size: 0x84
    struct KPADStatus k;             // offset 0x0, size 0x84
    struct KPADUnifiedWpadStatus w;  // offset 0x0, size 0x38
} KPADTmpStatus;

struct KPADInsideStatus {
    // total size: 0x524
    struct KPADStatus status;                     // offset 0x0, size 0x84
    f32 pos_play_radius;                          // offset 0x84, size 0x4
    f32 pos_sensitivity;                          // offset 0x88, size 0x4
    f32 hori_play_radius;                         // offset 0x8C, size 0x4
    f32 hori_sensitivity;                         // offset 0x90, size 0x4
    f32 dist_play_radius;                         // offset 0x94, size 0x4
    f32 dist_sensitivity;                         // offset 0x98, size 0x4
    f32 acc_play_radius;                          // offset 0x9C, size 0x4
    f32 acc_sensitivity;                          // offset 0xA0, size 0x4
    f32 dist_org;                                 // offset 0xA4, size 0x4
    Vec2 accXY_nrm_hori;                          // offset 0xA8, size 0x8
    Vec2 sec_nrm_hori;                            // offset 0xB0, size 0x8
    Vec2 center_org;                              // offset 0xB8, size 0x8
    f32 dpd2pos_scale;                            // offset 0xC0, size 0x4
    struct KPADObject kobj_sample[4];             // offset 0xC4, size 0x30
    struct KPADObject kobj_regular[2];            // offset 0xF4, size 0x18
    s16 valid_objs;                               // offset 0x10C, size 0x2
    u8 bufIdx;                                    // offset 0x10E, size 0x1
    u8 bufCount;                                  // offset 0x10F, size 0x1
    struct KPADUnifiedWpadStatus uniRingBuf[12];  // offset 0x110, size 0x2A0
    u8 pad_0x3B0[0xD8];                           // offset 0x3B0, size 0xD8
    struct KPADUnifiedWpadStatus* uniRingBufEx;   // offset 0x488, size 0x4
    u32 uniRingBufExLen;                          // offset 0x48C, size 0x4
    f32 sec_length;                               // offset 0x490, size 0x4
    Vec2 sec_nrm;                                 // offset 0x494, size 0x8
    f32 sec_dist;                                 // offset 0x49C, size 0x4
    f32 trust_sec_length;                         // offset 0x4A0, size 0x4
    Vec hard_acc;                                 // offset 0x4A4, size 0xC
    Vec2 obj_horizon;                             // offset 0x4B0, size 0x8
    Vec2 acc_horizon;                             // offset 0x4B8, size 0x8
    Vec2 ah_circle_pos;                           // offset 0x4C0, size 0x8
    u16 ah_circle_ct;                             // offset 0x4C8, size 0x2
    u8 dpd_valid2_ct;                             // offset 0x4CA, size 0x1
    u8 readLocked;                                // offset 0x4CB, size 0x1
    u16 btn_repeat_time;                          // offset 0x4CC, size 0x2
    u16 btn_repeat_next;                          // offset 0x4CE, size 0x2
    u16 btn_repeat_delay;                         // offset 0x4D0, size 0x2
    u16 btn_repeat_pulse;                         // offset 0x4D2, size 0x2
    u16 btn_cl_repeat_time;                       // offset 0x4D4, size 0x2
    u16 btn_cl_repeat_next;                       // offset 0x4D6, size 0x2
    void (*dpd_ctrl_callback)(s32, s32);          // offset 0x4D8, size 0x4
    f32 acc_scale_x;                              // offset 0x4DC, size 0x4
    f32 acc_scale_y;                              // offset 0x4E0, size 0x4
    f32 acc_scale_z;                              // offset 0x4E4, size 0x4
    f32 fs_acc_scale_x;                           // offset 0x4E8, size 0x4
    f32 fs_acc_scale_y;                           // offset 0x4EC, size 0x4
    f32 fs_acc_scale_z;                           // offset 0x4F0, size 0x4
    Vec2 kobj_frame_min;                          // offset 0x4F4, size 0x8
    Vec2 kobj_frame_max;                          // offset 0x4FC, size 0x8
    f32 err_dist_speed_1;                         // offset 0x504, size 0x4
    f32 err_dist_speedM_1;                        // offset 0x508, size 0x4
    f32 ah_circle_radius2;                        // offset 0x50C, size 0x4
    f32 dist_vv1;                                 // offset 0x510, size 0x4
    f32 err_dist_min;                             // offset 0x514, size 0x4
    void (*sampling_callback)(s32);               // offset 0x518, size 0x4
    u8 UNK_0x51C;                                 // offset 0x51C, size 0x1
    u8 resetReq;                                  // offset 0x51D, size 0x1
    u8 exResetReq;                                // offset 0x51E, size 0x1
    u8 fsAccRevise;                               // offset 0x51F, size 0x1
    u8 pos_play_mode;                             // offset 0x520, size 0x1
    u8 hori_play_mode;                            // offset 0x521, size 0x1
    u8 dist_play_mode;                            // offset 0x522, size 0x1
    u8 acc_play_mode;                             // offset 0x523, size 0x1
};

void KPADSetBtnRepeat(s32 chan, f32 delay_sec, f32 pulse_sec);
void KPADSetPosParam(s32 chan, f32 play_radius, f32 sensitivity);
void KPADSetHoriParam(s32 chan, f32 play_radius, f32 sensitivity);
void KPADSetDistParam(s32 chan, f32 play_radius, f32 sensitivity);
void KPADSetAccParam(s32 chan, f32 play_radius, f32 sensitivity);
void reset_kpad(struct KPADInsideStatus* kp);
void KPADSetSensorHeight(s32 chan, f32 level);
void calc_button_repeat(struct KPADInsideStatus* kp, u32 dev_type, u32 count);
static void calc_acc_horizon(struct KPADInsideStatus* kp);
static void calc_acc_vertical(struct KPADInsideStatus* kp);
void read_kpad_acc(struct KPADInsideStatus* kp, struct KPADUnifiedWpadStatus* uwp);
s8 select_2obj_first(struct KPADInsideStatus* kp);
static s8 select_2obj_continue(struct KPADInsideStatus* kp);
static s8 select_1obj_first(struct KPADInsideStatus* kp);
s8 select_1obj_continue(struct KPADInsideStatus* kp);
static void calc_dpd_variable(struct KPADInsideStatus* kp, s8 valid_fg_next);
void read_kpad_dpd(struct KPADInsideStatus* kp, struct KPADUnifiedWpadStatus* uwp);
static void clamp_stick_circle(Vec2* stick, s32 sx, s32 sy, s32 min, s32 max);
static void clamp_stick_cross(Vec2* stick, s32 sx, s32 sy, s32 min, s32 max);
void read_kpad_stick(struct KPADInsideStatus* kp, struct KPADUnifiedWpadStatus* uwp);
s32 KPADRead(s32 chan, struct KPADStatus samplingBufs[], u32 length);
void KPADInit(void);
void KPADDisableDPD(s32 chan);
void KPADEnableDPD(s32 chan);
void KPADSetControlDpdCallback(s32 chan, WPADCallback cb);
static void KPADiSamplingCallback(s32 chan);
void KPADiControlDpdCallback(s32 chan, s32 result);

#ifdef __cplusplus
}
#endif
#endif
