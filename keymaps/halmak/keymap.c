#include QMK_KEYBOARD_H

enum dasbob_layers {
    _HALMAK,
    _LOWER,
    _RAISE,
    _SUPER
};

typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP,
    TD_DOUBLE_HOLD,
} td_state_t;

typedef struct {
    bool       is_press_action;
    td_state_t state;
} td_tap_t;

enum {
    TD_ALT_WIN,
    OTHER_DANCE,
};

td_state_t cur_dance(tap_dance_state_t *state);

void x_finished(tap_dance_state_t *state, void *user_data);
void x_reset(tap_dance_state_t *state, void *user_data);

tap_dance_action_t tap_dance_actions[] = {
    [TD_ALT_WIN] = ACTION_TAP_DANCE_DOUBLE(KC_LALT, KC_LGUI),
    [OTHER_DANCE] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, x_finished, x_reset),
};

td_state_t cur_dance(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return TD_SINGLE_TAP;
        // Key has not been interrupted, but the key is still held. Means you want to send a 'HOLD'.
        else return TD_SINGLE_HOLD;
    } else if (state->count == 2) {
        if (state->pressed) return TD_DOUBLE_HOLD;
        else return TD_DOUBLE_TAP;
    }

    return TD_UNKNOWN;
}

// Create an instance of 'td_tap_t' for the 'x' tap dance.
static td_tap_t xtap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

void x_finished(tap_dance_state_t *state, void *user_data) {
    xtap_state.state = cur_dance(state);
    switch (xtap_state.state) {
        case TD_SINGLE_TAP: register_code(KC_LGUI); break;
        case TD_SINGLE_HOLD: layer_on(1); break;
        case TD_DOUBLE_TAP: register_code(KC_LGUI); break;
        case TD_DOUBLE_HOLD: register_code(KC_LGUI); break;
        default: break;
    }
}

void x_reset(tap_dance_state_t *state, void *user_data) {
    switch (xtap_state.state) {
        case TD_SINGLE_TAP: unregister_code(KC_LGUI); break;
        case TD_SINGLE_HOLD: layer_off(1); break;
        case TD_DOUBLE_TAP: unregister_code(KC_LGUI); break;
        case TD_DOUBLE_HOLD: unregister_code(KC_LGUI); break;
        default: break;
    }
    xtap_state.state = TD_NONE;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
     /*
      * ┌───┬───┬───┬───┬───┐       ┌───┬───┬───┬───┬───┐
      * │ W │ L │ R │ B │ Z │       │ ; │ Q │ U │ D │ J │
      * ├───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┤
      * │ S │ H │ N │ T │ , │       │ . │ A │ E │ O │ I │
      * ├───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┤
      * │ F │ M │ V │ C │ / │       │ G │ P │ X │ K │ Y │
      * └───┴───┴───┴───┴───┘       └───┴───┴───┴───┴───┘
      *           ┌───┐                   ┌───┐
      *           │ALT├───┐           ┌───┤ESC│
      *           └───┤LOW├───┐   ┌───┤RSE├───┘
      *               └───│SPC│   │ENT├───┘
      *                   └───┘   └───┘
      */


    [_HALMAK] = LAYOUT_split_3x5_3(
        KC_W,          KC_L,          KC_R,            KC_B,             KC_Z,               KC_SCLN,         KC_Q,         KC_U,         KC_D,           KC_J,
        KC_S,          KC_H,          KC_N,            KC_T,             KC_COMM,            KC_DOT,          KC_A,         KC_E,         KC_O,           KC_I,
        KC_F,          KC_M,          KC_V,            KC_C,             KC_SLSH,            KC_G,            KC_P,         KC_X,         KC_K,           KC_Y,
                                      TD(TD_ALT_WIN),  TD(OTHER_DANCE),  LCTL_T(KC_SPC),     LSFT_T(KC_ENT),  TL_UPPR,      KC_BSPC
    ),

    [_LOWER] = LAYOUT_split_3x5_3(
        KC_1,          KC_2,          KC_3,            KC_4,             KC_5,               KC_F1,           KC_F2,        KC_F3,            KC_F4,              KC_DEL,
        KC_6,          KC_7,          KC_8,            KC_9,             KC_0,               KC_F5,           KC_F6,        KC_F7,            KC_F8,              XXXXXXX,
        RALT(KC_4),    KC_NUBS,       KC_MINS,         KC_EQL,           KC_NUHS,            KC_F9,           KC_F10,       KC_F11,           KC_F12,             XXXXXXX,
                                      KC_TRNS,         KC_TRNS,          KC_TRNS,            KC_TRNS,         KC_TRNS,      KC_TAB
    ),

    [_RAISE] = LAYOUT_split_3x5_3(
        XXXXXXX,       XXXXXXX,       KC_LBRC,         KC_RBRC,          XXXXXXX,            KC_QUOT,         KC_UP,        S(KC_QUOT),     KC_HOME,     KC_PGUP,
        KC_NUBS,       S(KC_NUBS),    S(KC_9),         S(KC_0),          XXXXXXX,            KC_LEFT,         KC_DOWN,      KC_RIGHT,       KC_END,      KC_PGDN,
        S(KC_NUHS),    KC_NUHS,       S(KC_LBRC),      S(KC_RBRC),       XXXXXXX,            XXXXXXX,         XXXXXXX,      XXXXXXX,        XXXXXXX,     XXXXXXX,
                                      KC_TRNS,         KC_TRNS,          KC_TRNS,            KC_TRNS,         KC_TRNS,      KC_ESC
    ),

    [_SUPER] = LAYOUT_split_3x5_3(
        C(S(KC_ESC)),  XXXXXXX,       XXXXXXX,         XXXXXXX,          XXXXXXX,            KC_PSCR,         XXXXXXX,        XXXXXXX,  XXXXXXX, C(A(KC_DEL)),
        XXXXXXX,       XXXXXXX,       XXXXXXX,         XXXXXXX,          XXXXXXX,            C(G(KC_LEFT)),   C(G(KC_RIGHT)), XXXXXXX,  XXXXXXX, XXXXXXX,
        XXXXXXX,       XXXXXXX,       XXXXXXX,         XXXXXXX,          XXXXXXX,            C(S(KC_TAB)),    C(KC_TAB),      XXXXXXX,  XXXXXXX, XXXXXXX,
                                      KC_TRNS,         KC_TRNS,          KC_TRNS,            KC_TRNS,         KC_TRNS,        KC_TRNS
    ),

};
#ifdef OLED_ENABLE

oled_rotation_t oled_init_user(oled_rotation_t rotation) { return OLED_ROTATION_0; }
bool oled_task_user(void) {
    if (is_keyboard_master()) {
        static const char PROGMEM runqmk_logo[] = {0,0,0,0,152,152,152,152,152,152,24,24,24,
        152,152,24,24,24,152,152,24,24,152,152,24,24,24,152,152,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,127,127,13,13,29,63,103,64,0,63,127,96,96,96,127,63,0,0,127,127,7,12,56,
        127,127,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,120,252,134,198,198,252,120,0,0,254,254,
        60,224,224,60,254,254,0,254,254,16,24,124,230,130,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,24,24,25,25,25,25,27,24,24,25,25,24,25,25,24,25,25,24,25,25,24,24,24,24,25,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0};


        oled_write_raw_P(runqmk_logo, sizeof(runqmk_logo));

        led_t led_usb_state = host_keyboard_led_state();
        oled_set_cursor(6, 3);
        oled_write_P(led_usb_state.num_lock    ? PSTR("NUM") : PSTR(""), false);
        oled_set_cursor(6, 2);
        oled_write_P(PSTR("WPM: "), false);
        oled_write(get_u8_str(get_current_wpm(), '0'), false);
        oled_set_cursor(6, 0);
        oled_write_P(PSTR("by GroooveBob"), false);
            oled_set_cursor(6, 1);
            oled_write_P(PSTR("Layer: "), false);

    switch (get_highest_layer(layer_state)) {
        case _HALMAK:
        oled_set_cursor(12, 1);
            oled_write_P(PSTR("Default\n"), false);
            break;
        case _LOWER:
        oled_set_cursor(12, 1);
            oled_write_P(PSTR("Lower\n"), false);
            break;
        case _RAISE:
        oled_set_cursor(12, 1);
            oled_write_P(PSTR("Raise\n"), false);
            break;
        case _SUPER:
        oled_set_cursor(12, 1);
            oled_write_P(PSTR("Raise\n"), false);
            break;
        default:
            // Or use the write_ln shortcut over adding '\n' to the end of your string
        oled_set_cursor(6, 1);
        oled_write_ln_P(PSTR("Undefined"), false);
    }
    } else {
        static const char PROGMEM dasbob_logo[] = {
        0,  0,  0,  0,  0,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,  0,  0,  0,  0,  0,128,128,128,192,192,192,192,192,192,192,192,192,192,192,128,128,  0,  0,  0,  0,  0,  0,  0,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,  0,  0,  0,  0,  0,128,128,128,128,128,128,128,128,128,128,128,128,128,  0,  0,  0,128,128,128,128,128,192,192,192,192,192,  0,  0,  0,128,128,128,128,128,128,128,128,128,  0,  0,  0,  0,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,  0,  0,
        0,  0,126,127,255,255,255,255,255,255,255,199,247,255,255,255,255,255,255,255,255,127,252,255,255,255,255,255,255,255,255,255, 15,127,255,255,255,255,255,255,255,255,254,248,  0,126,127,255,255,255,255,255,255,255,199,247,255,255,255,255,255,255,255,255,127,126,255,255,255,255,255,255,255,255,239,239,239,239,207,223,223,223,159, 63,  0,  7, 63,255,255,255,255,255,255,255,254, 14,254,255,255,255,255,255,255,255, 63,241,252,254,255,255,255,255,255,255,255, 31,255,255,255,255,255,255,255,255,255,255,128,  0,
        0,  0,126,255,255,255,255,255,255,255,255,227,255,255,255,255,255,255,255,255,255,  0, 15, 63,127,255,255,255,255,255,255,255,248,255,255,255,255,255,255,255,255,127, 63,  7,  0,126,255,255,255,255,255,255,255,255,227,255,255,255,255,255,255,255,255,255,128,252,253,249,249,251,251,251,251,251,255,255,255,255,255,255,255,255,127, 30,  0,  0,  0,  3, 31,255,255,255,255,255,255,255,255,255,255,255,255,255,255,  7,  0, 63,255,255,255,255,255,255,255,255,255,248,255,255,255,255,255,255,255,255,255,255,255,  0,
        0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,
        };
        oled_write_raw_P(dasbob_logo, sizeof(dasbob_logo));
    }
    return false;
}
#endif
