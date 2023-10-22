// Copyright 2022 Alexander Lozyuk (@keyzog)
// SPDX-License-Identifier: GPL-2.0-or-later
#include QMK_KEYBOARD_H
#include "transactions.h"
#include "encoder.c"

typedef union {
    uint32_t raw;
    struct {
        uint8_t lbg_pos : 4;
        uint8_t bgl_act : 2;
        uint8_t bgl_hue : 8;
        uint8_t bgl_sat : 8;
        uint8_t bgl_val : 8;
    };
} user_config_t;

user_config_t user_config;
uint8_t lbg_pos = 4;
uint8_t bgl_act = 0;
uint8_t bgl_hue = 222;
uint8_t bgl_sat = 255;
uint8_t bgl_val = 10;
bool b_sync_need_send = false;

#define ____    KC_TRNS
#define SPC_FN  LT(1,KC_SPC)

// Defines the keycodes used by our macros in process_record_user
enum custom_keycodes {
    LBI_TOG = SAFE_RANGE,
    BG_TOG,
    BG_HUI,
    BG_HUD,
    BG_SAI,
    BG_SAD,
    BG_VAI,
    BG_VAD
};



const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT( // qwerty
  //┌────────┬────────┬────────┬────────┬────────┬────────┬────────┐           ┌────────┬────────┬────────┬────────┬────────┬────────┬────────┐
     KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_PGUP,             KC_HOME, KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤           ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_PGDN,             KC_END,  KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_EQL,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤           ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     KC_GRV,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_LBRC,             KC_RBRC, KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤           ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_EXLM,             KC_QUES, KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
  //├────────┼────────┼────────┼────────┼────────┼─┬──────┴─┬──────┴─┐       ┌─┴──────┬─┴──────┬─┼────────┼────────┼────────┼────────┼────────┤
     KC_LCTL, KC_LGUI, KC_LALT, MO(2),   SPC_FN,    KC_BSPC, KC_ENT,          KC_DEL,  KC_SPC,    MO(1),   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT
  //└────────┴────────┴────────┴────────┴────────┘ └────────┴────────┘       └────────┴────────┘ └────────┴────────┴────────┴────────┴────────┘
  ),

  [1] = LAYOUT( // russian
  //┌────────┬────────┬────────┬────────┬────────┬────────┬────────┐           ┌────────┬────────┬────────┬────────┬────────┬────────┬────────┐
     KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_PGUP,             KC_HOME, KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤           ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_PGDN,             KC_END,  KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_EQL,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤           ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     KC_GRV,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_LBRC,             KC_RBRC, KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤           ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_EXLM,             KC_QUES, KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
  //├────────┼────────┼────────┼────────┼────────┼─┬──────┴─┬──────┴─┐       ┌─┴──────┬─┴──────┬─┼────────┼────────┼────────┼────────┼────────┤
     KC_LCTL, KC_LGUI, KC_LALT, MO(2),   SPC_FN,    KC_BSPC, KC_ENT,          KC_DEL,  KC_SPC,    MO(1),   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT
  //└────────┴────────┴────────┴────────┴────────┘ └────────┴────────┘       └────────┴────────┘ └────────┴────────┴────────┴────────┴────────┘
  ),

  [2] = LAYOUT( // colemak
  //┌────────┬────────┬────────┬────────┬────────┬────────┬────────┐           ┌────────┬────────┬────────┬────────┬────────┬────────┬────────┐
     KC_TRNS, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_TRNS,             KC_PSCR, KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤           ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     KC_TRNS, KC_EXLM, KC_AT,   KC_LCBR, KC_RCBR, KC_PIPE, KC_TRNS,             KC_TRNS, KC_PSLS, KC_P7,   KC_P8,   KC_P9,   KC_PMNS, KC_F12,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤           ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     KC_TRNS, KC_HASH, KC_DLR,  KC_LBRC, KC_RBRC, KC_GRV,  KC_TRNS,             KC_TRNS, KC_PAST, KC_P4,   KC_P5,   KC_P6,   KC_PPLS, KC_F13,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤           ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     KC_TRNS, KC_PERC, KC_CIRC, KC_LPRN, KC_RPRN, KC_TILD, KC_TRNS,             KC_TRNS, KC_NO,   KC_P1,   KC_P2,   KC_P3,   KC_PENT, KC_F14,
  //├────────┼────────┼────────┼────────┼────────┼─┬──────┴─┬──────┴─┐       ┌─┴──────┬─┴──────┬─┼────────┼────────┼────────┼────────┼────────┤
     KC_TRNS, KC_TRNS, KC_TRNS, KC_NO,   KC_NO,     KC_TRNS, KC_TRNS,         KC_TRNS, KC_TRNS,   KC_NO,   KC_P0,   KC_PDOT, KC_PENT, KC_F15
  //└────────┴────────┴────────┴────────┴────────┘ └────────┴────────┘       └────────┴────────┘ └────────┴────────┴────────┴────────┴────────┘
  ),

  [3] = LAYOUT( // game
  //┌────────┬────────┬────────┬────────┬────────┬────────┬────────┐           ┌────────┬────────┬────────┬────────┬────────┬────────┬────────┐
     KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,               KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   QK_RBT,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤           ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     KC_NO,   RGB_SAI, RGB_SPI, RGB_VAI, RGB_HUI, RGB_MOD, KC_NO,               KC_NO,   KC_VOLU, KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤           ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     KC_NO,   RGB_SAD, RGB_SPD, RGB_VAD, RGB_HUD, RGB_RMOD,KC_NO,               KC_NO,   KC_VOLD, KC_MPLY, KC_MNXT, KC_MPRV, KC_NO,   KC_NO,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤           ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     KC_NO,   KC_NO,   KC_NO,   KC_NO,   RGB_TOG, RGB_M_P, KC_NO,               KC_NO,   KC_MUTE, KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
  //├────────┼────────┼────────┼────────┼────────┼─┬──────┴─┬──────┴─┐       ┌─┴──────┬─┴──────┬─┼────────┼────────┼────────┼────────┼────────┤
     KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,     KC_NO,   KC_NO,           KC_NO,   KC_NO,     KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO
  //└────────┴────────┴────────┴────────┴────────┘ └────────┴────────┘       └────────┴────────┘ └────────┴────────┴────────┴────────┴────────┘
  ),

  [4] = LAYOUT( // fn и препинаки
  //┌────────┬────────┬────────┬────────┬────────┬────────┬────────┐           ┌────────┬────────┬────────┬────────┬────────┬────────┬────────┐
     ____,    ____,    ____,    ____,    ____,    ____,    ____,                ____,    ____,    ____,    ____,    ____,    ____,    ____,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤           ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     ____,    ____,    ____,    ____,    ____,    ____,    ____,                ____,    ____,    ____,    ____,    ____,    ____,    ____,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤           ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     ____,    ____,    ____,    ____,    ____,    ____,    ____,                ____,    ____,    ____,    ____,    ____,    ____,    ____,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤           ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     ____,    ____,    ____,    ____,    ____,    ____,    ____,                ____,    ____,    ____,    ____,    ____,    ____,    ____,
  //├────────┼────────┼────────┼────────┼────────┼─┬──────┴─┬──────┴─┐       ┌─┴──────┬─┴──────┬─┼────────┼────────┼────────┼────────┼────────┤
     ____,    ____,    ____,    ____,    ____,      ____,    ____,            ____,    ____,      ____,    ____,    ____,    ____,    ____
  //└────────┴────────┴────────┴────────┴────────┘ └────────┴────────┘       └────────┴────────┘ └────────┴────────┴────────┴────────┴────────┘
   ),

  [5] = LAYOUT( // rgb layers
  //┌────────┬────────┬────────┬────────┬────────┬────────┬────────┐           ┌────────┬────────┬────────┬────────┬────────┬────────┬────────┐
     ____,    ____,    ____,    ____,    ____,    ____,    ____,                ____,    BG_HUI,  BG_SAI,  BG_VAI,  ____,    ____,    ____,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤           ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     KC_NO,   RGB_SAI, RGB_SPI, RGB_VAI, RGB_HUI, RGB_MOD, KC_NO,               ____,    BG_HUD,  BG_SAD,  BG_VAD,  ____,    ____,    ____,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤           ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     KC_NO,   RGB_SAD, RGB_SPD, RGB_VAD, RGB_HUD, RGB_RMOD,KC_NO,               ____,    ____,    ____,    ____,    ____,    ____,    ____,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤           ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     KC_NO,   KC_NO,   KC_NO,   KC_NO,   RGB_TOG, RGB_M_P, KC_NO,               ____,    BG_TOG,  LBI_TOG, ____,    ____,    ____,    ____,
  //├────────┼────────┼────────┼────────┼────────┼─┬──────┴─┬──────┴─┐       ┌─┴──────┬─┴──────┬─┼────────┼────────┼────────┼────────┼────────┤
     ____,    ____,    ____,    ____,    ____,      ____,    ____,            ____,    ____,      ____,    ____,    ____,    ____,    ____
  //└────────┴────────┴────────┴────────┴────────┘ └────────┴────────┘       └────────┴────────┘ └────────┴────────┴────────┴────────┴────────┘
   )
};



void user_sync_a_slave_handler(uint8_t in_buflen, const void* in_data, uint8_t out_buflen, void* out_data) {
    const user_config_t *m2s = (const user_config_t*)in_data;
    lbg_pos = m2s->lbg_pos;
    bgl_act = m2s->bgl_act;
    bgl_hue = m2s->bgl_hue;
    bgl_sat = m2s->bgl_sat;
    bgl_val = m2s->bgl_val;
}

void keyboard_post_init_user(void) {
    user_config.raw = eeconfig_read_user();
    lbg_pos = user_config.lbg_pos;
    bgl_act = user_config.bgl_act;
    bgl_hue = user_config.bgl_hue;
    bgl_sat = user_config.bgl_sat;
    bgl_val = user_config.bgl_val;
    transaction_register_rpc(USER_SYNC_A, user_sync_a_slave_handler);
    b_sync_need_send = true;
}

void eeconfig_init_user(void) {  // EEPROM is getting reset!
    user_config.raw = 0;
    user_config.lbg_pos = lbg_pos;
    user_config.bgl_act = bgl_act;
    user_config.bgl_hue = bgl_hue;
    user_config.bgl_sat = bgl_sat;
    user_config.bgl_val = bgl_val;
    eeconfig_update_user(user_config.raw); // Write default value to EEPROM now
}


void housekeeping_task_user(void) {
    if (!is_keyboard_master() || !b_sync_need_send) return;
    if (transaction_rpc_send(USER_SYNC_A, sizeof(user_config), &user_config)) {
        b_sync_need_send = false;
    }
}


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case LBI_TOG:
      if (record->event.pressed) {
        lbg_pos = lbg_pos + 1 > 6 ? 0 : lbg_pos + 1;
        user_config.lbg_pos = lbg_pos;
        eeconfig_update_user(user_config.raw);
        b_sync_need_send = true;
      }
      break;
    case BG_TOG:
      if (record->event.pressed) {
        bgl_act = bgl_act + 1 > 2 ? 0 : bgl_act + 1;
        user_config.bgl_act = bgl_act;
        eeconfig_update_user(user_config.raw);
        b_sync_need_send = true;
      }
      break;
    case BG_HUI:
      if (record->event.pressed) {
        bgl_hue = bgl_hue + 10 > 255 ? 0 : bgl_hue + 10;
        user_config.bgl_hue = bgl_hue;
        eeconfig_update_user(user_config.raw);
        b_sync_need_send = true;
      }
      break;
    case BG_HUD:
      if (record->event.pressed) {
        bgl_hue = bgl_hue - 10 < 0 ? 255 : bgl_hue - 10;
        user_config.bgl_hue = bgl_hue;
        eeconfig_update_user(user_config.raw);
        b_sync_need_send = true;
      }
      break;
    case BG_SAI:
      if (record->event.pressed) {
        bgl_sat = bgl_sat + 50 > 255 ? 255 : bgl_sat + 50;
        user_config.bgl_sat = bgl_sat;
        eeconfig_update_user(user_config.raw);
        b_sync_need_send = true;
      }
      break;
    case BG_SAD:
      if (record->event.pressed) {
        bgl_sat = bgl_sat - 50 < 0 ? 0 : bgl_sat - 50;
        user_config.bgl_sat = bgl_sat;
        eeconfig_update_user(user_config.raw);
        b_sync_need_send = true;
      }
      break;
    case BG_VAI:
      if (record->event.pressed) {
        bgl_val = bgl_val + 2 > 10 ? 10 : bgl_val + 2;
        user_config.bgl_val = bgl_val;
        eeconfig_update_user(user_config.raw);
        b_sync_need_send = true;
      }
      break;
    case BG_VAD:
      if (record->event.pressed) {
        bgl_val = bgl_val - 2 < 0 ? 0 : bgl_val - 2;
        user_config.bgl_val = bgl_val;
        eeconfig_update_user(user_config.raw);
        b_sync_need_send = true;
      }
      break;
  }
  return true;
}


layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
    case 1:
        tap_code16(RCS(KC_2));
        break;
    default:
        tap_code16(RCS(KC_1));
        break;
    }
    return state;
}


#ifdef RGB_MATRIX_ENABLE
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    // ОТДЕЛЬНОЕ УПРАВЛЕНИЕ НИЖНЕЙ ПОДСВЕТКОЙ
    if(bgl_act != 0) {
        HSV hsv = {
            bgl_hue,
            bgl_sat,
            rgb_matrix_config.hsv.v / 10 * bgl_val
        };
        if(bgl_act == 2) hsv.v = 0;

        RGB rgb = hsv_to_rgb(hsv);
        for (uint8_t i = 0; i < 9; i++) {
            rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
            rgb_matrix_set_color(i+44, rgb.r, rgb.g, rgb.b);
        }
    }

    // ИНДИКАЦИЯ СЛОЕВ
    if(lbg_pos != 0) {
        uint8_t state[6][9] = {
            {1, 2, 3, 4, 5, 6, 7, 8, 9},
            {1, 2, 3, 9},
            {43, 44},
            {1, 2, 3, 9, 43, 44},
            {10, 11, 12, 13, 14, 15, 16},
            {16, 17, 30, 31}
        };
        uint8_t size = (sizeof(state[lbg_pos-1]) / sizeof(uint8_t));
        HSV hsv = {0, 255, rgb_matrix_config.hsv.v / 10 * bgl_val};
        RGB rgb;

        for (uint8_t i = 0; i < size; i++) {
            uint8_t led_num = state[lbg_pos-1][i] - 1;
            if(led_num != -1) {
                hsv.h = 47 * get_highest_layer(layer_state | default_layer_state);
                rgb = hsv_to_rgb(hsv);
                rgb_matrix_set_color(led_num, rgb.r, rgb.g, rgb.b);
                rgb_matrix_set_color(led_num + 44, rgb.r, rgb.g, rgb.b);
            }
        }
    }
    
    return false;
}
#endif
