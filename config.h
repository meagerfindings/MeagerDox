/*
Copyright 2012 Jun Wako <wakojun@gmail.com>
Copyright 2013 Oleg Kostyuk <cub.uanic@gmail.com>

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

#ifndef ERGODOX_EZ_CONFIG_H
#define ERGODOX_EZ_CONFIG_H

#include "../config.h"

#include "config_common.h"

/* USB Device descriptor parameter */
#define VENDOR_ID       0xFEED
#define PRODUCT_ID      0x1307
#define DEVICE_VER      0x0001
#define MANUFACTURER    meagerfindings
#define PRODUCT         ErgoDox
#define DESCRIPTION     QMK keyboard firmware for Ergodox

/* key matrix size */
#define MATRIX_ROWS 14
#define MATRIX_COLS 6

/* number of backlight levels */
#define BACKLIGHT_LEVELS 3

#define LED_BRIGHTNESS_LO       15
#define LED_BRIGHTNESS_HI       255

/* ws2812 RGB LED */
#define RGB_DI_PIN D7
#define RGBLIGHT_ANIMATIONS
#define RGBLED_NUM 15     // Number of LEDs
#define RGBLIGHT_HUE_STEP 12
#define RGBLIGHT_SAT_STEP 255
#define RGBLIGHT_VAL_STEP 12

// #define RGB_MIDI
#define RGBW_BB_TWI

#define RGBW 1

/* Set 0 if debouncing isn't needed */
#define DEBOUNCE    5

#define USB_MAX_POWER_CONSUMPTION 500

#define TAPPING_TERM 200

#define MOUSEKEY_INTERVAL       20
#define MOUSEKEY_DELAY          0
#define MOUSEKEY_TIME_TO_MAX    20
#define MOUSEKEY_MAX_SPEED      4
#define MOUSEKEY_WHEEL_DELAY 0

/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

/* disable debug print */
// #define NO_DEBUG

/* disable print */
// #define NO_PRINT

/* disable action features */
//#define NO_ACTION_LAYER
//#define NO_ACTION_TAPPING
//#define NO_ACTION_ONESHOT
//#define NO_ACTION_MACRO
//#define NO_ACTION_FUNCTION
//#define DEBUG_MATRIX_SCAN_RATE

#endif
