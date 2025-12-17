# MeagerDox - Ergodox Keyboard Firmware

## Hardware

- **Board:** Ergodox EZ (custom-built)
- **MCU:** Teensy 2.0 (ATmega32U4)
- **Bootloader:** HalfKay
- **LEDs:** None (no RGB)
- **Features:** Mouse keys, media keys, 7 layers

## Environment

- **QMK CLI:** 1.1.8
- **QMK Firmware:** 0.27.1
- **QMK Repo Location:** `/Users/mat/git/qmk`
- **Keymap Location:** `/Users/mat/git/qmk/keyboards/ergodox_ez/keymaps/meagerfindings/`
- **OS:** macOS on Apple Silicon (M2 Pro)

## File Structure

This repo contains the source files that are copied into the QMK repo for compilation:

| File | Purpose |
|------|---------|
| `keymap.c` | Main keymap with 7 layers, macros, tap dance |
| `config.h` | Mouse key settings, tapping term |
| `rules.mk` | Build flags, bootloader setting |

## Layers

0. **BASE** - Primary QWERTY layer
1. **OLD_BASE** - Alternative base layer
2. **SYMBL** - Symbols and special characters
3. **MEDIA** - Media controls and mouse keys
4. **MKDWN** - Markdown shortcuts (headers, links)
5. **ARROWS** - Arrow keys and navigation
6. **NUM** - Number pad

## Commands

```bash
# Copy files to QMK repo (run from MeagerDox dir)
cp keymap.c config.h rules.mk ~/git/qmk/keyboards/ergodox_ez/keymaps/meagerfindings/

# Compile
qmk compile -kb ergodox_ez -km meagerfindings

# Flash (hold reset button on Teensy when prompted)
qmk flash -kb ergodox_ez -km meagerfindings

# Check environment
qmk doctor
```

## ⚠️ Critical: File Sync Workflow

**The MeagerDox repo and QMK repo are separate!** Changes made here do NOT automatically sync to the QMK repo.

### Before Flashing

Always copy files from MeagerDox to QMK before compiling/flashing:

```bash
# From MeagerDox directory
cp keymap.c config.h rules.mk ~/git/qmk/keyboards/ergodox_ez/keymaps/meagerfindings/

# Then compile/flash
qmk flash -kb ergodox_ez -km meagerfindings
```

### Why This Matters

- **MeagerDox repo** (`/Users/mat/git/MeagerDox`): Source of truth, version controlled
- **QMK repo** (`~/git/qmk/keyboards/ergodox_ez/keymaps/meagerfindings/`): Where compilation happens

If you skip the copy step, QMK will compile the old files and your changes won't appear on the keyboard.

## Flashing Process

1. Run `qmk flash -kb ergodox_ez -km meagerfindings`
2. When prompted "Waiting for bootloader...", hold the reset button on the Teensy
3. Release when flashing begins
4. Keyboard will reconnect automatically

## Key Patterns

- **Macros:** Handled in `process_record_user()` with timer-based hold detection
- **Tap Dance:** Uses `ACTION_TAP_DANCE_DOUBLE` for dual-function keys
- **Layer Access:** `get_highest_layer(layer_state)` for layer state checking
- **Layout Macro:** `LAYOUT_ergodox()` for key definitions

## Custom Features

- **TD_COPY_CUT:** Tap=Copy, Double-tap=Cut
- **CT_LBP / CT_RBP:** Bracket tap dance (parentheses/brackets)
- **Macros:** MD_LINK, XKCD, TODO, RUBYMINE, ZENDESK, PAST_PS, etc.
- **OSM Keys:** One-shot Hyper and Shift modifiers

## Firmware Size

Last build: 22,676/32,256 bytes (70% usage, ~9.5KB free)

## Troubleshooting

- **Bootloader not detected:** Ensure Teensy reset button is accessible; try different USB port
- **Compilation errors:** Run `qmk doctor` to verify environment
- **Keys not working:** Check layer activation; verify `LAYOUT_ergodox` positions match physical keys
