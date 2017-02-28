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

//Redifne Key Names for readaibilty:
#define _______ KC_TRANSPARENT
#define XXXXXXX KC_NO
#define TODO LGUI(LSFT(KC_A)) //shortcut for adding a task to Todoist

//define Macro Names
#define DOUBLE_ZERO M(2) //Types two numpad zeroes
#define LayerT0 M(3) //Prints layer identifier text for layer 0
#define LayerT1 M(4) //Prints layer identifier text for layer 1
#define LayerT2 M(5) //Prints layer identifier text for layer 2
#define LayerT3 M(6) //Prints layer identifier text for layer 3
#define WBR M(7) //Typinator signature abbreviation
#define DNS M(8) //Typinator abbreviation to set PiHole DNS when tapped, when held, Typinator abbreviation to set Google DNS
#define CLR_DNS M(9) //Typinator abbreviation to clear OS X dns when tapped, when held, Typinator abbreviation for ssh into rPI
#define XKCD_AUTO M(10) //Sends string "https://xkcd.com/1319/"
#define SCRN_VIDEO M(11) //When tapped:Typinator screenshot abbreviation. Result = "(Screenshot: <clilpboard>)" When held: //Typinator screenshot abbreviation. Result = "(Screenshot: <clilpboard>)"
#define PAST_PS M(12) // Tap = CMD + V , Held = CMD + Shift + V

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

/*
void td_layers(qk_tap_dance_state_t *state, void *user_data) {
  switch (state->count) {
  case 1:
    register_code(LGUI_T(TD_B_L_SEL));
    unregister_code(LGUI_T(TD_B_L_SEL));
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
*/

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
  /*[TD_LAYERS] = ACTION_TAP_DANCE_DOUBLE (KC_LGUI, TD(TD_B_L_SEL)),*/
  [TD_OSL3] = ACTION_TAP_DANCE_FN (td_osl3_tol3),
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  /* Keymap 0: Basic layer
  * ,--------------------------------------------------.           ,--------------------------------------------------.
  * |   =    |   1  |   2  |   3  |   4  |   5  | Paste|           |Snagit|   6  |   7  |   8  |   9  |   0  |   -    |
  * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
  * | Tab    |   Q  |   W  |   E  |   R  |   T  | Copy |           | SCRN |   Y  |   U  |   I  |   O  |   P  |   \    |
  * |--------+------+------+------+------+------| /Cut |           | VIDEO|------+------+------+------+------+--------|
  * | NULL   |   A  |   S  |   D  |   F  |   G  |------|           |------|   H  |   J  |   K  |   L  | ; /L2|   '    |
  * |--------+------+------+------+------+------|Layer |           | Layer|------+------+------+------+------+--------|
  * |  ( [ { |Z/Ctrl|X/Alt |   C  |   V  |   B  |  1   |           |   2  |   N  |   M  |   ,  | ./Alt|//Ctrl| ) ] }  |
  * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
  *   |  `   |      |      |      | ToDo |                                       |      |      |      |      |OS3/L3|
  *   `----------------------------------'                                       `----------------------------------'
  *                                        ,-------------.       ,--------------.
  *                                        | Esc  | Home |       |Layer?| OSL 2 |
  *                                 ,------|------|------|       |------+-------+------.
  *                                 |      |      | End  |       | Undo |       |      |
  *                                 |Shift |Backsp|------|       |------| Return|Space/|
  *                                 | OSM  | /CMD | DEL  |       |CMDEsc| /LT1  |Arrows|
  *                                 `--------------------'       `---------------------'
  *
  *Copy & Paste key copies on one tap and pastes on two taps, and on three taps cuts.
  *
  *One tap on Snagit key = is Ctrl + Shift + C which is Snagit's selector.
  *Two taps on Snagit key = Cmd + Shift + Opt + 4 (OS X cropping screenshot that is copied to the clipboard only.)
  */
 // If it accepts an argument (i.e, is a function), it doesn't need KC_.
 // Otherwise, it needs KC_*
  [0] = KEYMAP(
    //left hand
    KC_EQUAL,     KC_1,     KC_2,     KC_3,     KC_4,     KC_5,          PAST_PS,
    KC_TAB,       KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,          TD(TD_COPY_CUT),
    XXXXXXX,      KC_A,     KC_S,     KC_D,     KC_F,     KC_G,
    TD(CT_LBP), CTL_T(KC_Z), ALT_T(KC_X), KC_C, KC_V,     KC_B,          TO(1),
               KC_GRAVE,    _______,  _______,  _______,  TODO,
                                                          KC_ESCAPE,     KC_HOME,
                                                                         KC_END,
                                      OSM(MOD_LSFT), GUI_T(KC_BSPACE),   KC_DELETE,

    //right hand
    TD(TD_SNAGIT),     KC_6,     KC_7,     KC_8,     KC_9,          KC_0,             KC_MINUS,
    SCRN_VIDEO,        KC_Y,     KC_U,     KC_I,     KC_O,          KC_P,             KC_BSLASH,
                       KC_H,     KC_J,     KC_K,     KC_L,          LT(2,KC_SCOLON),  KC_QUOTE,
    TO(2),             KC_N,     KC_M,     KC_COMMA, ALT_T(KC_DOT), CTL_T(KC_SLASH),  TD(CT_RBP),
                       _______,  _______,  _______,  _______,       TD(TD_OSL3),
    LayerT0,           OSL(2),
    LGUI(KC_Z),
    GUI_T(KC_ESCAPE),   LT(1,KC_ENTER),  LT(5,KC_SPACE)),

    /* Keymap 1: Symbol & Numpad Layer
     * ,--------------------------------------------------.           ,--------------------------------------------------.
     * |        |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |           |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 |    -   |
     * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
     * |Shft Tab|   !  |   @  |   &  |   }  |   |  | Cmd  |           |      | NULL |   7  |   8  |   9  |   *  |        |
     * |--------+------+------+------+------+------| + K  |           |      |------+------+------+------+------+--------|
     * |        |   #  |   $  |   !  |   `  |   @  |------|           |------| NULL |   4  |   5  |   6  |   +  |        |
     * |--------+------+------+------+------+------|Layer |           |Layer |------+------+------+------+------+--------|
     * |        |   %  |   ^  |   [  |   ]  |   ~  |  0   |           |  2   |   &  |   1  |   2  |   3  |   /  |        |
     * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
     *   |      |      |      |      |      |                                       |   0  |  00  |   .  |   =  | Enter|
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
    LSFT(KC_TAB),     _______,           KC_AT,             KC_AMPR,        _______,        KC_PIPE,    LGUI(KC_K),
    _______,          KC_HASH,           KC_DLR,            KC_EXLM,        KC_GRAVE,        KC_AT,
    _______,          KC_PERC,           KC_CIRC,           _______,        _______,        KC_TILD,    TO(0),
                       _______,          _______,           _______,        _______,        _______,
                                                                                            _______,   _______,
                                                                                                       _______,
                                                                            _______,        _______,   _______,

    //right hand
    KC_F7,        KC_F8,        KC_F9,            KC_F10,      KC_F11,      KC_F12,         KC_MINUS,
    _______,      XXXXXXX,      KC_7,             KC_8,        KC_9,        KC_ASTR,        _______,
                  XXXXXXX,      KC_4,             KC_5,        KC_6,        KC_PLUS,        _______,
    TO(2),        KC_AMPR,      KC_1,             KC_2,        KC_3,        KC_KP_SLASH,    _______,
                  KC_KP_0,      DOUBLE_ZERO,      KC_DOT,      KC_EQUAL,    KC_KP_ENTER,
    LayerT1,      _______,
    _______,
    _______,      _______,      _______),

  /* Keymap 2: Media and mouse keys
   * ,--------------------------------------------------.           ,--------------------------------------------------.
   * |        |      |      |      |      |      |      |           | XKCD |      |      |      |      |      |  F15   |
   * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
   * |        |Whl Up|      | MsUp |      |Whl Dn|      |           |      |      |      | AltUp|      |      |  F14   |
   * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
   * |        |Ctl L |MsLeft|MsDown|MsRght| Ctl R|------|           |------|      | Alt L|AltDn | Alt R|      |  Play  |
   * |--------+------+------+------+------+------|Layer |           |Layer |------+------+------+------+------+--------|
   * |        |WHL L |      |      |      | Whl R|  1   |           |  0   |      |      | Prev | Next |      |        |
   * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
   *   |      |      |      |      |      |                                       | Mute |VolDn |VolUp |      |      |
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
    _______,    KC_MS_WH_UP,     _______,         KC_MS_UP,        _______,         KC_MS_WH_DOWN,   _______,
    _______,    LCTL(KC_LEFT),   KC_MS_LEFT,      KC_MS_DOWN,      KC_MS_RIGHT,     RCTL(KC_RIGHT),
    _______,    KC_MS_WH_LEFT,   _______,         _______,         _______,         KC_MS_WH_RIGHT,  TO(1),
                _______,         _______,         _______,         _______,         _______,
                                                                                    _______,         _______,
                                                                                                     _______,
                                                                   KC_MS_BTN1,      KC_MS_BTN2,      _______,

    //right hand
    XKCD_AUTO,  _______,       _______,               _______,               _______,    _______,    KC_F15,
    _______,    _______,       _______,           LALT(KC_UP),               _______,    _______,    KC_F14,
                _______, LALT(KC_LEFT),         LALT(KC_DOWN),        LALT(KC_RIGHT),    _______,    KC_MEDIA_PLAY_PAUSE,
    TO(0),      _______,       _______,   KC_MEDIA_PREV_TRACK,   KC_MEDIA_NEXT_TRACK,    _______,    _______,
                       KC_AUDIO_MUTE,       KC_AUDIO_VOL_DOWN,       KC_AUDIO_VOL_UP,    _______,    _______,
    LayerT2,           _______,
    _______,
    _______,    KC_WWW_FORWARD,    LT(5,KC_WWW_BACK)),

    /* Keymap 3: Anki Layer for adding cloze deletions, secondary cloze deletions and new cards
     * ,--------------------------------------------------.           ,--------------------------------------------------.
     * |        |      |      |      |      |      |Sc cp |           |      |      |      |      |      |      |        |
     * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
     * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
     * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
     * |        |      |      |      |      |      |------|           |------|      | Wrong| Hard | Good | Easy |        |
     * |--------+------+------+------+------+------|Layer0|           |Layer0|------+------+------+------+------+--------|
     * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
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
    _______,       _______,       _______,       _______,       _______,       _______,       LCTL(LGUI(LSFT(KC_4))),
    _______,       _______,       _______,       _______,       _______,       _______,       _______,
    _______,       _______,       _______,       _______,       _______,       _______,
    _______,       _______,       _______,       _______,       _______,       _______,       TO(0),
                   _______,       _______,       _______,       _______,       _______,
                                                                               _______,       _______,
                                                                                              _______,
                                                LGUI(LSFT(KC_C)),     LALT(LGUI(LSFT(KC_C))), _______,

   //right hand
   _______,       _______,       _______,       _______,       _______,       _______,       _______,
   _______,                      _______,       _______,       _______,       _______,       _______,       _______,
                                 _______,       KC_1,          KC_2,          KC_3,          KC_4,          _______,
   TO(0),                        _______,       _______,       _______,       _______,       _______,       _______,
                                 _______,       _______,       _______,       _______,       TO(0),
   LayerT3,                      _______,
   _______,
   _______,               LGUI(KC_ENTER),       _______),

   /* Keymap 5: Movement
    *
    * ,--------------------------------------------------.           ,--------------------------------------------------.
    * |  RESET |      |      |      |      |  DNS |clrDNS|           |      |      |      |      |      |      |        |
    * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
    * |        |      | Opt+L|  Up  |Opt+R |      |      |           |      |      |      |  Up  |      |      |        |
    * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
    * |        |Ctrl+L| Left |  Dn  | Right|Ctrl+R|------|           |------|      | Left |  Dn  | Right|      |        |
    * |--------+------+------+------+------+------|      |           |Layer0|------+------+------+------+------+--------|
    * |        |      |      |  Dn  |      |      |      |           |      |      |      |  Dn  |      |      |        |
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

  [5] = KEYMAP(
  //left hand
    RESET,       _______,       _______,       _______,        _______,         DNS,           CLR_DNS,
    _______,     _______, LALT(LGUI(KC_LEFT)),   KC_UP, LALT(LGUI(KC_RIGHT)),   _______,       _______,
    _______,LCTL(KC_LEFT),      KC_LEFT,       KC_DOWN,       KC_RIGHT,    LCTL(KC_RIGHT),
    _______,     _______,       _______,       KC_DOWN,        _______,         _______,       _______,
                 _______,       _______,       _______,        _______,         _______,
                                                                                _______,       _______,
                                                                                               _______,
                                                               _______,         _______,       _______,
  //right hand
    _______,     _______,       _______,       _______,        _______,         _______,       _______,
    _______,     _______, LALT(LGUI(KC_LEFT)), KC_UP, LALT(LGUI(KC_RIGHT)),     _______,       _______,
           LCTL(KC_LEFT),       KC_LEFT,       KC_DOWN,        KC_RIGHT, LCTL(KC_RIGHT),       _______,
    TO(0),       _______,       _______,       KC_DOWN,        _______,         _______,       _______,
                 _______,       _______,       _______,        _______,         _______,
    _______,     _______,
    _______,
    _______,     _______,       _______),

};

const uint16_t PROGMEM fn_actions[] = {
  [1] = ACTION_LAYER_TAP_TOGGLE(1)
};

// leaving this in place for compatibilty with old keymaps cloned and re-compiled.
const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
      switch(id) {
        case 0:
          if (record->event.pressed) {
            SEND_STRING (QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION);
          }
          break;
        case 2: // this would trigger when you hit a key mapped as M(2)
          if (record->event.pressed) {
            return MACRO( T(0), T(0), END  ); //this sends 00
          }
          break;
        case 3:
          if (record->event.pressed) {
            SEND_STRING ("Layer 0: Base Layer - keyboard-layout-editor.com/#/gists/58a85096e0ad0f343b57ea005121b261");
          }
          break;
        case 4:
          if (record->event.pressed) {
            SEND_STRING ("Layer 1: Symbol Layer - keyboard-layout-editor.com/#/gists/be9ae4b4100a41b8b942ca27a7978d68");
          }
          break;
        case 5:
            if (record->event.pressed) {
              SEND_STRING ("Layer 2: Media & Mousekeys Layer - keyboard-layout-editor.com/#/gists/e68114befd162858f8b4e7f5a2b34ee9");
            }
            break;
        case 6:
            if (record->event.pressed) {
              SEND_STRING ("Layer 3: Anki Layer - keyboard-layout-editor.com/#/gists/11bbe5e7df8672be282a83b41128920c");
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
                    key_timer = timer_read(); // if the key is being pressed, we start the timer.
            }
            else { // this means the key was just released, so we can figure out how long it was pressed for (tap or "held down").
                 if (timer_elapsed(key_timer) > 150) { // 150 being 150ms, the threshhold we pick for counting something as a tap.
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
                    key_timer = timer_read(); // if the key is being pressed, we start the timer.
            }
            else { // this means the key was just released, so we can figure out how long it was pressed for (tap or "held down").
                 if (timer_elapsed(key_timer) > 150) { // 150 being 150ms, the threshhold we pick for counting something as a tap.
                   return MACRO( D(LSFT), T(BSLASH), T(BSLASH), U(LSFT), T(V), T(I), T(D), T(E), T(O),  T(LEFT), T(SPACE), T(BSPACE), T(RIGHT),  END);
                 }
                 else {
                   return MACRO( D(LSFT), T(BSLASH), T(BSLASH), U(LSFT), T(S), T(C), T(R), T(N),  T(LEFT), T(SPACE), T(BSPACE), T(RIGHT),  END);
                 }
        }
           break;

        case 12:
            if (record->event.pressed) {
                    key_timer = timer_read(); // if the key is being pressed, we start the timer.
            }
            else { // this means the key was just released, so we can figure out how long it was pressed for (tap or "held down").
                 if (timer_elapsed(key_timer) > 150) { // 150 being 150ms, the threshhold we pick for counting something as a tap.
                   return MACRO( D(LGUI), D(LSFT), T(V), U(LSFT), U(LGUI),  END);
                 }
                 else {
                  return MACRO( D(LGUI), T(V), U(LGUI),  END);                 }
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
    ergodox_board_led_off();
    ergodox_right_led_1_off();
    ergodox_right_led_2_off();
    ergodox_right_led_3_off();
};
