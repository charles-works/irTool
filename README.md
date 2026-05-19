# STM8L051F3P6 CR2032 NEC Remote

Complete first-pass engineering assets for the STM8L051F3P6 + IRLML2502 + CR2032 + 3-key NEC IR remote project.

## Active hardware target
- **MCU**: STM8L051F3P6 (TSSOP-20), all 20 pins connected
- **IR driver**: IRLML2502 NMOS low-side switch with 100 Ω gate resistor, 1 MΩ gate pull-down
- **IR LED**: 940 nm IR LED with R1 (10–22 Ω) current limiter from VDD_MCU
- **Power**: CR2032 coin cell holder on back copper layer
- **Keys**: 3 tactile switches (KEY1/KEY2/KEY3), active-low to GND
- **Debug**: 5 test points (SWIM, NRST, VDD, GND, DBG) along board top edge
- **Unused pins**: All tied to GND via 11 × 100 K pull-down resistors (RD1–RD11)
- **PCB**: 170 × 75 mm, 2 mm corner radii, 4 × M2 mounting holes, dual-layer GND zones

## Repository layout
- `hardware/` — KiCad schematic (hierarchical, 3 pages) + PCB layout + symbol library
- `firmware/` — PlatformIO project for STM8L051F3P6 custom target
- `docs/` — design and bring-up notes
- `.vscode/` — recommended VSCode workspace configuration

## Hardware files
| File | Description |
|------|-------------|
| `hardware/kicad/remote.kicad_pro` | KiCad project with design settings and net classes |
| `hardware/kicad/remote.kicad_sch` | Root schematic (page 1): hierarchy + PWR_FLAG |
| `hardware/kicad/mcu_keys.kicad_sch` | MCU + keys + debug (page 2): full 20-pin U1, pull-downs, switches, TPs |
| `hardware/kicad/power_ir.kicad_sch` | Power + IR driver (page 3): CR2032, decoupling, IRLML2502, R1 |
| `hardware/kicad/remote.kicad_pcb` | PCB layout: 22 footprints, 31 traces, GND zones, mounting holes |
| `hardware/kicad/lib/irtool_symbols.kicad_sym` | Project-local symbol library |
| `hardware/schematic-structure.md` | Authoritative block/net/BOM mapping |

## Build firmware
```sh
. .venv-pio/bin/activate
cd firmware
pio run
```

## Key references
- `hardware/schematic-structure.md` — complete schematic mapping
- `hardware/README.md` — hardware design details
- `docs/design/stm8l051f3p6-irlml2502-migration.md`
- `.omx/plans/prd-stm8l051f3p6-irlml2502-migration.md`

## Remaining work
- R1 final resistance value (bench LED current / range validation)
- C3 necessity (CR2032 pulse-load testing)
- Replace `irtool:` footprint prefixes with KiCad standard library names
- Manufacturing package / Gerber generation
- Enclosure / mechanical integration
