#include QMK_KEYBOARD_H

enum layers {
    BAS,
    SYM,
    NAV,
    MOU,
    MDA,
    UTL
};

enum custom_keycodes {
    KC_MSSC = SAFE_RANGE,
    KC_DRAG,
    KC_CDCL,
    KC_ALCL,
    KC_SCST,
    KC_APSW,
    KC_NAV,
};

#define KC_BACK LCMD(KC_LBRC)
#define KC_FRWD LCMD(KC_RBRC)
#define KC_WN_L LOPT(LCMD(KC_LEFT))
#define KC_WN_R LOPT(LCMD(KC_RGHT))
#define KC_WN_F LOPT(LCMD(KC_F))
#define KC_VIM LCTL(KC_F)
#define KC_EMJI LCTL(LCMD(KC_SPC))

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BAS] = LAYOUT(
        KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,         KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,
        KC_A,    KC_S,    KC_D,    KC_F,    KC_G,         KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN,
        KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,         KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,
                 KC_LBRC, KC_MINS,                                          KC_QUOT, KC_EQL,
                 KC_LCMD, KC_SPC,  KC_LSFT, KC_LOPT,      KC_NAV,                    KC_BSPC,
                                   MO(SYM), KC_ESC,                KC_TAB,  KC_ENT
    ),
    [SYM] = LAYOUT(
        G(KC_LEFT), G(KC_DOWN), G(KC_UP), G(KC_RGHT), TO(BAS),      KC_GRV,  KC_7,    KC_8,    KC_9,    TO(BAS),
        KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_VIM,       KC_DOT,  KC_4,    KC_5,    KC_6,    KC_BSLS,
        A(KC_LEFT), A(KC_DOWN), A(KC_UP), A(KC_RGHT), TO(BAS),      TO(BAS), KC_1,    KC_2,    KC_3,    KC_EMJI,
                 KC_RBRC, TO(BAS),                                          KC_0,    TO(BAS),
                 KC_LCMD, KC_SPC,  KC_LSFT, KC_LOPT,      TO(BAS),                    KC_BSPC,
                                   _______, TO(BAS),               TO(BAS), KC_ENT
    ),
    [NAV] = LAYOUT(
        QK_BOOT, TO(BAS), TO(BAS), TO(BAS), TO(BAS),      TO(BAS), _______, _______, TO(BAS), TO(BAS),
        TO(BAS), KC_WN_L, KC_WN_F, KC_WN_R, TO(BAS),      KC_WH_U, G(KC_GRV), KC_APSW, KC_BACK, KC_FRWD,
        TO(BAS), TO(BAS), TO(BAS), TO(BAS), TO(BAS),      KC_WH_D, S(G(KC_LBRC)), S(G(KC_RBRC)), TO(BAS), TO(BAS),
                 TO(BAS), TO(BAS),                                          TO(BAS), TO(BAS),
                 TO(BAS), TO(BAS), TO(BAS), TO(BAS),      _______,                   TO(BAS),
                                   TO(BAS), TO(BAS),               MO(MDA), TO(BAS)
    ),
    [MDA] = LAYOUT(
        _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______,      _______, KC_VOLU, KC_MPLY, KC_MNXT, _______,
        _______, _______, _______, _______, _______,      _______, KC_VOLD, _______, _______, _______,
                 _______, _______,                                          _______, _______,
                 _______, _______, _______, _______,      _______,                   _______,
                                   _______, _______,               _______, _______
    ),
    [MOU] = LAYOUT(
        _______, _______, _______, _______, _______,      KC_SCST, KC_DRAG, _______, _______, _______,
        _______, _______, _______, _______, _______,      KC_CDCL, KC_BTN1, KC_BTN2, KC_MSSC, TO(BAS),
        _______, _______, _______, _______, _______,      KC_ALCL, _______, _______, _______, _______,
                 _______, _______,                                          _______, _______,
                 _______, _______, _______, _______,      _______,                   _______,
                                   _______, _______,               _______, _______
    )
};

bool is_scrolling = false;
bool waiting_for_scroll = false;
deferred_token mouse_clear_token = INVALID_DEFERRED_TOKEN;

void keyboard_post_init_user(void) {
  debug_config.matrix = false;
  debug_config.mouse = false;
}

uint32_t clear_mouse(uint32_t trigger_time, void *cb_arg) {
    layer_off(MOU);
    mouse_clear_token = INVALID_DEFERRED_TOKEN;
    return 0;
}

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    static report_mouse_t last;
    static bool init = false;
    static int16_t scroll_buffer_x = 0;
    static int16_t scroll_buffer_y = 0;
    bool changed = init && (mouse_report.x != last.x || mouse_report.y != last.y);
    last = mouse_report;
    init = true;
    if (changed) {
        if (!layer_state_is(MOU)) {
            is_scrolling = false;
            pointing_device_set_cpi(PMW33XX_CPI);
            layer_on(MOU);
        }
        if (is_scrolling) {
            waiting_for_scroll = false;
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
        if (mouse_clear_token != INVALID_DEFERRED_TOKEN) {
            cancel_deferred_exec(mouse_clear_token);
            mouse_clear_token = INVALID_DEFERRED_TOKEN;
        }
    }
    return mouse_report;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    static bool is_dragging = false;
    switch (keycode) {
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
            waiting_for_scroll = is_scrolling;
            pointing_device_set_cpi(is_scrolling ? 100 : PMW33XX_CPI);
        } else {
            if (!waiting_for_scroll) {
                is_scrolling = false;
                pointing_device_set_cpi(PMW33XX_CPI);
            }
        }
        break;
        case KC_NAV:
        if (record->event.pressed) {
            layer_on(NAV);
        } else {
            layer_off(NAV);
            unregister_code(KC_LGUI);
        }
        break;
        case KC_APSW:
        if (record->event.pressed) {
            register_code(KC_LGUI);
            register_code(KC_TAB);
        } else {
            unregister_code(KC_TAB);
        }
        break;
    }
    if (layer_state_is(MOU)) {
        if (keymaps[MOU][record->event.key.row][record->event.key.col] == KC_TRNS) {
            layer_off(MOU);
        }
    }
    switch (keycode) {
        case KC_BTN2:
        case KC_CDCL:
        case KC_ALCL:
        break;
        if (!record->event.pressed) {
            layer_off(MOU);
        }
        case KC_BTN1:
        if (!record->event.pressed) {
            if (mouse_clear_token == INVALID_DEFERRED_TOKEN) {
                mouse_clear_token = defer_exec(250, clear_mouse, NULL);
            }
        }
        break;
    }
    return true;
}
