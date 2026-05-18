# Structured Schematic Mapping

Active default: STM8L051F3P6 MCU with IRLML2502 low-side NMOS IR driver. STM8L001 is legacy/background context only.

## Top-level blocks

1. **MCU + Debug**
   - U1: STM8L051F3P6
   - MCU IR/GPIO output routes to the IR driver gate network
   - Test/debug points: SWIM, NRST, VDD, GND
   - Legal pin map for this pass: PA0=IR, PB0=KEY1, PB1=KEY2, PD0=KEY3, PC1=strap/spare

2. **Keys**
   - SW1 -> PB0 / KEY1_N
   - SW2 -> PB1 / KEY2_N
   - SW3 -> PD0 / KEY3_N
   - Internal pull-up strategy assumed in firmware

3. **Power**
   - BT1: CR2032 holder
   - C1/C2 mandatory decoupling
   - C3 optional IR burst reservoir

4. **IR Driver**
   - Q1: IRLML2502 NMOS low-side switch
   - D1: 940 nm IR LED
   - R1 / R_LED: LED current limit from CR2032+ to the IR LED anode path
   - R_GATE: 100 Ω from MCU IR/GPIO to IRLML2502 gate
   - R_PD: 1 MΩ from IRLML2502 gate to GND
   - IRLML2502 source: GND
   - IRLML2502 drain: IR LED cathode return path

## KiCad root-page structure

- KiCad project preview root file: `hardware/kicad/remote.kicad_sch`
- Child sheet 1: `hardware/kicad/mcu_keys.kicad_sch`
- Child sheet 2: `hardware/kicad/power_ir.kicad_sch`
- `hardware/kicad/top.kicad_sch` is retained only as a legacy review note page and is not the project root anymore.

## First-pass BOM mapping

| Ref | Qty | Function | Value / part | Notes |
|---|---:|---|---|---|
| U1 | 1 | MCU | STM8L051F3P6 | TSSOP-20 active default; expose SWIM/NRST/VDD/GND. |
| Q1 | 1 | IR low-side switch | IRLML2502 | SOT-23 N-MOS; source GND, drain IR LED cathode return. |
| D1 | 1 | IR emitter | 940 nm IR LED | Optical/mechanical selection deferred. |
| R_LED | 1 | IR LED current limit | 10 Ω to 22 Ω placeholder | Final value requires current/range/CR2032 droop validation. |
| R_GATE | 1 | Gate series resistor | 100 Ω | MCU IR/GPIO to Q1 gate. |
| R_PD | 1 | Gate pull-down | 1 MΩ | 100 kΩ alternative if stronger off-state/noise margin is needed. |
| SW1-SW3 | 3 | Keys | Normally-open tactile switches | Active-low to GND; internal pull-ups in firmware. |
| BT1 | 1 | Battery holder | CR2032 holder | Footprint/mechanical details deferred. |
| C1 | 1 | Local decoupling | 100 nF | Close to U1 VDD/VSS. |
| C2 | 1 | Bulk decoupling | 4.7 µF to 10 µF | Low leakage preferred. |
| C3 | 0/1 | Optional IR reservoir | 47 µF to 100 µF | Optional; validate leakage and pulse benefit. |
| TP_SWIM, TP_NRST, TP_VDD, TP_GND | 4 | Debug/test points | Pads or pogo pads | Programming, reset recovery, and measurement access. |

## Suggested net names
- `VBAT`
- `CR2032_PLUS`
- `GND`
- `VDD_MCU`
- `KEY1_N`
- `KEY2_N`
- `KEY3_N`
- `IR_GPIO`
- `IR_GATE`
- `IR_LED_A`
- `IR_LED_K`
- `SWIM_TP`
- `NRST_TP`
- `VDD_TP`
- `GND_TP`

## Local symbol library binding
- Project-local symbol library: `hardware/kicad/lib/irtool_symbols.kicad_sym`
- Project symbol table entry: `hardware/kicad/sym-lib-table`
- Intended first-pass symbol usage:
  - `irtool_symbols:STM8L051F3P6` for U1
  - `irtool_symbols:IRLML2502` for Q1
  - `irtool_symbols:CR2032_Holder` for BT1
  - `irtool_symbols:IR_LED` for D1
  - `irtool_symbols:SW_Push` for SW1-SW3
  - `irtool_symbols:TestPoint` for TP_SWIM / TP_NRST / TP_VDD / TP_GND
  - `irtool_symbols:R`, `irtool_symbols:C`, `irtool_symbols:CP` for first-pass passives

## Deferred details
- Exact footprint binding
- PCB-only electrical/mechanical refinements
- Any manufacturing annotations
- Bench-measured waveform, current, distance, or power claims

## PCB skeleton status
- KiCad board file added: `hardware/kicad/remote.kicad_pcb`
- Current content is a review/edit skeleton only:
  - board outline
  - primary nets aligned to schematic intent
  - key component placement placeholders for U1/Q1/BT1/D1/R/C/SW/TP
- Not yet a finished placement/routing result and not a production release layout.
