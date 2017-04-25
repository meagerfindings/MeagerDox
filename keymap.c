#include "ergodox.h"
#include "debug.h"
#include "action_layer.h"
#include "version.h"

enum custom_keycodes {
  PLACEHOLDER = SAFE_RANGE, // can always be here
  EPRM,
  VRSN,
  TD_COPY_CUT = 6,
  TD_SNAGIT = 8,
  TD_EXCl_AT = 9,
  TD_B_L_SEL = 10,
  CT_LBP = 11,
  CT_RBP = 12,
  TD_OSL3 = 13,
};

//Redifne Key Names for readaibilty
#define _______ KC_TRANSPARENT
#define XXXXXXX KC_NO
#define SCRN_CLIPB LCTL(LGUI(LSFT(KC_4)))
#define CHRM_L LALT(LGUI(KC_LEFT)) //Move left one tab in Chrome
#define CHRM_R LALT(LGUI(KC_RIGHT)) //Move right one tab in Chrome

//Macro Names
#define DBL_0 M(2) //Types two numpad zeroes
#define L_ID_0 M(3) //Prints layer identifier text for layer 0
#define L_ID_1 M(4) //Prints layer identifier text for layer 1
#define L_ID_2 M(5) //Prints layer identifier text for layer 2
#define L_ID_3 M(6) //Prints layer identifier text for layer 3
#define WBR M(7) //Typinator signature abbreviation
#define DNS M(8) //Typinator abbreviation to set PiHole DNS when tapped, when held, Typinator abbreviation to set Google DNS
#define CLR_DNS M(9) //Typinator abbreviation to clear OS X dns when tapped, when held, Typinator abbreviation for ssh into rPI
#define XKCD_AUTO M(10) //Sends string "https://xkcd.com/1319/"
#define SCRN_VIDEO M(11) //When tapped:Typinator screenshot abbreviation. Result = "(Screenshot: <clilpboard>)" When held: //Typinator screenshot abbreviation. Result = "(Screenshot: <clilpboard>)"
#define PAST_PS M(12) // Tap = CMD + V , Held = CMD + Shift + V
#define SLACK M(13) //Opens Slack on a mac
#define TODO M(14) //Macro to initiate new Todoist task window on tap and switch to Todoist when held
#define ATOM M(15) //Opens Atom
#define ZENDESK M(16) //Macro opens Chrome and navigates to my Zendesk Agent dashboard. Calls spotlight and then triggers Typinator expansion.
#define PUSHBUL M(17)
#define S_SAVE M(18) //Macro to send S on tap and Save on Hold
#define A_ALL M(19) //Macro to send A on tap and CMD + A on Hold
#define TYFW M(20) //Macro for Typinator expansion of TYFW

static uint16_t key_timer; //key timer for macros

void td_osl3_tol3(qk_tap_dance_state_t *state, void *user_data) {
  switch (state->count) {
  case 1:
     set_oneshot_layer(3, ONESHOT_START);
     clear_oneshot_layer_state (ONESHOT_PRESSED);
     break;
  case 2:
     layer_on(3);
     break;
  case 3:
   layer_on(0);
   break;
  }
}

void td_base_layer_selector(qk_tap_dance_state_t *state, void *user_data) {
  switch (state->count) {
  case 1:
     layer_on(1);
     break;
  case 2:
     layer_on(2);
     break;
  case 3:
     layer_on(3);
     break;
  case 4:
     layer_on(4);
     break;
  case 5:
     layer_on(0);
     break;
  }
}


//Tap Dance Definitions
qk_tap_dance_action_t tap_dance_actions[] = {
  //Tap once for Copy, twice for Paste, three times for Cut.
  [TD_COPY_CUT]  = ACTION_TAP_DANCE_DOUBLE(LGUI(KC_C),LGUI(KC_X)),
    //Tap once for Snagit, twice for Cmd + Shift + Opt + 4 (OS X cropping screenshot that is copied to the clipboard only.)
  [TD_SNAGIT] = ACTION_TAP_DANCE_DOUBLE(LCTL(LSFT(KC_C)), LCTL(LGUI(LSFT(KC_4)))),
    //Need to make this one execute macros to do this instead?
  [TD_EXCl_AT] = ACTION_TAP_DANCE_DOUBLE(KC_EXLM, KC_AT),
  [TD_B_L_SEL] = ACTION_TAP_DANCE_FN(td_base_layer_selector),
  [CT_LBP] = ACTION_TAP_DANCE_DOUBLE (KC_LPRN, KC_LBRC),
  [CT_RBP] = ACTION_TAP_DANCE_DOUBLE (KC_RPRN, KC_RBRC),
  [TD_OSL3] = ACTION_TAP_DANCE_FN (td_osl3_tol3),
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  /* Keymap 0: Basic layer
  * ,--------------------------------------------------.           ,--------------------------------------------------.
  * |   =    |   1  |   2  |   3  |   4  |   5  | Paste|           |Snagit|   6  |   7  |   8  |   9  |   0  |   -    |
  * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
  * | Tab    |   Q  |   W  |   E  |   R  |   T  | Copy |           | SCRN/|   Y  |   U  |   I  |   O  |   P  |   \    |
  * |--------+------+------+------+------+------| /Cut |           | VIDEO|------+------+------+------+------+--------|
  * | Hyper  |   A  |S /CMD|   D  |   F  |   G  |------|           |------|   H  |   J  |   K  |L /CMD| ; /L2|' /Hyper|
  * |--------+------+------+------+------+------|  (   |           |  )   |------+------+------+------+------+--------|
  * |Layer 1 |Z/Ctrl|X/Alt |   C  |   V  |   B  | [ {  |           | ] }  |   N  |   M  |   ,  | ./Alt|//Ctrl|Layer 2 |
  * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
  *   |  `   |Zendsk| ATOM |CMD+S | ToDo |                                       |      |      |      |      |OS3/L3|
  *   `----------------------------------'                                       `----------------------------------'
  *                                        ,-------------.       ,--------------.
  *                                        | Esc  | Home |       |Layer?| Esc   |
  *                                 ,------|------|------|       |------+-------+------.
  *                                 |      |      | End  |       | Undo |       |      |
  *                                 |Shift |Backsp|------|       |------| Return|Space/|
  *                                 | OSM  | /L5  | DEL  |       | CMD  | /LT1  |Arrows|
  *                                 `--------------------'       `---------------------'
  *
  *Copy/ cut key copies on tap, cut's on two taps.
  *
  *One tap on Snagit key = is Ctrl + Shift + C which is Snagit's selector.
  *Two taps on Snagit key = Cmd + Shift + Opt + 4 (OS X cropping screenshot that is copied to the clipboard only.)
  */


  [0] = KEYMAP(
    //left hand
    KC_EQUAL,     KC_1,     KC_2,    KC_3,     KC_4,     KC_5,          PAST_PS,
    KC_TAB,       KC_Q,     KC_W,    KC_E,     KC_R,     KC_T,          TD(TD_COPY_CUT),
    OSM(MOD_HYPR),          KC_A,  LGUI_T(KC_S),  LGUI_T(LALT_T(KC_D)),     KC_F,          KC_G,
    TO(1), CTL_T(KC_Z), ALT_T(KC_X),  KC_C,     KC_V,     KC_B,          TD(CT_LBP),
               KC_GRAVE,    ZENDESK,  ATOM, LGUI(KC_S),    TODO,
                                                          KC_ESCAPE,     KC_HOME,
                                                                          KC_END,
                                      OSM(MOD_LSFT), LT(5,KC_BSPACE),  KC_DELETE,

    //right hand
    TD(TD_SNAGIT),     KC_6,     KC_7,     KC_8,     KC_9,          KC_0,             KC_MINUS,
    SCRN_VIDEO,        KC_Y,     KC_U,     KC_I,     KC_O,          KC_P,             KC_BSLASH,
                       KC_H,     KC_J,     KC_K,     LGUI_T(KC_L),  LT(2,KC_SCOLON),  ALL_T(KC_QUOTE),
    TD(CT_RBP),        KC_N,     KC_M,     KC_COMMA, ALT_T(KC_DOT), CTL_T(KC_SLASH),  TO(2),
                    _______,  _______,      _______, _______,       TD(TD_OSL3),
    L_ID_0,         KC_ESCAPE,
    LGUI(KC_Z),
    OSM(MOD_LGUI),   LT(1,KC_ENTER),  LT(4,KC_SPACE)),

    /* Keymap 1: Symbol
     * ,--------------------------------------------------.           ,--------------------------------------------------.
     * |        |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |           |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 |        |
     * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
     * |        |      |      |   &  |   `  |   |  | Cmd  |           |      |      |      |      |      |      |        |
     * |--------+------+------+------+------+------| + K  |           |      |------+------+------+------+------+--------|
     * |Shft Tab|   #  |   $  |   !  |   '  |   @  |------|           |------|   *  |      |      |      |      |        |
     * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
     * |Layer 0 |   %  |   ^  |   [  |   ]  |   ~  |      |           |      |      |      |      |      |      |Layer 2 |
     * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
     *   |      |      |      |      |      |                                       |      |      |      |      |      |
     *   `----------------------------------'                                       `----------------------------------'
     *                                        ,-------------.       ,-------------.
     *                                        |      |      |       |Layer?|      |
     *                                 ,------|------|------|       |------+------+------.
     *                                 |      |      |      |       |      |      |      |
     *                                 |      |      |------|       |------|      |      |
     *                                 |      |      |      |       |      |      |      |
     *                                 `--------------------'       `--------------------'
     */
    // SYMBOLS & Numpad
  [1] = KEYMAP(
    //left hand
    _______,          KC_F1,             KC_F2,             KC_F3,          KC_F4,          KC_F5,      KC_F6,
    _______,        _______,           _______,           KC_AMPR,        KC_GRAVE,       KC_PIPE,    LGUI(KC_K),
    LSFT(KC_TAB),   KC_HASH,           KC_DLR,            KC_EXLM,        KC_QUOTE,       KC_AT,
    TO(0),          KC_PERC,           KC_CIRC,           KC_LBRC,        KC_RBRC,        KC_TILD,    _______,
                       _______,          _______,           _______,        _______,        _______,
                                                                                            _______,   _______,
                                                                                                       _______,
                                                                            _______,        _______,   _______,

    //right hand
      KC_F7,        KC_F8,       KC_F9,       KC_F10,       KC_F11,      KC_F12,    KC_MINUS,
    _______,      _______,     _______,      _______,      _______,     _______,    _______,
                  KC_ASTR,     _______,      _______,      _______,     _______,    _______,
    _______,      _______,     _______,      _______,      _______,     _______,    TO(2),
                  _______,     _______,      _______,      _______,     _______,
    L_ID_1,       _______,
    _______,
    _______,      _______,      _______),

  /* Keymap 2: Media and mouse keys
   * ,--------------------------------------------------.           ,--------------------------------------------------.
   * |        |      |      |      |      |      |      |           | XKCD |      |      |      |      |      |  F15   |
   * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
   * |        |      |Whl Up| MsUp |Whl Dn|      |      |           |      |      |      |      |      |      |  F14   |
   * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
   * |        |Ctl L |MsLeft|MsDown|MsRght| Ctl R|------|           |------|      | Mute | VolDn| VolUp| TL 2 |  Play  |
   * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
   * |Layer 0 |      |WHL L |      |Whl R |      |      |           |      |      |      | Prev | Next |      | Layer 0|
   * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
   *   |      |      |      |      |      |                                       | Mute |      |      |      |      |
   *   `----------------------------------'                                       `----------------------------------'
   *                                        ,-------------.       ,-------------.
   *                                        |      |      |       |LAYER?|      |
   *                                 ,------|------|------|       |------+------+------.
   *                                 | Left |Right |      |       |      |Brwser|Brwser|
   *                                 | Click|Click |------|       |------| Fwd  |Back  |
   *                                 |      |      |      |       |      |      |      |
   *                                 `--------------------'       `--------------------'
   */
  // MEDIA AND MOUSE

  [2] = KEYMAP(
    //left hand
    _______,    _______,         _______,         _______,         _______,         _______,         _______,
    _______,    _______,         KC_MS_WH_UP,     KC_MS_UP,        KC_MS_WH_DOWN,   _______,         _______,
    _______,    LCTL(KC_LEFT),   KC_MS_LEFT,      KC_MS_DOWN,      KC_MS_RIGHT,     RCTL(KC_RIGHT),
    TO(1),    _______,         KC_MS_WH_LEFT,   _______,         KC_MS_WH_RIGHT,  _______,         _______,
                _______,         _______,         _______,         _______,         _______,
                                                                                    _______,         _______,
                                                                                                     _______,
                                                                   KC_MS_BTN1,      KC_MS_BTN2,      _______,

    //right hand
    XKCD_AUTO,  _______,       _______,               _______,               _______,    _______,    KC_F15,
    _______,    _______,       _______,               _______,               _______,    _______,    KC_F14,
                _______, KC_AUDIO_MUTE,     KC_AUDIO_VOL_DOWN,       KC_AUDIO_VOL_UP,    _______,    KC_MEDIA_PLAY_PAUSE,
   _______,     _______,       _______,   KC_MEDIA_PREV_TRACK,   KC_MEDIA_NEXT_TRACK,    _______,    TO(0),
                               _______,               _______,               _______,    _______,    _______,
    L_ID_2,           _______,
    _______,
    _______,    TD(TD_COPY_CUT),    PAST_PS),

    /* Keymap 3: Anki Layer for adding cloze deletions, secondary cloze deletions and new cards
     * ,--------------------------------------------------.           ,--------------------------------------------------.
     * |        |      |      |      |      |      |Sc cp |           |      |      |      |      |      |      |Spdcube |
     * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
     * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
     * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
     * |        |      |      |      |      |      |------|           |------|      | Wrong| Hard | Good | Easy |        |
     * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
     * |Layer 0 |      |      |      |      |      |      |           |      |      |      |      |      |      |Layer 0 |
     * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
     *   |      |      |      |      |      |                                       |         |      |   |      |Layer0|
     *   `----------------------------------'                                       `----------------------------------'
     *                                        ,-------------.       ,-------------.
     *                                        |      |      |       |Layer?|      |
     *                                 ,------|------|------|       |------+------+------.
     *                                 |Add   |Add   |      |       |      |Add   |      |
     *                                 |Cloze |Cloze2|------|       |------|Card  |Space/|
     *                                 |      |      |      |       |      |      | Good |
     *                                 `--------------------'       `--------------------'
     * This layer is specifically for use with the notecard program Anki: ankiweb.net/about
     * These keys allow adding cloze deletions, secondary cloze deletions and new cards.
     * Sc cp = OS X cropping screenshot that is copied to the clipboard only.
     */
    // Anki  Layer
  [3] = KEYMAP(
    //left hand
    _______,       _______,       _______,       _______,       _______,       _______,       _______,
    _______,       _______,       _______,       _______,       _______,       _______,       _______,
    _______,       _______,       _______,       _______,       _______,       _______,
    TO(0),         _______,       _______,       _______,       _______,       _______,       _______,
                   _______,       _______,       _______,       _______,       _______,
                                                                               _______,       _______,
                                                                                              _______,
                                                LGUI(LSFT(KC_C)),     LALT(LGUI(LSFT(KC_C))), _______,

   //right hand
   SCRN_CLIPB,             _______,       _______,       _______,       _______,       _______,       TO(6),
   _______,                _______,       _______,       _______,       _______,       _______,       _______,
                           _______,       KC_1,          KC_2,          KC_3,          KC_4,          _______,
   _______,                _______,       _______,       _______,       _______,       _______,       TO(0),
                           _______,       _______,       _______,       _______,       TO(0),
   L_ID_3,                _______,
   _______,
   _______,               LGUI(KC_ENTER),       _______),

   /* Keymap 4: Arrows
    *
    * ,--------------------------------------------------.           ,--------------------------------------------------.
    * |  RESET |      |      |      |      |  DNS |clrDNS|           |      |      |      |      |      |      |        |
    * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
    * |        |      | Opt+L|  Up  |Opt+R |      |      |           |      |      |      |  Up  |      |      |        |
    * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
    * |        |Ctrl+L| Left |  Dn  | Right|Ctrl+R|------|           |------|      | Left |  Dn  | Right|      |        |
    * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
    * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
    * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
    *   | MO 4 |      |      |      |      |                                       |         |      |   |      |      |
    *   `----------------------------------'                                       `----------------------------------'
    *                                        ,-------------.       ,-------------.
    *                                        |      |      |       |      |      |
    *                                 ,------|------|------|       |------+------+------.
    *                                 |      |      |      |       |      |      |      |
    *                                 |      |      |------|       |------|      |      |
    *                                 |      |      |      |       |      |      |      |
    *                                 `--------------------'       `--------------------'
    *
    *
    *
    */
   // Movement  Layer

  [4] = KEYMAP(
  //left hand
    RESET,       _______,       _______,       _______,        _______,         DNS,           CLR_DNS,
    _______,     _______,        CHRM_L,       KC_UP,           CHRM_R,         _______,       _______,
    _______,LCTL(KC_LEFT),      KC_LEFT,       KC_DOWN,       KC_RIGHT,    LCTL(KC_RIGHT),
    TO(0),       _______,       _______,       _______,        _______,         _______,       _______,
                 _______,       _______,       _______,        _______,         _______,
                                                                                _______,       _______,
                                                                                               _______,
                                                               _______,         _______,       _______,
  //right hand
    _______,     _______,       _______,       _______,        _______,         _______,       _______,
    _______,     _______,        CHRM_L,         KC_UP,         CHRM_R,         _______,       _______,
           LCTL(KC_LEFT),       KC_LEFT,       KC_DOWN,       LGUI_T(KC_RIGHT),  LCTL(KC_RIGHT),       _______,
    _______,     _______,       _______,       _______,        _______,         _______,       TO(0),
                 _______,       _______,       _______,        _______,         _______,
    _______,     _______,
    _______,
    _______,     _______,       _______),

    /* Keymap 4: Numpad
    * ,--------------------------------------------------.           ,--------------------------------------------------.
    * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
    * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
    * |        |TYFW  | MFKS | WBR  |  FR  |      |      |           |      |   =  |   7  |   8  |   9  |   *  |        |
    * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
    * |        |      |      |Cp/Cut|paste |      |------|           |------|   +  |   4  |   5  |   6  |   +  |        |
    * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
    * |        |      |      |      |      |      |      |           |      |   -  |   1  |   2  |   3  |   /  |        |
    * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
    *   |      |      |      |      |      |                                       |   0  |  0   |   .  |  00  | Enter|
    *   `----------------------------------'                                       `----------------------------------'
    *                                        ,-------------.       ,-------------.
    *                                        |      |      |       |      |      |
    *                                 ,------|------|------|       |------+------+------.
    *                                 |      |      |      |       |      |Nupmad|      |
    *                                 |      |      |------|       |------|Enter |      |
    *                                 |      |      |      |       |      |      |      |
    *                                 `--------------------'       `--------------------'
    */
    // Numpad  Layer
    [5] = KEYMAP(
      //left hand
      _______,        _______,           _______,           _______,        _______,        _______,    _______,
      _______,        TYFW,              _______,           _______,        _______,        _______,    _______,
      _______,        _______,           _______,   TD(TD_COPY_CUT),        PAST_PS,        _______,
      _______,        _______,           _______,           _______,        _______,        _______,    _______,
                      _______,           _______,           _______,        _______,        _______,
                                                                                            _______,    _______,
                                                                                                        _______,
                                                                            _______,        _______,    _______,

      //right hand
      _______,      _______,      _______,       _______,     _______,     _______,        _______,
      _______,     KC_EQUAL,      KC_7,          KC_8,        KC_9,        KC_ASTR,        _______,
                    KC_PLUS,      KC_4,          KC_5,        KC_6,        KC_PLUS,        _______,
      _______,     KC_MINUS,      KC_1,          KC_2,        KC_3,        KC_KP_SLASH,    _______,
                    KC_KP_0,    KC_KP_0,         KC_DOT,      DBL_0,       KC_KP_ENTER,
      _______,      _______,
      _______,
      _______,      KC_PENT,      _______),

      [6] = KEYMAP(
        /* "Speed cube layer" Everything is a spaaaaaace bar for use with https://cstimer.net/ */

        //left hand
        TO(0),           KC_SPACE,           KC_SPACE,           KC_SPACE,        KC_SPACE,        KC_SPACE,    KC_SPACE,
        KC_SPACE,        KC_SPACE,           KC_SPACE,           KC_SPACE,        KC_SPACE,        KC_SPACE,    KC_SPACE,
        KC_SPACE,        KC_SPACE,           KC_SPACE,           KC_SPACE,        KC_SPACE,        KC_SPACE,
        KC_SPACE,        KC_SPACE,           KC_SPACE,           KC_SPACE,        KC_SPACE,        KC_SPACE,    KC_SPACE,
                         KC_SPACE,           KC_SPACE,           KC_SPACE,        KC_SPACE,        KC_SPACE,
                                                                                                   KC_SPACE,    KC_SPACE,
                                                                                                                KC_SPACE,
                                                                                  KC_SPACE,        KC_SPACE,    KC_SPACE,

        //right hand
        KC_SPACE,      KC_SPACE,      KC_SPACE,          KC_SPACE,        KC_SPACE,        KC_SPACE,        TO(0),
        KC_SPACE,      KC_SPACE,      KC_SPACE,          KC_SPACE,        KC_SPACE,        KC_SPACE,        KC_SPACE,
                       KC_SPACE,      KC_SPACE,          KC_SPACE,        KC_SPACE,        KC_SPACE,        KC_SPACE,
        KC_SPACE,      KC_SPACE,      KC_SPACE,          KC_SPACE,        KC_SPACE,        KC_SPACE,        KC_SPACE,
                       KC_SPACE,      KC_SPACE,          KC_SPACE,        KC_SPACE,        KC_SPACE,
        KC_SPACE,      KC_SPACE,
        KC_SPACE,
        KC_SPACE,      KC_SPACE,      KC_SPACE),

};

const uint16_t PROGMEM fn_actions[] = {
  [1] = ACTION_LAYER_TAP_TOGGLE(1)
};

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
      switch(id) {
        case 0:
          if (record->event.pressed) {
            SEND_STRING (QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION);
          }
          break;
        case 2:
          if (record->event.pressed) {
            return MACRO( T(0), T(0), END  );
          }
          break;
        case 3:
          if (record->event.pressed) {
            return MACRO( D(LGUI), T(SPACE), U(LGUI), D(LSFT), T(L), U(LSFT), T(A), T(Y), T(E), T(R), T(SPACE), T(0), T(SPACE), T(MINUS), T(SPACE), D(LSFT), T(B), U(LSFT), T(A), T(S), T(E), END);
            /*SEND_STRING ("Layer 0: Base Layer - keyboard-layout-editor.com/#/gists/58a85096e0ad0f343b57ea005121b261");*/
          }
          break;
        case 4:
          if (record->event.pressed) {
            return MACRO( D(LGUI), T(SPACE), U(LGUI), D(LSFT), T(L), U(LSFT), T(A), T(Y), T(E), T(R), T(SPACE), T(1), T(SPACE), T(MINUS), T(SPACE), D(LSFT), T(S), U(LSFT), T(Y), T(M), T(B), END);
            /*SEND_STRING ("Layer 1: Symbol Layer - keyboard-layout-editor.com/#/gists/be9ae4b4100a41b8b942ca27a7978d68");*/
          }
          break;
        case 5:
            if (record->event.pressed) {
              return MACRO( D(LGUI), T(SPACE), U(LGUI), D(LSFT), T(L), U(LSFT), T(A), T(Y), T(E), T(R), T(SPACE), T(2), T(SPACE), T(MINUS), T(SPACE), D(LSFT), T(M), U(LSFT), T(O), T(U), T(S), END);
              /*SEND_STRING ("Layer 2: Media & Mousekeys Layer - keyboard-layout-editor.com/#/gists/e68114befd162858f8b4e7f5a2b34ee9");*/
            }
            break;
        case 6:
            if (record->event.pressed) {
              return MACRO( D(LGUI), T(SPACE), U(LGUI), D(LSFT), T(L), U(LSFT), T(A), T(Y), T(E), T(R), T(SPACE), T(3), T(SPACE), T(MINUS), T(SPACE), D(LSFT), T(A), U(LSFT), T(N), T(K), T(I), END);
              /*SEND_STRING ("Layer 3: Anki Layer - keyboard-layout-editor.com/#/gists/11bbe5e7df8672be282a83b41128920c");*/
            }
            break;
        case 7:
            if (record->event.pressed) {
            return MACRO( T(W), T(B), T(R), T(BSPACE), END  ); //this sends "WBR" - Typinator expands this to be my signature
          }
            break;
        case 8:
            if (record->event.pressed) {
                    key_timer = timer_read(); // if the key is being pressed, we start the timer.
            }
            else { // this means the key was just released, so we can figure out how long it was pressed for (tap or "held down").
                 if (timer_elapsed(key_timer) > 150) { // 150 being 150ms, the threshhold we pick for counting something as a tap.
                   return MACRO( D(LGUI), T(SPACE), U(LGUI), T(T), T(E), T(R), T(M),  T(ENTER),  T(S), T(E), T(T), T(G), T(O), T(O), T(G), T(L), T(E), T(D), T(N), T(S), END);
                 }
                 else {
                   return MACRO( D(LGUI), T(SPACE), U(LGUI), T(T), T(E), T(R), T(M),  T(ENTER),  T(S), T(E), T(T), T(H), T(O), T(M), T(E), T(D), T(N), T(S), END);
                 }
            }
            break;
        case 9:
            if (record->event.pressed) {
                    key_timer = timer_read();
            }
            else {
                 if (timer_elapsed(key_timer) > 150) {
                   return MACRO( D(LGUI), T(SPACE), U(LGUI), T(T), T(E), T(R), T(M),  T(ENTER),  T(C), T(L), T(E), T(A), T(R), T(D), T(N), T(S), END);
                 }
                 else {
                   return MACRO( D(LGUI), T(SPACE), U(LGUI), T(T), T(E), T(R), T(M),  T(ENTER),  D(LSFT), T(BSLASH), T(BSLASH), U(LSFT), T(S), T(S), T(H), END);
                 }
            }
            break;
        case 10:
            if (record->event.pressed) {
            SEND_STRING ("https://xkcd.com/1319/");
          }
            break;
        case 11:
            if (record->event.pressed) {
                    key_timer = timer_read();
            }
            else {
                 if (timer_elapsed(key_timer) > 150) {
                   return MACRO( D(LSFT), T(BSLASH), T(BSLASH), U(LSFT), T(V), T(I), T(D), T(E), T(O), END);
                 }
                 else {
                   return MACRO( D(LSFT), T(BSLASH), T(BSLASH), U(LSFT), T(S), T(C), T(R), T(N), END);
                 }
        }
           break;

        case 12:
            if (record->event.pressed) {
                    key_timer = timer_read();
            }
            else {
                 if (timer_elapsed(key_timer) > 150) {
                   return MACRO( D(LGUI), D(LSFT), T(V), U(LSFT), U(LGUI),  END);
                 }
                 else {
                  return MACRO( D(LGUI), T(V), U(LGUI),  END);                 }
        }
           break;

        case 13:
            if (record->event.pressed){
              return MACRO( D(LGUI), T(SPACE), U(LGUI), T(S), T(L), T(A), T(C), T(K), T(ENTER), END);
            }
            break;
        case 14:
            if (record->event.pressed) {
                    key_timer = timer_read();
            }
            else {
                 if (timer_elapsed(key_timer) > 150) {
                   return MACRO( D(LGUI), T(SPACE), U(LGUI), T(T), T(O), T(D), T(O), T(I), T(ENTER), END); //switch to Todoist when held and released
                 }
                 else {
                   return MACRO( D(LGUI), D(LSFT), T(A), U(LSFT), U(LGUI), END); //macro to open Todoist new task dialog
                 }
        }
           break;
        case 15:
            if (record->event.pressed){
              return MACRO( D(LGUI), T(SPACE), U(LGUI), T(A), T(T), T(O), T(M), T(ENTER), END);
            }
            break;
        case 16:
            if (record->event.pressed){
              return MACRO( D(LGUI), T(SPACE), U(LGUI), D(LSFT), T(BSLASH), T(BSLASH), U(LSFT), T(Z), T(E), T(N), END);
            }
            break;
        case 17:
            if (record->event.pressed){
              return MACRO( D(LGUI), T(SPACE), U(LGUI), D(LSFT), T(BSLASH), T(BSLASH), U(LSFT), T(P), T(B), T(U), END);
            }
            break;

        case 18:
            if (record->event.pressed) {
                    key_timer = timer_read();
            }
            else {
                 if (timer_elapsed(key_timer) > 200) {
                   return MACRO( D(LGUI), T(S), U(LGUI), END);
                 }
                 else {
                   return MACRO(  I(0), T(S), END);
                 }
        }
        case 19:
            if (record->event.pressed) {
                    key_timer = timer_read();
            }
            else {
                 if (timer_elapsed(key_timer) > 200) {
                   return MACRO( D(LGUI), T(A), U(LGUI), END);
                 }
                 else {
                   return MACRO( I(0), T(A), END);
                 }
        }
    case 20:
            if (record->event.pressed) {
            return MACRO( T(T), T(Y), T(F), T(W), T(SPACE), END);
          }
        break;
    }
    return MACRO_NONE;
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    // dynamically generate these.
    case EPRM:
      if (record->event.pressed) {
        eeconfig_init();
      }
      return false;
      break;
    case VRSN:
      if (record->event.pressed) {
        SEND_STRING (QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION);
      }
      return false;
      break;
  }
  return true;
}


void matrix_scan_user(void) {

};
