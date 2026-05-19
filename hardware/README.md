# Hardware Design

Complete KiCad schematic and PCB for the STM8L051F3P6 + IRLML2502 + CR2032 NEC IR remote.

## Files
- `kicad/remote.kicad_pro` — KiCad project with design settings and net classes
- `kicad/remote.kicad_sch` — root schematic (page 1) with hierarchical sheets and PWR_FLAG
- `kicad/mcu_keys.kicad_sch` — MCU + keys + debug (page 2): full 20-pin U1, pull-down resistors, test points, switches
- `kicad/power_ir.kicad_sch` — power + IR driver (page 3): CR2032, decoupling, IRLML2502 driver, R1 current limiter
- `kicad/remote.kicad_pcb` — PCB layout with all footprints, routing, mounting holes, GND zones
- `kicad/top.kicad_sch.legacy-bak` — legacy note page (archived, not part of active hierarchy)
- `kicad/lib/irtool_symbols.kicad_sym` — project-local symbol library (U1, Q1, D1, BT1, R, C, CP, SW, TP, PWR_FLAG)
- `schematic-structure.md` — authoritative block/net/BOM mapping

## Active default hardware target
- MCU: STM8L051F3P6 (TSSOP-20), all 20 pins connected
- IR driver: IRLML2502 NMOS low-side switch with 100 Ω gate resistor and 1 MΩ gate pull-down
- IR LED current limiting: R1 (10–22 Ω) from VDD_MCU to IR LED anode
- Battery: CR2032 holder on back copper layer
- Unused MCU pins: tied to GND via 11 × 100 K pull-down resistors (RD1–RD11)
- Debug access: 5 test points (SWIM, NRST, VDD, GND, DBG) along board top edge
- User input: 3 tactile switches (KEY1, KEY2, KEY3), active-low to GND
- PCB: 170 × 75 mm, 2 mm corner radii, 4 × M2 mounting holes, GND zones on both layers
- Net classes: Default (0.25 mm signal) and Power (0.5 mm for VDD_MCU, GND, IR_LED)

## Design hierarchy
```
remote.kicad_sch (page 1, root)
├── mcu_keys.kicad_sch (page 2) — MCU, keys, debug, pull-downs
└── power_ir.kicad_sch (page 3) — battery, decoupling, IR driver
```

## Key connection intent
1. CR2032+ → VDD_MCU rail → U1 VDD, C1, C2, C3, R1
2. PA0/IR_GPIO → 100 Ω R2 → IRLML2502 gate (R_PD 1 MΩ to GND)
3. IRLML2502 drain → IR LED cathode (IR_LED_K)
4. VDD_MCU → R1 (10–22 Ω) → IR LED anode (IR_LED_A)
5. IRLML2502 source → GND
6. PB0/PB1/PD0 → KEY1_N/KEY2_N/KEY3_N → tactile switches → GND

## Editing intent
- Open `remote.kicad_pro` as the project entry
- Use `irtool_symbols` library for all project-specific symbols
- Footprint names use `irtool:` prefix — replace with KiCad standard library footprints when finalizing
- This is a first-pass engineering asset, not a production release
