# Ergodox Layout Update & Flashing Plan

## Project Overview
Update MeagerDox layout from outdated version to current QMK firmware (as of Dec 2025), set up Mac M2 Pro for flashing, and successfully reflash the custom-built Ergodox.

## Overall Project Status
**🎉 PROJECT COMPLETE - FIRMWARE FLASHED, TESTED, AND VERIFIED**

✅ Research completed with comprehensive findings
✅ Environment setup (QMK CLI 1.1.8, QMK 0.27.1)
✅ Code migration from 2017-era syntax to modern QMK standards
✅ Firmware compiled successfully (70% of device memory)
✅ Firmware flashed successfully via HalfKay bootloader
✅ Comprehensive testing of all layers and features completed
✅ TD_SNAGIT macro removed from codebase

---

## Phase 1: Research
**Status:** ✅ COMPLETE  
**Owner:** Research Agent

### 1.1 QMK Firmware Current State
- [x] Determine current stable QMK version (as of Dec 2025)
- [x] Identify breaking changes since layout was last updated
- [x] Document any deprecated features or folder structure changes
- [x] Note any new Ergodox-specific improvements/features available

**Findings:**
```
CURRENT VERSION: QMK 0.29.0 (released May 25, 2025)
  - Next breaking changes cycle: August 31, 2025 (version 0.30.0)
  - November 30, 2025 changelog shows version 0.30.0 with "Speculative Hold" feature

MAJOR BREAKING CHANGES SINCE YOUR LAYOUT WAS LAST UPDATED:
(Your layout appears to be from ~2017-2018 era based on syntax)

1. KEYCODE CHANGES (CRITICAL):
   - `RESET` → `QK_BOOT` (May 2022, v0.17.0) ⚠️ YOUR KEYMAP USES `RESET`
   - Many legacy keycodes deprecated in send_string
   - Mouse keycodes updated (old `RGB_*` and mouse keycodes removed July 2025)

2. CONFIG/BUILD SYSTEM CHANGES:
   - `QMK_KEYBOARD_CONFIG_H` removed - use `#pragma once` instead
   - `PREVENT_STUCK_MODIFIERS` removed (now default behavior)
   - `FORCE_NKRO` deprecated (May 2025) - use `host.default.nkro` in keyboard.json
   - `fn_actions[]` array completely deprecated and removed
   - `COMMAND_ENABLE` syntax unchanged but rarely used now
   - Data-driven configuration via `keyboard.json` is now preferred

3. FOLDER STRUCTURE:
   - Handwired keyboards go in `keyboards/handwired/` folder
   - Ergodox EZ is at `keyboards/ergodox_ez/` 
   - For custom builds, you'd use `keyboards/handwired/ergodox/` or similar
   
4. NEW FEATURES AVAILABLE:
   - Flow Tap (May 2025): Better HRM handling during fast typing
   - Caps Word: Auto-capitalize until word ends
   - Encoder Mapping: Easier encoder configuration
   - Community Modules: Reusable code modules
```

### 1.2 Current Layout Assessment
- [x] Examine existing keymap.c, config.h, and rules.mk
- [x] Identify deprecated syntax or deprecated QMK features in use
- [x] List all custom features/macros that may need updating
- [x] Check for compatibility issues with modern QMK

**Findings:**
```
DEPRECATED ITEMS IN YOUR CODEBASE:

keymap.c:
├── Line 254: `RESET` → Must change to `QK_BOOT`
├── Lines 315-317: `fn_actions[]` array → REMOVE ENTIRELY (deprecated)
├── Line 485: `biton32(layer_state)` → Use `get_highest_layer(layer_state)` instead
├── Lines 2-4: `#include "debug.h"` and `#include "action_layer.h"` → May be unnecessary
└── Line 5: `#include "version.h"` → Still valid but optional

config.h:
├── Lines 1-2: `#ifndef/#define` guard → Use `#pragma once` instead
├── Line 4: `#include QMK_KEYBOARD_CONFIG_H` → REMOVE (no longer exists)
├── Line 32: `PREVENT_STUCK_MODIFIERS` → REMOVE (now default behavior)
├── Lines 6-17: USB descriptor overrides → Move to keyboard.json for modern approach
└── MOUSEKEY settings → Still valid, no changes needed

rules.mk:
├── Line 1: `COMMAND_ENABLE = no` → Still valid
├── Line 2: `TAP_DANCE_ENABLE = yes` → Still valid
└── Line 3: `FORCE_NKRO = yes` → DEPRECATED, use keyboard.json setting instead

CUSTOM FEATURES TO PRESERVE:
- Tap Dance definitions (TD_COPY_CUT, TD_SNAGIT, CT_LBP, CT_RBP)
- Custom keycodes: MD_LINK, XKCD, DBLE_ZER0, PAST_PS, TODO, RUBYMINE, ZENDESK, etc.
- Layer IDs macros (L_ID_0, L_ID_1, L_ID_2)
- Markdown helpers (H_ONE through H_FIVE, M_LINK, M_GREATER)
- OSM(MOD_HYPR) and OSM(MOD_LSFT) usage
- Complex layer tap configurations

COMPATIBILITY ASSESSMENT:
- Core keymap structure (LAYOUT_ergodox) is still valid
- process_record_user() function signature unchanged
- Tap dance definitions syntax unchanged
- Most custom keycodes just need SAFE_RANGE verified
- LED control via ergodox_*_led functions still works
```

### 1.3 Mac M2 Pro Flashing Requirements
- [x] Identify required tools (dfu-util, avrdude, etc. for ARM Macs)
- [x] Determine correct AVR compiler toolchain for M2 Pro
- [x] Check for any architecture-specific issues (ARM-native vs Rosetta)
- [x] Document QMK CLI installation on macOS

**Findings:**
```
INSTALLATION STEPS FOR MAC M2 PRO:

1. HOMEBREW (if not installed):
   /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

2. QMK CLI INSTALLATION:
   brew install qmk/qmk/qmk

3. QMK SETUP:
   qmk setup
   - Answer 'y' to install dependencies
   - This will clone qmk_firmware and install AVR toolchain

4. QMK TOOLBOX (optional but recommended GUI):
   brew install --cask qmk-toolbox

TOOLCHAIN NOTES FOR ARM MACS:
- QMK's Homebrew formula handles ARM-native builds automatically
- avr-gcc and arm-none-eabi-gcc are installed via qmk setup
- dfu-programmer used for Atmel DFU bootloaders
- teensy_loader_cli used for Teensy/HalfKay bootloaders
- All tools now have native ARM builds (no Rosetta needed)

POTENTIAL ISSUES:
- Some older tutorials reference x86 paths - ignore those
- QMK Toolbox works natively on Apple Silicon
- If using Teensy: ensure using latest teensy_loader_cli

VERIFICATION COMMANDS:
  qmk doctor        # Check environment
  qmk compile -kb ergodox_ez -km default  # Test compile
```

### 1.4 Ergodox-Specific Details
- [x] Confirm microcontroller type (Teensy, Pro Micro, etc.)
- [x] Identify bootloader type
- [x] Verify current QMK Ergodox folder structure requirements
- [x] Check for any recent changes to Ergodox EZ vs standard Ergodox handling

**Findings:**
```
ERGODOX HARDWARE VARIANTS:

1. ERGODOX EZ (commercial):
   - MCU: ATmega32U4
   - Bootloader: HalfKay (Teensy 2.0 compatible)
   - QMK Path: keyboards/ergodox_ez/
   - Build: qmk compile -kb ergodox_ez -km default

2. ORIGINAL/HANDBUILT ERGODOX (your likely setup):
   - MCU: Teensy 2.0 (ATmega32U4) - MOST COMMON for handbuilds
   - Bootloader: HalfKay
   - QMK Path: keyboards/ergodox_ez/ (can be used)
   - Alternative: keyboards/handwired/ (for custom setup)

DETERMINING YOUR HARDWARE:
Since you have a "custom-built Ergodox", you likely have:
- Teensy 2.0 controller (most common for DIY Ergodox)
- HalfKay bootloader (comes pre-flashed on Teensy)
- I/O Expander: MCP23018 (for the other half)

TO CONFIRM YOUR MCU:
1. Look at the controller in your keyboard
2. If it says "Teensy 2.0" or "PJRC" → HalfKay bootloader
3. If Pro Micro → Caterina bootloader (different flashing)

BOOTLOADER SETTINGS FOR rules.mk:
For Teensy 2.0 (HalfKay):
  BOOTLOADER = halfkay

For Pro Micro:
  BOOTLOADER = caterina

FLASHING COMMANDS:
Teensy/HalfKay:
  qmk flash -kb ergodox_ez -km <your_keymap>
  # Or manually: teensy_loader_cli -mmcu=atmega32u4 -w <firmware.hex>

ENTERING BOOTLOADER:
- Press reset button on Teensy (if accessible)
- Or use QK_BOOT key if already in firmware
- Or short RST to GND pins

QMK FOLDER STRUCTURE FOR ERGODOX_EZ:
keyboards/ergodox_ez/
├── base/           # Base configuration
├── glow/           # RGB variant
├── keymaps/        # User keymaps go here
│   ├── default/
│   └── <your_keymap>/
├── config.h
├── ergodox_ez.c
├── ergodox_ez.h
├── info.json       # New data-driven config
├── matrix.c
└── rules.mk
```

---

## Phase 2: Environment Setup
**Status:** ✅ COMPLETE  
**Owner:** User (completed)

### 2.1 Prerequisites
- [x] Install Homebrew (if not installed)
- [x] Install QMK CLI via Homebrew
- [x] Run `qmk setup` to install dependencies
- [x] (Optional) Install QMK Toolbox for GUI flashing

**Actual Completion:**
```
✅ QMK CLI Version: 1.1.8
✅ QMK Firmware: 0.27.1
✅ Location: /Users/mat/git/qmk
✅ Homebrew: Already installed
✅ All dependencies installed and verified
```

### 2.2 QMK Setup
- [x] Clone/update QMK firmware repository
- [x] Verify QMK environment setup (`qmk setup`)
- [x] Test QMK build command works

**Actual Results:**
```
✅ QMK repo cloned and configured at /Users/mat/git/qmk
✅ Test compilation successful
✅ AVR toolchain verified and working
```

### 2.3 Ergodox Firmware Dependencies
- [x] Verify Ergodox-specific dependencies are present
- [x] Test compilation of existing Ergodox example layout

**Verification Results:**
```
✅ Ergodox EZ board definitions: Present and verified
✅ MCP23018 I/O expander drivers: Present
✅ All build dependencies: Satisfied
✅ Test build of ergodox_ez/default keymap: Successful
```

---

## Phase 3: Layout Modification
**Status:** ✅ COMPLETE  
**Owner:** Modification Agent

### 3.1 Update Syntax & Structure
- [x] Update keymap.c to current QMK syntax
- [x] Migrate config.h settings to current format  
- [x] Update rules.mk for modern QMK build system
- [x] Address any deprecated feature syntax

**Changes Required:**
```
keymap.c:
1. Line 254: Change `RESET` to `QK_BOOT`
2. Lines 315-317: Remove entire `fn_actions[]` array
3. Line 485: Change `biton32(layer_state)` to `get_highest_layer(layer_state)`
4. Lines 2-4: Remove unnecessary includes (debug.h, action_layer.h)
5. Verify SAFE_RANGE still correct for custom keycodes

config.h:
1. Lines 1-2: Replace `#ifndef/#define` with `#pragma once`
2. Line 4: Remove `#include QMK_KEYBOARD_CONFIG_H`
3. Line 32: Remove `PREVENT_STUCK_MODIFIERS` (now default)
4. Consider moving USB descriptors to keyboard.json (optional)

rules.mk:
1. Line 3: Remove `FORCE_NKRO = yes` (deprecated)
2. Add: `BOOTLOADER = halfkay` (assuming Teensy 2.0)
```

### 3.2 Feature Modernization
- [x] Review and update any custom macros
- [x] Modernize any deprecated features to current equivalents
- [x] Validate layer structure against current QMK standards

**Changes Made (Verified Dec 2025):**
```
keymap.c:
✅ Line 55: qk_tap_dance_action_t → tap_dance_action_t
✅ Lines 101, 148: KC_BSPACE → KC_BSPC
✅ Lines 105, 152: KC_BSLASH → KC_BSLS  
✅ Lines 106, 153: KC_SCOLON → KC_SCLN
✅ Line 316: RESET → QK_BOOT (in ARROWS layer)
✅ Removed fn_actions[] array entirely (was deprecated)
✅ Line 389: VRSN uses SEND_STRING(QMK_KEYBOARD "/" QMK_KEYMAP)
✅ Line 542: biton32(layer_state) → get_highest_layer(layer_state)
✅ Removed unnecessary includes (debug.h, action_layer.h, version.h)
✅ All custom macros (MD_LINK, XKCD, PAST_PS, TODO, etc.) verified working

config.h:
✅ Line 1: #ifndef/#define → #pragma once
✅ Removed QMK_KEYBOARD_CONFIG_H include (no longer exists)
✅ Removed PREVENT_STUCK_MODIFIERS (now default behavior)
✅ Removed custom USB descriptor overrides (conflicts with modern QMK)
✅ Kept MOUSEKEY settings with proper #undef to override board defaults
✅ Kept TAPPING_TERM 200 setting

rules.mk:
✅ Removed FORCE_NKRO = yes (deprecated May 2025)
✅ Added BOOTLOADER = halfkay (required for Teensy 2.0)
✅ Kept COMMAND_ENABLE = no and TAP_DANCE_ENABLE = yes
```

### 3.3 Testing & Validation
- [x] Verify layout compiles without errors
- [x] Test compilation with all build flags
- [x] Validate keymap logic (visually/mentally walk through layers)

**Compilation Results:**
```
✅ Firmware compiled successfully
   File: .build/ergodox_ez_base_meagerfindings.hex
   Size: 22676/32256 bytes (70%, 9580 bytes free)
   Status: Ready for flashing
```

---

## Phase 4: Firmware Flashing
**Status:** ✅ COMPLETE  
**Owner:** User (all phases completed)

### 4.1 Pre-Flash Preparation
- [x] Confirm bootloader type (HalfKay for Teensy 2.0)
- [x] Prepare hex file from compiled firmware
- [x] Test USB connection to Ergodox

**Actual Completion:**
```
✅ Bootloader confirmed: HalfKay (Teensy 2.0 ATmega32U4)
✅ Firmware hex file: ergodox_ez_base_meagerfindings.hex
✅ File size: 22676/32256 bytes (70% usage, 9580 bytes free)
✅ Location: /Users/mat/git/qmk/.build/ergodox_ez_base_meagerfindings.hex
✅ USB connection: Verified and working
```

### 4.2 Flash Execution
- [x] Enter bootloader mode on Ergodox
- [x] Run flashing command with correct parameters
- [x] Monitor for success/error messages
- [x] Verify firmware loaded

**Flash Results:**
```
✅ Command executed: qmk flash -kb ergodox_ez -km meagerfindings
✅ Bootloader mode: Successfully entered via QK_BOOT key
✅ Flash status: SUCCESS
✅ Device re-enumerated: Yes
✅ Firmware loading: Confirmed
```

### 4.3 Post-Flash Verification
- [x] Test basic key functionality (qwerty layer)
- [x] Verify all 7 layers switching
- [x] Test tap dance configurations (3 of 4 working; 1 removed)
- [x] Verify custom macros (MD_LINK, XKCD, TODO, RUBYMINE, ZENDESK, all working)
- [x] Validate mouse/media key functionality
- [x] Remove unused TD_SNAGIT macro
- [x] Skip LED testing (no LEDs on this board)

**Testing Results:**
```
Status: COMPLETE ✅
- Physical keyboard connection: Verified working
- Firmware is loaded and responding
- All tap dance keys: Working correctly (TD_COPY_CUT, CT_LBP, CT_RBP)
- All custom macros: Verified functional
- Mouse keys: Functional
- Media keys: Functional
- TD_SNAGIT: Removed entirely from code (unused - no Snagit software)
```

### 4.4 Code Cleanup
- [x] Removed TD_SNAGIT enum entry from custom_keycodes
- [x] Removed TD_SNAGIT tap dance definition from tap_dance_actions[]
- [x] Replaced TD(TD_SNAGIT) with _______ in all keymap layers
- [x] Removed related comments referencing Snagit functionality
- [x] Updated both source files (QMK repo and MeagerDox repo)

**Cleanup Results:**
```
Status: COMPLETE ✅
- TD_SNAGIT references: All removed
- Code consistency: Both repos synchronized
- Comments updated: All Snagit references cleaned out
- Ready for production use
```

---

## Current Codebase State Summary

| File | Status | Location | Notes |
|------|--------|----------|-------|
| keymap.c | ✅ Active | /Users/mat/git/qmk/keyboards/ergodox_ez/keymaps/meagerfindings/ | Modern QMK syntax, all deprecated items fixed |
| config.h | ✅ Active | /Users/mat/git/qmk/keyboards/ergodox_ez/keymaps/meagerfindings/ | Clean `#pragma once`, essential settings only |
| rules.mk | ✅ Active | /Users/mat/git/qmk/keyboards/ergodox_ez/keymaps/meagerfindings/ | HalfKay bootloader configured |
| Compiled Hex | ✅ Ready | /Users/mat/git/qmk/.build/ | ergodox_ez_base_meagerfindings.hex (22KB, ready to flash) |

**Firmware Status:**
- Compiled: ✅ Success (Dec 2025)
- Flashed: ✅ Success (HalfKay bootloader)
- Currently Running: ✅ Active and responsive

---

## Key Blockers & Decisions

### ~~Blocker 1: Confirm MCU Type~~ ✅ RESOLVED
**Confirmed:** Teensy 2.0 with HalfKay bootloader
**Setting:** `BOOTLOADER = halfkay`

### Blocker 2: Keymap Location Decision
**Options:**
1. Use `ergodox_ez/keymaps/meagerfindings/` (simpler, recommended)
2. Create `handwired/meagerdox/` (more work, more control)
**Recommendation:** Option 1 unless you need custom matrix configuration

### Decision: Keep MeagerDox as Standalone Repo?
**Current:** Standalone files not in QMK structure
**Options:**
1. Copy files into QMK repo's keymaps folder when building
2. Restructure MeagerDox to be a QMK External Userspace
**Recommendation:** Start with Option 1 for simplicity

---

## Timeline & Progress

| Phase | Estimated | Actual | Status |
|-------|-----------|--------|--------|
| Phase 1 (Research) | - | ~2 hours | ✅ COMPLETE |
| Phase 2 (Env Setup) | 15-30 min | ~30 min | ✅ COMPLETE |
| Phase 3 (Modification) | 30-60 min | ~2 hours | ✅ COMPLETE |
| Phase 4.1-4.2 (Flash) | 10-15 min | ~20 min | ✅ COMPLETE |
| Phase 4.3 (Testing) | 30-45 min | ~30 min | ✅ COMPLETE |
| Phase 4.4 (Cleanup) | 5-10 min | ~5 min | ✅ COMPLETE |

**Total Time to Flash:** ~4.5 hours
**Total Time for Full Project:** ~5.5 hours (all phases completed)

---

## Quick Reference Commands

```bash
# Install QMK
brew install qmk/qmk/qmk
qmk setup

# Check environment
qmk doctor

# Compile
qmk compile -kb ergodox_ez -km meagerfindings

# Flash (Teensy)
qmk flash -kb ergodox_ez -km meagerfindings

# List available keymaps
qmk list-keymaps -kb ergodox_ez
```

---

## Notes
- This document is the single source of truth for the project
- Phase 1 Research completed December 2025
- Update status sections as work progresses
- Flag any blockers immediately
- Hand off to next agent with all findings documented
