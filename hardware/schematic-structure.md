# Structured Schematic Mapping

Active default: STM8L051F3P6 MCU with IRLML2502 low-side NMOS IR driver.

## Top-level blocks

1. **MCU + Debug (page 2: mcu_keys.kicad_sch)**
   - U1: STM8L051F3P6 — full 20-pin TSSOP-20 symbol with all pins connected
   - C4: 100 nF local decoupling (close to VDD/VSS on this sheet)
   - RD1–RD11: 100 K pull-down resistors for all unused GPIO pins (PC7, PA2, PA3, PB7, PB2–PB6, PC4, PC5)
   - Test/debug points: TP1=SWIM/PA0/IR, TP2=NRST, TP3=VDD, TP4=GND, TP5=DBG/PC1
   - Key switches: SW1→PB0/KEY1_N, SW2→PB1/KEY2_N, SW3→PD0/KEY3_N
   - Legal pin map: PA0=IR, PB0=KEY1, PB1=KEY2, PD0=KEY3, PC1=strap/spare

2. **Power + IR Driver (page 3: power_ir.kicad_sch)**
   - BT1: CR2032 holder
   - C1: 100 nF mandatory MCU decoupling
   - C2: 4.7 µF bulk decoupling
   - C3: 47–100 µF optional IR burst reservoir (polarized)
   - R2 (R_GATE): 100 Ω series gate resistor (IR_GPIO → Q1 gate)
   - R_PD: 1 MΩ gate-to-GND pull-down (holds Q1 off in reset/halt)
   - Q1: IRLML2502 SOT-23 NMOS low-side switch
   - R1: 10–22 Ω IR LED current limiter (VDD_MCU → IR_LED_A)
   - D1: 940 nm IR LED (anode from R1, cathode to Q1 drain)

3. **Root schematic (page 1: remote.kicad_sch)**
   - Hierarchical sheet references to MCU_Keys_Debug and Power_IR_Driver
   - Inter-sheet wires: VDD_MCU, GND, IR_GPIO
   - PWR_FLAG symbols on VDD_MCU and GND for ERC compliance

## KiCad root-page structure

- Root file: `hardware/kicad/remote.kicad_sch`
- Child sheet 1: `hardware/kicad/mcu_keys.kicad_sch` (page 2)
- Child sheet 2: `hardware/kicad/power_ir.kicad_sch` (page 3)
- `hardware/kicad/top.kicad_sch` — legacy review note page (not the project root)

## Net names

| Net name | Description |
|----------|-------------|
| `VDD_MCU` | Battery positive rail (CR2032+), MCU VDD, decoupling, R1 input |
| `GND` | Battery negative rail (CR2032-), all VSS pins, switch returns, MOSFET source |
| `IR_GPIO` | MCU PA0 output → R_GATE input; also TP1/SWIM node |
| `IR_GATE` | R_GATE output → Q1 gate → R_PD |
| `IR_LED_A` | R1 output → D1 anode |
| `IR_LED_K` | D1 cathode → Q1 drain |
| `KEY1_N` | U1 PB0 → SW1 |
| `KEY2_N` | U1 PB1 → SW2 |
| `KEY3_N` | U1 PD0 → SW3 |
| `NRST_TP` | U1 NRST/PA1 → TP2 |
| `SWIM_TP` | TP1 (same node as IR_GPIO on PCB) |
| `DBG_STRAP` | U1 PC1 → TP5 |

## BOM mapping

| Ref | Qty | Function | Value / part | Notes |
|---|---:|---|---|---|
| U1 | 1 | MCU | STM8L051F3P6 | TSSOP-20; all 20 pins connected |
| Q1 | 1 | IR low-side switch | IRLML2502 | SOT-23 N-MOS |
| D1 | 1 | IR emitter | 940 nm IR LED | 5mm THT |
| R1 | 1 | IR LED current limit | 10–22 Ω | 0603; final value needs bench validation |
| R2 | 1 | Gate series resistor | 100 Ω | 0603; 47–220 Ω acceptable |
| R_PD | 1 | Gate pull-down | 1 MΩ | 0603; 100 K alternative if stronger off-state needed |
| RD1–RD11 | 11 | Unused pin pull-down | 100 K | 0402; prevent floating GPIO on unused MCU pins |
| SW1–SW3 | 3 | User keys | Tact switch | Active-low to GND |
| BT1 | 1 | Battery holder | CR2032 | Back-side SMD holder |
| C1 | 1 | MCU decoupling | 100 nF | 0402; close to VDD/VSS |
| C2 | 1 | Bulk decoupling | 4.7 µF | 0603; low leakage |
| C3 | 0/1 | Optional IR reservoir | 47–100 µF | Validate leakage and pulse benefit |
| C4 | 1 | MCU local decoupling | 100 nF | 0402; on mcu_keys sheet near U1 |
| TP1–TP5 | 5 | Test points | SMD pads | SWIM, NRST, VDD, GND, DBG |
| H1–H4 | 4 | Mounting holes | M2 (2.2 mm) | At board corners |

## PCB skeleton status

- Board: 170 mm × 75 mm with 2 mm rounded corners
- Full 20-pin TSSOP-20 footprint for U1 with all pads assigned to correct nets
- All component footprints placed with correct net assignments
- Signal traces (0.25 mm) and power traces (0.5 mm) routed
- GND zone fills on both F.Cu and B.Cu layers
- 4 × M2 mounting holes at board corners
- Net classes: Default (0.25 mm) and Power (0.5 mm) for VDD_MCU/GND/IR_LED nets
- CR2032 holder on back copper layer
- Test points grouped along top edge near MCU

## Deferred details

- Final R1 resistance value (requires bench LED current / range validation)
- C3 necessity (validate with CR2032 pulse-load testing)
- Exact footprint library names (currently placeholder `irtool:` prefixes)
- Manufacturing package and gerber generation
- Enclosure / mechanical integration
- Bench-measured waveform, current, distance, or power claims
