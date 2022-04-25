// Copyright 2022 Alexander Lozyuk (@keyzog)
// SPDX-License-Identifier: GPL-2.0-or-later
#ifdef ENCODER_ENABLE

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (get_highest_layer(layer_state|default_layer_state) > 0) {
        if (index == 0) {
            // if (clockwise) {
            //     tap_code(KC_WH_D);
            // } else {
            //     tap_code(KC_WH_U);
            // }
        } else if (index == 1) {
            if (clockwise) {
                rgb_matrix_increase_val_noeeprom();
            } else {
                rgb_matrix_decrease_val_noeeprom();
            }
        }
    } else {  /* Layer 0 */
        if (index == 0) {
            // if (clockwise) {
            //     tap_code(KC_PGDN);
            // } else {
            //     tap_code(KC_PGUP);
            // }
        } else if (index == 1) {
            if (clockwise) {
                tap_code(KC_VOLU);
            } else {
                tap_code(KC_VOLD);
            }
        }
    }
    return false;
}

#endif
