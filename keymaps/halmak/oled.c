#define TEXT_MOVE_TIMEOUT 250
#define OLED_ROW_START 6
#define OLED_WRITEABLE_WIDTH 15
#define BUFF_LEN 16
#define START_COUNT 6  // for pause at the beginning of a quotes
#define END_COUNT 4  // for a pause at the end of scrolling
#define NUM_QUOTES 5

static const char *quotes[] = {
    "Behind every cloud is a Bitcoin miner...",
    "Jono's mum's a nice lady!",
    "You miss every shot you don't take",
    "I'm quite a fan of this sausage",
    "Every screen is a touch screen, if you get close enough"
};
static uint8_t quote_len[] = {40, 25, 34, 31, 55};  // quote lendth without null terminator

char buff[BUFF_LEN];

static uint16_t anim_timer = 0;
bool first = true;

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

    return OLED_ROTATION_0;
}
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
        0,0,0,0,0,0,0,0,0,0}; // 512  128 x 4

        if (first) {
            oled_write_raw_P(runqmk_logo, sizeof(runqmk_logo));
            render_text();
            first = false;
        }

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