#include QMK_KEYBOARD_H

#define XXX KC_NO

#define LAYOUT( \
    L00, L01, L02, L03, L04,                     R00, R01, R02, R03, R04, \
    L10, L11, L12, L13, L14,                     R10, R11, R12, R13, R14, \
    L20, L21, L22, L23, L24,                     R20, R21, R22, R23, R24, \
         L30, L31,                                         R30, R31, \
                   L40, L41, L42, L43,      R40,           R41, \
                             L50, L51,           R50, R51 \
) { \
    { L04, L03, L02, L01, L00 }, \
    { L14, L13, L12, L11, L10 }, \
    { L24, L23, L22, L21, L20 }, \
    { L41, L40, L31, L30, XXX }, \
    { L42, L50, L51, L43, XXX }, \
\
    { R00, R01, R02, R03, R04 }, \
    { R10, R11, R12, R13, R14 }, \
    { R20, R21, R22, R23, R24 }, \
    { XXX, XXX, R30, R31, XXX }, \
    { XXX, R41, R51, R50, R40 } \
}

enum layers {
    BAS,
    SYM,
    MOU,
    UTL
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BAS] = LAYOUT(
        KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,         KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,
        KC_A,    KC_S,    KC_D,    KC_F,    KC_G,         KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN,
        KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,         KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,
                 KC_LBRC, KC_MINS,                                          KC_QUOT, KC_EQL,
                 KC_LCMD, KC_SPC,  KC_LSFT, KC_LOPT,      KC_NO,                     KC_BSPC,
                                   TO(SYM), KC_ESC,                KC_TAB,  KC_ENT
    ),
    [SYM] = LAYOUT(
        TO(BAS), TO(BAS), TO(BAS), TO(BAS), TO(BAS),      TO(BAS), KC_7,    KC_8,    KC_9,    TO(BAS),
        TO(BAS), TO(BAS), TO(BAS), TO(BAS), TO(BAS),      KC_GRV,  KC_4,    KC_5,    KC_6,    TO(BAS),
        TO(BAS), TO(BAS), TO(BAS), TO(BAS), TO(BAS),      TO(BAS), KC_1,    KC_2,    KC_3,    TO(BAS),
                 TO(BAS), TO(BAS),                                          KC_0,    TO(BAS),
                 TO(BAS), TO(BAS), TO(BAS), TO(BAS),      TO(BAS),                   TO(BAS),
                                   TO(UTL), TO(BAS),               TO(BAS), TO(BAS)
    ),
    [MOU] = LAYOUT(
        TO(BAS), TO(BAS), TO(BAS), TO(BAS), TO(BAS),      TO(BAS), TO(BAS), TO(BAS), TO(BAS), TO(BAS),
        TO(BAS), TO(BAS), KC_BTN2, KC_BTN1, TO(BAS),      TO(BAS), KC_BTN1, KC_BTN2, TO(BAS), TO(BAS),
        TO(BAS), TO(BAS), TO(BAS), TO(BAS), TO(BAS),      TO(BAS), TO(BAS), TO(BAS), TO(BAS), TO(BAS),
                 TO(BAS), TO(BAS),                                          TO(BAS), TO(BAS),
                 TO(BAS), TO(BAS), TO(BAS), TO(BAS),      TO(BAS),                  TO(BAS),
                                   TO(BAS), TO(BAS),               TO(BAS), TO(BAS)
    ),
    [UTL] = LAYOUT(
        TO(BAS), TO(BAS), TO(BAS), TO(BAS), TO(BAS),      TO(BAS), TO(BAS), TO(BAS), TO(BAS), TO(BAS),
        TO(BAS), TO(BAS), TO(BAS), TO(BAS), TO(BAS),      TO(BAS), TO(BAS), TO(BAS), TO(BAS), TO(BAS),
        TO(BAS), TO(BAS), TO(BAS), TO(BAS), TO(BAS),      TO(BAS), TO(BAS), TO(BAS), TO(BAS), TO(BAS),
                 TO(BAS), TO(BAS),                                          TO(BAS), TO(BAS),
                 TO(BAS), TO(BAS), TO(BAS), TO(BAS),      TO(BAS),                  TO(BAS),
                                   TO(BAS), QK_BOOT,               TO(BAS), TO(BAS)
    )
};

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    static report_mouse_t last;
    if (last && has_mouse_report_changed(&mouse_report, &last)) {
        layer_on(MOU);
    }
    last = mouse_report;
    return mouse_report;
}
