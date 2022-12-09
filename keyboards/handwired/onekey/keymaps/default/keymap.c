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
    NAV,
    MOU,
    UTL
};

enum custom_keycodes {
    KC_MSSC = SAFE_RANGE,
    KC_DRAG,
    KC_CDCL,
    KC_ALCL,
    KC_SYM,
    KC_SCST,
};

#define KC_BACK LCMD(KC_LBRC)
#define KC_FRWD LCMD(KC_RBRC)
#define KC_WN_L LOPT(LCMD(KC_LEFT))
#define KC_WN_R LOPT(LCMD(KC_RGHT))
#define KC_WN_F LOPT(LCMD(KC_F))
#define KC_NAV LM(NAV, MOD_LGUI)
#define KC_VIM LCTL(KC_F)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BAS] = LAYOUT(
        KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,         KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,
        KC_A,    KC_S,    KC_D,    KC_F,    KC_G,         KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN,
        KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,         KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,
                 KC_LBRC, KC_MINS,                                          KC_QUOT, KC_EQL,
                 KC_LCMD, KC_SPC,  KC_LSFT, KC_LOPT,      KC_NAV,                    KC_BSPC,
                                   KC_SYM,  KC_ESC,                KC_TAB,  KC_ENT
    ),
    [SYM] = LAYOUT(
        G(KC_LEFT), G(KC_DOWN), G(KC_UP), G(KC_RGHT), TO(BAS),      KC_GRV,  KC_7,    KC_8,    KC_9,    QK_BOOT,
        KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_VIM,       KC_DOT,  KC_4,    KC_5,    KC_6,    TO(BAS),
        A(KC_LEFT), A(KC_DOWN), A(KC_UP), A(KC_RGHT), TO(BAS),      TO(BAS), KC_1,    KC_2,    KC_3,    TO(BAS),
                 TO(BAS), TO(BAS),                                          KC_0,    TO(BAS),
                 KC_LCMD, KC_SPC,  KC_LSFT, KC_LOPT,      KC_NAV,                    KC_BSPC,
                                   KC_SYM,  TO(BAS),               TO(BAS), KC_ENT
    ),
    [NAV] = LAYOUT(
        TO(BAS), TO(BAS), TO(BAS), TO(BAS), TO(BAS),      TO(BAS), TO(BAS), TO(BAS), TO(BAS), TO(BAS),
        TO(BAS), KC_WN_L, KC_WN_F, KC_WN_R, TO(BAS),      TO(BAS), KC_GRV,  KC_TAB, TO(BAS), TO(BAS),
        TO(BAS), TO(BAS), TO(BAS), TO(BAS), TO(BAS),      TO(BAS), S(KC_LBRC), S(KC_RBRC), TO(BAS), TO(BAS),
                 TO(BAS), TO(BAS),                                          TO(BAS), TO(BAS),
                 TO(BAS), TO(BAS), TO(BAS), TO(BAS),      _______,                   TO(BAS),
                                   TO(BAS), TO(BAS),               TO(BAS), TO(BAS)
    ),
    [MOU] = LAYOUT(
        _______, _______, _______, _______, _______,      KC_SCST, KC_DRAG, KC_BACK, KC_FRWD, _______,
        _______, _______, _______, _______, _______,      KC_CDCL, KC_BTN1, KC_BTN2, KC_MSSC, TO(BAS),
        _______, _______, _______, _______, _______,      KC_ALCL, KC_WH_U, KC_WH_D, _______, _______,
                 _______, _______,                                          _______, _______,
                 _______, _______, _______, _______,      _______,                   _______,
                                   _______, _______,               _______, _______
    )
};

bool is_scrolling = false;

void keyboard_post_init_user(void) {
  debug_config.matrix = false;
  debug_config.mouse = false;
}

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    static report_mouse_t last;
    static bool init = false;
    static int16_t scroll_buffer_x = 0;
    static int16_t scroll_buffer_y = 0;
    if (init && has_mouse_report_changed(&mouse_report, &last) && !layer_state_is(MOU)) {
        is_scrolling = false;
        pointing_device_set_cpi(PMW33XX_CPI);
        layer_on(MOU);
    }
    init = true;
    last = mouse_report;
    if (is_scrolling) {
        scroll_buffer_x += mouse_report.x;
        scroll_buffer_y += mouse_report.y;
        if (abs(scroll_buffer_x) > 6) {
            mouse_report.h = scroll_buffer_x > 0 ? 1 : -1;
            scroll_buffer_x = 0;
        }
        if (abs(scroll_buffer_y) > 6) {
            mouse_report.v = scroll_buffer_y > 0 ? -1 : 1;
            scroll_buffer_y = 0;
        }
        mouse_report.x = 0;
        mouse_report.y = 0;
    }
    return mouse_report;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    static bool is_dragging = false;
    static uint16_t last_key = 0;
    switch (keycode) {
        case KC_SYM:
        if (record->event.pressed) {
            layer_invert(SYM);
        } else if (last_key != KC_SYM) {
            layer_off(SYM);
        }
        break;
        case KC_CDCL:
        if (record->event.pressed) {
            register_code(KC_LCMD);
        } else {
            tap_code(KC_BTN1);
            unregister_code(KC_LCMD);
        }
        break;
        case KC_ALCL:
        if (record->event.pressed) {
            register_code(KC_LALT);
        } else {
            tap_code(KC_BTN1);
            unregister_code(KC_LALT);
        }
        break;
        case KC_DRAG:
        if (record->event.pressed) {
            is_dragging = !is_dragging;
            if (is_dragging) {
                register_code(KC_BTN1);
            } else {
                unregister_code(KC_BTN1);
            }
        }
        break;
        case KC_SCST:
        if (record->event.pressed) {
            tap_code16(LCTL(LSFT(LCMD(KC_4))));
        } else {
            is_dragging = true;
            register_code(KC_BTN1);
        }
        break;
        case KC_BTN1:
        if (record->event.pressed) {
            is_dragging = false;
        }
        break;
        case KC_MSSC:
        if (record->event.pressed) {
            is_scrolling = !is_scrolling;
            pointing_device_set_cpi(is_scrolling ? 100 : PMW33XX_CPI);
        }
        break;
    }
    if (layer_state_is(MOU)) {
        if (keymaps[MOU][record->event.key.row][record->event.key.col] == KC_TRNS) {
            layer_off(MOU);
        }
    }
    if (record->event.pressed) {
        last_key = keycode;
    }
    return true;
}
