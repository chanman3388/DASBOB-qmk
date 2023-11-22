#include QMK_KEYBOARD_H

enum dasbob_layers {
    _HALMAK,
    _LOWER,
    _RAISE,
    _SUPER,
    _FUNCTION
};

typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_HOLD,
    TD_DOUBLE,
} td_state_t;

typedef struct {
    bool       is_press_action;
    td_state_t state;
} td_tap_t;

enum {
    TD_ALT_WIN,
    L1_SHFT,
    L2_CTRL,
};

td_state_t cur_dance(tap_dance_state_t *state);

void l1_finished(tap_dance_state_t *state, void *user_data);
void l1_reset(tap_dance_state_t *state, void *user_data);
void l2_finished(tap_dance_state_t *state, void *user_data);
void l2_reset(tap_dance_state_t *state, void *user_data);

tap_dance_action_t tap_dance_actions[] = {
    [TD_ALT_WIN] = ACTION_TAP_DANCE_DOUBLE(KC_LALT, KC_LGUI),
    [L1_SHFT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, l1_finished, l1_reset),
    [L2_CTRL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, l2_finished, l2_reset),
};

td_state_t cur_dance(tap_dance_state_t *state) {
    if (state->count == 1) {
        return TD_SINGLE_HOLD;
    } else {
        return TD_DOUBLE;
    }

    return TD_UNKNOWN;
}

// Create an instance of 'td_tap_t' for the 'l1' tap dance.
static td_tap_t l1_tap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

// Create an instance of 'td_tap_t' for the 'l2' tap dance.
static td_tap_t l2_tap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

void l1_finished(tap_dance_state_t *state, void *user_data) {
    l1_tap_state.state = cur_dance(state);
    switch (l1_tap_state.state) {
        case TD_SINGLE_HOLD:
            layer_on(1);
            update_tri_layer(_LOWER, _RAISE, _SUPER);
            break;
        case TD_DOUBLE: register_code(KC_LSFT); break;
        default: break;
    }
}

void l1_reset(tap_dance_state_t *state, void *user_data) {
    switch (l1_tap_state.state) {
        case TD_SINGLE_HOLD:
            layer_off(1);
            update_tri_layer(_LOWER, _RAISE, _SUPER);
            break;
        case TD_DOUBLE: unregister_code(KC_LSFT); break;
        default: break;
    }
    l1_tap_state.state = TD_NONE;
}

void l2_finished(tap_dance_state_t *state, void *user_data) {
    l2_tap_state.state = cur_dance(state);
    switch (l2_tap_state.state) {
        case TD_SINGLE_HOLD:
            layer_on(2);
            update_tri_layer(_LOWER, _RAISE, _SUPER);
            break;
        case TD_DOUBLE: register_code(KC_LCTL); break;
        default: break;
    }
}

void l2_reset(tap_dance_state_t *state, void *user_data) {
    switch (l2_tap_state.state) {
        case TD_SINGLE_HOLD:
            layer_off(2);
            update_tri_layer(_LOWER, _RAISE, _SUPER);
            break;
        case TD_DOUBLE: unregister_code(KC_LCTL); break;
        default: break;
    }
    l2_tap_state.state = TD_NONE;
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
        KC_W,                   KC_L,               KC_R,           KC_B,               KC_Z,               KC_SCLN,            KC_Q,           KC_U,           KC_D,           KC_J,
        KC_S,                   KC_H,               KC_N,           KC_T,               KC_COMM,            KC_DOT,             KC_A,           KC_E,           KC_O,           KC_I,
        LT(_FUNCTION, KC_F),    KC_M,               KC_V,           KC_C,               KC_SLSH,            KC_G,               KC_P,           KC_X,           KC_K,           KC_Y,
                                TD(TD_ALT_WIN),     TD(L1_SHFT),    LCTL_T(KC_SPC),     LSFT_T(KC_ENT),     TD(L2_CTRL),        KC_BSPC
    ),

    [_LOWER] = LAYOUT_split_3x5_3(
        KC_1,                   KC_2,               KC_3,            KC_4,              KC_5,               S(KC_1),            S(KC_2),        S(KC_3),        S(KC_4),        S(KC_5),
        KC_6,                   KC_7,               KC_8,            KC_9,              KC_0,               S(KC_6),            S(KC_7),        S(KC_8),        KC_QUOT,        KC_NUHS,
        KC_GRV,                 KC_NUBS,            KC_MINS,         KC_EQL,            KC_NUHS,            S(KC_NUHS),         S(KC_EQL),      S(KC_MINS),     S(KC_NUBS),     KC_DEL,
                                                    KC_TRNS,         KC_TRNS,           KC_TRNS,            KC_TRNS,            KC_TRNS,        KC_TAB
    ),

    [_RAISE] = LAYOUT_split_3x5_3(
        KC_TAB,                 XXXXXXX,            KC_LBRC,         KC_RBRC,           XXXXXXX,            KC_QUOT,            KC_UP,          S(KC_QUOT),     KC_HOME,     KC_PGUP,
        XXXXXXX,                XXXXXXX,            S(KC_9),         S(KC_0),           RALT(KC_4),         KC_LEFT,            KC_DOWN,        KC_RIGHT,       KC_END,      KC_PGDN,
        KC_TAB,                 XXXXXXX,            S(KC_LBRC),      S(KC_RBRC),        XXXXXXX,            XXXXXXX,            XXXXXXX,        XXXXXXX,        XXXXXXX,     KC_BSPC,
                                                    KC_TRNS,         KC_TRNS,           KC_TRNS,            KC_TRNS,            KC_TRNS,        KC_ESC
    ),

    [_SUPER] = LAYOUT_split_3x5_3(
        C(S(KC_ESC)),  XXXXXXX,       XXXXXXX,         XXXXXXX,          XXXXXXX,            KC_PSCR,         XXXXXXX,        XXXXXXX,          XXXXXXX, C(A(KC_DEL)),
        XXXXXXX,       XXXXXXX,       A(S(KC_TAB)),    A(KC_TAB),        XXXXXXX,            XXXXXXX,         C(G(KC_LEFT)),  C(G(KC_RIGHT)),   XXXXXXX, XXXXXXX,
        XXXXXXX,       XXXXXXX,       C(S(KC_TAB)),    C(KC_TAB),        XXXXXXX,            XXXXXXX,         C(S(KC_TAB)),   C(KC_TAB),        XXXXXXX, XXXXXXX,
                                      KC_TRNS,         KC_TRNS,          KC_TRNS,            KC_TRNS,         KC_TRNS,        KC_TRNS
    ),

    [_FUNCTION] = LAYOUT_split_3x5_3(
        XXXXXXX,        XXXXXXX,        XXXXXXX,        XXXXXXX,        XXXXXXX,        KC_F1,          KC_F2,          KC_F3,          KC_F4,          XXXXXXX,
        XXXXXXX,        XXXXXXX,        XXXXXXX,        XXXXXXX,        XXXXXXX,        KC_F5,          KC_F6,          KC_F7,          KC_F8,          XXXXXXX,
        XXXXXXX,        XXXXXXX,        XXXXXXX,        XXXXXXX,        XXXXXXX,        KC_F9,          KC_F10,         KC_F11,         KC_F12,         XXXXXXX,
                                        KC_TRNS,        KC_TRNS,        KC_TRNS,        KC_TRNS,        KC_TRNS,        KC_TRNS
    ),

};
#ifdef OLED_ENABLE
#include "oled.c"
#endif
