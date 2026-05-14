# STM8L051F3P6 + IRLML2502 Migration Notes

## Active default

The active default hardware target is:

- MCU: `STM8L051F3P6`
- IR low-side NMOS: `IRLML2502`
- Battery: CR2032

STM8L001 references in older project material are legacy/background context only.

## Hardware skeleton intent

- MCU `IR/GPIO` output drives a `100 Ω` series gate resistor.
- The `100 Ω` resistor drives the IRLML2502 gate.
- A `1 MΩ` resistor pulls the IRLML2502 gate down to `GND`.
- `R_LED` and the IR LED connect to `CR2032+`.
- The IRLML2502 drain is in the IR LED cathode return path.
- The IRLML2502 source connects to `GND`.
- Bring-up/debug access exposes `SWIM`, `NRST`, `VDD`, and `GND` test points.

## Scope boundaries

- Documentation, schematic skeleton text, and compile-first firmware target migration.
- No PCB layout or production firmware validation.
- No production package.
- No complete KiCad symbol/footprint library.
- Not yet bench tested; no measured waveform, current, distance/range, or power-consumption claims are made.


## First-pass component BOM

This BOM is for schematic review and bring-up planning only. It is not a production purchasing package, and manufacturer part numbers for generic passives/connectors may be substituted after PCB footprint selection.

| Ref | Qty | Component | Recommended value / part | Package / footprint intent | Key requirements / notes |
|---|---:|---|---|---|---|
| U1 | 1 | MCU | `STM8L051F3P6` | TSSOP-20 | Active default MCU. Preserve `SWIM`, `NRST`, `VDD`, `GND` access. 1.8 V to 3.6 V supply class; CR2032 pulse droop must be validated. |
| Q1 | 1 | N-MOS low-side IR switch | `IRLML2502` | SOT-23 | Fixed NMOS choice. Logic-level gate drive suitable for low-voltage MCU output. Drain in IR LED cathode return path, source to GND. |
| D1 | 1 | IR LED | 940 nm IR LED, high-radiant-intensity type | 5 mm through-hole or SMD high-power IR LED, TBD by mechanical/PCB pass | Choose viewing angle and radiant intensity during mechanical/optical design. Not bench-validated yet. |
| R_LED | 1 | IR LED current-limit resistor | Default placeholder `10 Ω` to `22 Ω` | 0603/0805 or pulse-rated part per layout | Final value must be set by measured LED current, CR2032 droop, range, and pulse thermal limits. |
| R_GATE | 1 | MOSFET gate resistor | `100 Ω` | 0402/0603 | Series resistor from MCU IR/GPIO to IRLML2502 gate; `47 Ω` to `220 Ω` acceptable during tuning. |
| R_PD | 1 | MOSFET gate pull-down | `1 MΩ` | 0402/0603 | Holds Q1 off in reset/halt/debug. `100 kΩ` is a stronger but leakier alternative if noise/reset discharge requires it. |
| SW1, SW2, SW3 | 3 | User keys | Momentary tactile switch, normally open | SMD tact switch, footprint TBD | Active-low to GND. Firmware assumes internal pull-ups and debounce. |
| BT1 | 1 | Battery holder | CR2032 holder | Holder footprint TBD | Verify polarity marking, retention, and battery access mechanically. |
| C1 | 1 | MCU local decoupling | `100 nF`, X7R/X5R | 0402/0603 close to VDD/VSS | Mandatory local decoupling for U1. |
| C2 | 1 | Bulk capacitor | `4.7 µF` to `10 µF`, low leakage | 0603/0805 | Supports MCU and moderate load transients; value depends on PCB and battery impedance. |
| C3 | 0/1 | Optional IR burst reservoir | `47 µF` to `100 µF`, low leakage | 1206/tantalum/polymer/electrolytic TBD | Optional. Helps IR burst current but increases size/leakage/inrush; validate with CR2032. |
| TP_SWIM | 1 | Test point | SWIM | Pad/test point | Programming/debug access. |
| TP_NRST | 1 | Test point | NRST | Pad/test point | Reset/debug recovery access. |
| TP_VDD | 1 | Test point | VDD | Pad/test point | Power measurement/programming reference. |
| TP_GND | 1 | Test point | GND | Pad/test point | Ground reference for programming and scope probing. |
| J1 | 0/1 | Optional programming header | SWIM/NRST/VDD/GND | 1.27 mm/2.54 mm header or pogo pads | Optional alternative to discrete test pads; not required for this skeleton. |

### BOM constraints and substitutions

- Do not substitute Q1 with `2N7002` or `BSS138` as the default IR switch unless low-voltage pulse-current performance is revalidated.
- Generic passives may use ±5% resistors and X5R/X7R capacitors unless leakage, size, or pulse behavior requires tighter control.
- `R_LED`, `C2`, and `C3` are validation-sensitive and should be finalized only after oscilloscope/current/range testing.
- This table intentionally omits vendor order codes and production alternates; those belong in a later manufacturing package.

## Firmware migration status

- PlatformIO active environment: `stm8l051f3p6`.
- Custom board: `firmware/boards/stm8l051f3p6_custom.json`.
- Active register shim: `firmware/include/stm8l_regs.h`; old target-specific `stm8l001_stub.h` is not an active include.
- NEC constants remain `0x80`, `0x7F`, `0xC0`, `0x40`, and `0x60`.
- Bit-bang NEC timing is compile-first only and must be scope/receiver verified.

## Datasheet and validation facts

- `STM8L051F3P6` is treated as the active TSSOP-20 target.
- 1.8 V to 3.6 V supply class is compatible with CR2032 only after pulse-load validation.
- `SWIM`, `NRST`, `VDD`, and `GND` must remain accessible during bring-up.
- `PA0/SWIM/BEEP/IR_TIM` is the first-pass IR output candidate; this is a documented debug tradeoff, not a bench-proven final assignment.
