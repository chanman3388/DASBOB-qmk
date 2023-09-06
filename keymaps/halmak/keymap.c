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
        XXXXXXX,                XXXXXXX,            KC_LBRC,         KC_RBRC,           XXXXXXX,            KC_QUOT,            KC_UP,          S(KC_QUOT),     KC_HOME,     KC_PGUP,
        XXXXXXX,                XXXXXXX,            S(KC_9),         S(KC_0),           RALT(KC_4),         KC_LEFT,            KC_DOWN,        KC_RIGHT,       KC_END,      KC_PGDN,
        S(KC_NUHS),             KC_NUHS,            S(KC_LBRC),      S(KC_RBRC),        XXXXXXX,            XXXXXXX,            XXXXXXX,        XXXXXXX,        XXXXXXX,     XXXXXXX,
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

#define TEXT_MOVE_TIMEOUT 250
#define OLED_ROW_START 6
#define OLED_WRITEABLE_WIDTH 15
#define BUFF_LEN 16
#define START_COUNT 6
#define END_COUNT 4
#define NUM_QUOTES 4

static const char *quotes[] = {
    "Behind every cloud is a Bitcoin miner...",
    "Jono's mum's a nice lady!",
    "You miss every shot you don't take",
    "I'm quite a fan of this sausage"
};
static uint8_t quote_len[] = {40, 25, 34, 31};  // quote lendth without null terminator

char buff[BUFF_LEN];

static uint16_t anim_timer = 0;

typedef struct {
    uint8_t start_idx;
    uint8_t end_idx;
    uint8_t quote_id;
    uint8_t counter;
} quote_state;

static quote_state oled_quote_state = {
    0,
    0,
    0,
    0,
};

void render_text(void) {
    uint8_t end, i;
    if (oled_quote_state.start_idx >= quote_len[oled_quote_state.quote_id] && oled_quote_state.counter > 1) goto check_end;
    if (oled_quote_state.end_idx < quote_len[oled_quote_state.quote_id]) {
        end = OLED_WRITEABLE_WIDTH;
    } else {
        end = quote_len[oled_quote_state.quote_id] - oled_quote_state.start_idx;        
    }
    for (i = 0; i < end; i++) buff[i] = quotes[oled_quote_state.quote_id][oled_quote_state.start_idx + i];
    if (end < OLED_WRITEABLE_WIDTH) {
        for (i = end; i < OLED_WRITEABLE_WIDTH; i++) buff[i] = ' ';
    }
    if (end == 0) {
        for (i = 0; i < OLED_WRITEABLE_WIDTH; i++) buff[i] = ' ';
    }
    if (oled_quote_state.counter == 0 || end == 0) {
        oled_set_cursor(OLED_ROW_START, 3);
        oled_write_P(PSTR(buff), false);
    }
    if (oled_quote_state.start_idx == 0) {
        if (oled_quote_state.counter < START_COUNT) {
            oled_quote_state.counter++;
            goto skip;
        } else {
            oled_quote_state.counter = 0;
        }
    }
    oled_quote_state.start_idx++;
    oled_quote_state.end_idx++;
    if (oled_quote_state.start_idx >= quote_len[oled_quote_state.quote_id]) {
    check_end:
        oled_quote_state.counter++;
        if (oled_quote_state.counter <= END_COUNT) {
            goto skip;
        }
        oled_quote_state.counter = 0;
        oled_quote_state.start_idx = 0;
        oled_quote_state.end_idx = OLED_WRITEABLE_WIDTH;
        oled_quote_state.quote_id++;
        if (oled_quote_state.quote_id == NUM_QUOTES) {
            oled_quote_state.quote_id = 0;
        }
    }
skip:
    return;
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    anim_timer = timer_read();
    buff[OLED_WRITEABLE_WIDTH] = '\0';
    oled_quote_state.start_idx = 0;
    oled_quote_state.end_idx = OLED_WRITEABLE_WIDTH;
    render_text();
    return OLED_ROTATION_0;
}
bool oled_task_user(void) {
    if (is_keyboard_master()) {
        // static const char PROGMEM runqmk_logo[] = {0,0,0,0,152,152,152,152,152,152,24,24,24,
        // 152,152,24,24,24,152,152,24,24,152,152,24,24,24,152,152,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        // 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        // 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        // 0,0,0,0,0,127,127,13,13,29,63,103,64,0,63,127,96,96,96,127,63,0,0,127,127,7,12,56,
        // 127,127,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        // 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        // 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,120,252,134,198,198,252,120,0,0,254,254,
        // 60,224,224,60,254,254,0,254,254,16,24,124,230,130,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        // 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        // 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        // 0,0,24,24,25,25,25,25,27,24,24,25,25,24,25,25,24,25,25,24,25,25,24,24,24,24,25,0,0,0,
        // 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        // 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        // 0,0,0,0,0,0,0,0,0,0}; // 512  128 x 4
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
        0,0,24,24,25,25,25,25,27,24,24,25,25,24,25,25,24,25,25,24,25,25,24,24,24,24,25,0,0,0};

        oled_write_raw_P(runqmk_logo, sizeof(runqmk_logo));

        // time for the next frame?
        if (timer_elapsed(anim_timer) > TEXT_MOVE_TIMEOUT) {
            anim_timer = timer_read();
            render_text();
        }
        oled_set_cursor(OLED_ROW_START, 2);
        oled_write_P(PSTR("WPM: "), false);
        oled_write(get_u8_str(get_current_wpm(), '0'), false);
        oled_set_cursor(OLED_ROW_START, 0);
        oled_write_P(PSTR("by GroooveBob"), false);
            oled_set_cursor(OLED_ROW_START, 1);
            oled_write_P(PSTR("Layer: "), false);

    switch (get_highest_layer(layer_state)) {
        case _HALMAK:
            oled_set_cursor(12, 1);
            oled_write_P(PSTR("halmak\n"), false);
            break;
        case _LOWER:
            oled_set_cursor(12, 1);
            oled_write_P(PSTR("lower\n"), false);
            break;
        case _RAISE:
            oled_set_cursor(12, 1);
            oled_write_P(PSTR("raise\n"), false);
            break;
        case _SUPER:
            oled_set_cursor(12, 1);
            oled_write_P(PSTR("super\n"), false);
            break;
        case _FUNCTION:
            oled_set_cursor(12, 1);
            oled_write_P(PSTR("function\n"), false);
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
