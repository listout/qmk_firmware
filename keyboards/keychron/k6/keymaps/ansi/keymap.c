/*
Copyright 2020 Dimitris Mantzouranis
Copyright 2022 Philip Mourdjis

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifdef CONSOLE_ENABLE
#include "print.h"
#endif

#ifdef USE_VIM
#include "qmk-vim/src/vim.h"
#include "qmk-vim/src/modes.h"
#endif

#include QMK_KEYBOARD_H

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
enum layer_names {
    WIN_BASE,
    WIN_FN1,
    FN2,
    RGB_BASE,
    RGB_CHANGE,
#ifdef MOUSE_ENABLE
    MOUSE_LAYER,
    MOUSE_LAYER_2,
#endif
};

enum custom_keycodes {
    QMK_VIM = SAFE_RANGE,
    KC_MISSION_CONTROL,
    KC_LAUNCHPAD
};

#define KC_TASK LGUI(KC_TAB)        // Task viewer
#define KC_FLXP LGUI(KC_E)          // Windows file explorer
#define KC_MCTL KC_MISSION_CONTROL  // Mission Control
#define KC_LPAD KC_LAUNCHPAD        // Launchpad

HSV hsv_y;
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Windows Base
     * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────┬───┐
     * │Esc│ 1 │ 2 │ 3 │ 4 │ 5 │ 6 │ 7 │ 8 │ 9 │ 0 │ - │ = │ Backsp│MOD│
     * ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┼───┤
     * │ Tab │ Q │ W │ E │ R │ T │ Y │ U │ I │ O │ P │ [ │ ] │  \  │HOM│
     * ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴─────┼───┤
     * │ Caps │ A │ S │ D │ F │ G │ H │ J │ K │ L │ ; │ ' │  Enter │PGU│
     * ├──────┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴────┬───┼───┤
     * │ Shift  │ Z │ X │ C │ V │ B │ N │ M │ , │ . │ / │ Shift│ ↑ │PGD│
     * ├────┬───┴┬──┴─┬─┴───┴───┴───┴───┴───┴──┬┴──┬┴──┬┴──┬───┼───┼───┤
     * │Ctrl│GUI │Alt │         Space          │Ctl│Fn1│Fn2│ ← │ ↓ │ → │
     * └────┴────┴────┴────────────────────────┴───┴───┴───┴───┴───┴───┘
     */
    [WIN_BASE] = LAYOUT_65_ansi(
        KC_ESC,  KC_1,    KC_2,    KC_3, KC_4, KC_5, KC_6,   KC_7, KC_8, KC_9,    KC_0,     KC_MINS,     KC_EQL,  KC_BSPC,            MO(RGB_CHANGE),
        KC_TAB,  KC_Q,    KC_W,    KC_E, KC_R, KC_T, KC_Y,   KC_U, KC_I, KC_O,    KC_P,     KC_LBRC,     KC_RBRC, KC_BSLASH,          KC_HOME,
        QMK_VIM, KC_A,    KC_S,    KC_D, KC_F, KC_G, KC_H,   KC_J, KC_K, KC_L,    KC_SCLN,  KC_QUOT,              KC_ENT,             KC_PGUP,
        KC_LSFT,          KC_Z,    KC_X, KC_C, KC_V, KC_B,   KC_N, KC_M, KC_COMM, KC_DOT,   KC_SLSH,              KC_RSFT,   KC_UP,   KC_PGDOWN,
#ifdef MOUSE_ENABLE
        KC_LCTL, KC_LGUI, KC_LALT,                LT(MOUSE_LAYER, KC_SPC),                      LT(RGB_BASE, KC_RCTRL), MO(WIN_FN1), MO(FN2), KC_LEFT,   KC_DOWN, KC_RGHT
#else
        KC_LCTL, KC_LGUI, KC_LALT,                KC_SPC,                      LT(RGB_BASE, KC_RCTRL), MO(WIN_FN1), MO(FN2), KC_LEFT,   KC_DOWN, KC_RGHT
#endif
    ),
    /* Windows FN1
     * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────┬───┐
     * │`~ │BRU│BRD│TSK│FLX│VAD│VAI│PRV│PLY│NXT│MTE│VLD│VLU│       │TOG│
     * ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┼───┤
     * │     │   │   │   │   │   │   │   │   │   │INS│DEL│END│     │   │
     * ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴─────┼───┤
     * │      │   │   │   │   │   │   │   │   │   │   │   │        │   │
     * ├──────┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴────┬───┼───┤
     * │        │   │   │   │   │   │   │   │   │   │   │      │   │   │
     * ├────┬───┴┬──┴─┬─┴───┴───┴───┴───┴───┴──┬┴──┬┴──┬┴──┬───┼───┼───┤
     * │    │    │    │                        │Alt│   │   │   │   │   │
     * └────┴────┴────┴────────────────────────┴───┴───┴───┴───┴───┴───┘
     */
    [WIN_FN1] = LAYOUT_65_ansi(
        KC_GRV,  KC_BRIU, KC_BRID, KC_TASK, KC_FLXP, RGB_VAD, RGB_VAI, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD, KC_VOLU, _______,          RGB_TOG,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_INS,  KC_DEL,  KC_END,  _______,          _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,          _______,
        _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______,
        _______, _______, _______,                            _______,                            KC_RALT, _______, _______, _______, _______, _______
    ),
    /* FN2
     * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────┬───┐
     * │ ~ │ F1│ F2│ F3│ F4│ F5│ F6│ F7│ F8│ F9│F10│F11│F12│ RESET │RBS│
     * ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┼───┤
     * │     │   │   │   │   │   │   │   │   │   │   │   │   │     │   │
     * ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴─────┼───┤
     * │      │   │   │   │   │   │   │   │   │   │   │   │        │   │
     * ├──────┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴────┬───┼───┤
     * │        │   │   │   │   │   │   │   │   │   │   │      │   │   │
     * ├────┬───┴┬──┴─┬─┴───┴───┴───┴───┴───┴──┬┴──┬┴──┬┴──┬───┼───┼───┤
     * │    │    │    │                        │   │   │   │   │   │   │
     * └────┴────┴────┴────────────────────────┴───┴───┴───┴───┴───┴───┘
     */
    [FN2] = LAYOUT_65_ansi(
        KC_TILD, KC_F1,   KC_F2,     KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,  KC_F12,  RESET,           _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,          KC_PSCR,
        _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______,
        _______, _______, _______,                            _______,                            _______, _______, _______, _______, _______, _______
    ),
    [RGB_BASE] = LAYOUT_65_ansi(
        _______, _______, _______, _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, RGB_M_P, RGB_M_B, RGB_M_R, RGB_M_SW, RGB_M_K, RGB_M_X, RGB_M_G, RGB_M_T, _______, _______, _______, _______, _______,          _______,
        _______, _______, _______, _______,  _______, _______, _______, _______, _______, _______, _______, _______,          _______,          _______,
        _______,          _______, _______,  _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______,
        _______, _______, _______,                             _______,                            _______, _______, _______, _______, _______, _______
    ),
    [RGB_CHANGE] = LAYOUT_65_ansi(
        _______, _______, _______, _______,  _______, _______, _______, _______, _______, _______, _______, RGB_RMOD, RGB_MOD, _______,          _______,
        _______, _______, _______, _______,  _______, _______, _______, _______, _______, _______, _______,  RGB_SPD, RGB_SPI, _______,          _______,
        _______, _______, _______, _______,  _______, _______, _______, _______, _______, _______, _______,  _______,          _______,          _______,
        _______,          _______, _______,  _______, _______, _______, _______, _______, _______, _______,  _______,          _______, RGB_SAI, _______,
        _______, _______, _______,                             _______,                            _______,  _______, _______, RGB_HUD, RGB_SAD, RGB_HUI
    ),
#ifdef MOUSE_ENABLE
    [MOUSE_LAYER] = LAYOUT_65_ansi(
        _______, _______, _______, _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, _______, _______, _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, _______, _______, _______,  _______, _______, KC_MS_L, KC_MS_D, KC_MS_U, KC_MS_R, _______, _______,          _______,          _______,
        _______,          _______, _______,  _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______,
        _______, _______, MO(MOUSE_LAYER_2),                   _______,                            _______, _______, _______, _______, _______, _______
    ),
    [MOUSE_LAYER_2] = LAYOUT_65_ansi(
        _______, _______, _______, _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, _______, _______, _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, _______, _______, _______,  _______, _______, _______, KC_WH_D, KC_WH_U, _______, _______, _______,          _______,          _______,
        _______,          _______, _______,  _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______,
        _______, _______, _______,                             _______,                            _______, _______, _______, _______, _______, _______
    )
#endif
};

void keyboard_post_init_user(void) {
    // Customise these values to desired behaviour
    // debug_enable=true;
    // debug_matrix=true;
    // debug_keyboard=true;
    // debug_mouse=true;
    rgb_matrix_mode(RGB_MATRIX_SOLID_COLOR);
    rgb_matrix_sethsv(181, 255, 255);
    hsv_y = rgb_matrix_get_hsv();
}

/*
 *{
 *    {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, NA, 14 },
 *    { 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, NA, 29 },
 *    { 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, NA, 42, NA, 43 },
 *    { 44, NA, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, NA, 55, 56, 57 },
 *    { 58, 59, 60, NA, NA, NA, 61, NA, NA, NA, 62, 63, 64, 65, 66, 67 }
 *},
 */
void rgb_matrix_indicators_advanced_user (uint8_t led_min, uint8_t led_max) {
    HSV hsv;// hsv_t;
    switch(get_highest_layer(layer_state)) {
        case WIN_FN1:
            rgb_matrix_sethsv(HSV_BLACK);
            for (int i = 0; i < 15; i++) {
                if (i == 13) continue;
                rgb_matrix_set_color(i, RGB_ORANGE);
            }
            rgb_matrix_set_color(25, RGB_ORANGE);
            rgb_matrix_set_color(26, RGB_ORANGE);
            rgb_matrix_set_color(27, RGB_ORANGE);
            rgb_matrix_set_color(62, RGB_ORANGE);
            break;
        case FN2:
            rgb_matrix_sethsv(HSV_BLACK);
            for (int i = 0; i < 13; i++) {
                rgb_matrix_set_color(i, RGB_GREEN);
            }
            rgb_matrix_set_color(13, RGB_RED);
            rgb_matrix_set_color(43, RGB_GREEN);
            break;
        case RGB_CHANGE:
            rgb_matrix_set_color(11, RGB_CORAL);
            rgb_matrix_set_color(12, RGB_CORAL);
            rgb_matrix_set_color(26, RGB_CORAL);
            rgb_matrix_set_color(27, RGB_CORAL);
            rgb_matrix_set_color(56, RGB_CORAL);
            rgb_matrix_set_color(65, RGB_CORAL);
            rgb_matrix_set_color(66, RGB_CORAL);
            rgb_matrix_set_color(67, RGB_CORAL);
            break;
        case RGB_BASE:
            rgb_matrix_sethsv(HSV_BLACK);
            for (int i = 16; i < 24; i++) {
                rgb_matrix_set_color(i, RGB_YELLOW);
            }
            break;
#ifdef MOUSE_ENABLE
        case MOUSE_LAYER:
            rgb_matrix_sethsv(HSV_BLACK);
            rgb_matrix_set_color(36, RGB_PINK);
            rgb_matrix_set_color(37, RGB_PINK);
            rgb_matrix_set_color(38, RGB_PINK);
            rgb_matrix_set_color(39, RGB_PINK);
            break;
        case MOUSE_LAYER_2:
            rgb_matrix_sethsv(HSV_BLACK);
            rgb_matrix_set_color(36, RGB_TURQUOISE);
            rgb_matrix_set_color(37, RGB_TURQUOISE);
            rgb_matrix_set_color(38, RGB_TURQUOISE);
            rgb_matrix_set_color(39, RGB_TURQUOISE);
            break;
#endif
        default: //  for any other layers, or the default layer
            hsv = rgb_matrix_get_hsv();
#ifdef CONSOLE_ENABLE
            uprintf("hsv.h %d, hsv.s %d, hsv.v %d\n", hsv.h, hsv.s, hsv.v);
            uprintf("hsv_t.h %d, hsv_t.s %d, hsv_t.v %d", hsv_y.h, hsv_y.s, hsv_y.v);
#endif
            if (hsv.h == 0 && hsv.s == 0 && hsv.v == 0)
                hsv = hsv_y;
            rgb_matrix_sethsv(hsv.h, hsv.s, hsv.v);
#ifdef USE_VIM
            if (vim_mode_enabled()) {
                rgb_matrix_set_color(30, RGB_RED);
            }
#endif
            break;
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
#ifdef USE_VIM
    // Process vim mode
    if (!process_vim_mode(keycode, record)) {
        return false;
    }
    if (keycode == QMK_VIM && record->event.pressed) {
        toggle_vim_mode();
        return false;
    }
    if (record->event.pressed && keycode == LCTL(KC_S)) {
        normal_mode();
        tap_code16(keycode);
        return false;
    }
#endif
    return true;
}
