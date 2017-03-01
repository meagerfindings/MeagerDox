# MeagerDox
I decided to build an ErgoDox as I wanted to use my thumbs for more with the added ergonimc benefits of a split board and ortholinear layout. I knew my hands were hurting from the weird contortions of copying and pasting constantly on my Mac, but I had no idea how strained I really was until I started moving as many modifier keys to the thumb clusters.

The overall goal of this layout is to place many modifiers into the thumb clusters and often used keys to the home row, while retaining the `QWERTY` layout for Mac.

##Credit where credit is due
The intial layout used was from the [ErgoDox EZ Configurator](https://ergodox-ez.com/pages/graphical-configurator). I went through 26+ iterations within the configurator prior to beginning my journey with QMK itself. Much of the dual function/layer keys are influenced by and adapted from the EZ Configurator.

Many features in this layout are either directly inspired by or adaptations of [Algernon's incredible ErgoDox layout](https://github.com/algernon/ergodox-layout). Specifically: TapDance and its usage, and the brillant idea of using TapDance to combining the four keys for parens, brackets, and curly braces into 2 keys.

Diagonal MouseKeys and MouseKeys Macros were adapted from [Michael Kreene's "The Ordinary ErgoDox" Layout](https://github.com/qmk/qmk_firmware/blob/master/keyboards/ergodox/keymaps/ordinary/keymap.c).


##Layer 0: Base
[![Base-layer](images/base-layer.png)](http://www.keyboard-layout-editor.com/#/gists/58a85096e0ad0f343b57ea005121b261)

* `Screen/Video` key sends `||scrn` whent tapped and '||video when held'
  - [Typinator](http://www.ergonis.com/products/typinator/) expands `||scrn` into `(Screenshot: <CLIPBOARD CONTENTS>)`, and `||video` into `(Video: <CLIPBOARD CONTENTS>)`, the QMK macro then moves back to before the last paren, inserts a space and removes a space to create a hyperlink for use in Zendesk.
* Tapping the `([{`/`}])` keys once yields `(`, tapping them twice yields `[` (or `{` when shifted).



##Layer 1: Symbols

##Layer 3: Anki

##Layer 4: Arrow/Movements
