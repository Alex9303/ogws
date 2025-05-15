#include <MetroTRK.h>

BOOL usr_puts_serial(const char* msg) {
    BOOL exit_condition = FALSE;
    char current_char_val;
    char osreport_buf[2];
    u32 original_trk_state_val;

    while (!exit_condition && (current_char_val = *msg++) != '\0') {
        original_trk_state_val = GetTRKConnected();

        osreport_buf[0] = current_char_val;
        osreport_buf[1] = '\0';

        SetTRKConnected(FALSE);
        OSReport(osreport_buf);
        SetTRKConnected(original_trk_state_val);
        exit_condition = FALSE;
    }

    return exit_condition;
}


void usr_put_initialize(void) {
    return;
}
