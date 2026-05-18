# Hardware Skeleton

This directory contains an editable KiCad-style schematic skeleton for the active default STM8L051F3P6 + IRLML2502 remote design.

STM8L001 references, if encountered in older notes, are legacy/background context and are not the current hardware target.

## Files
- `kicad/remote.kicad_pro` - project shell
- `kicad/remote.kicad_sch` - root schematic opened by KiCad project preview
- `kicad/sym-lib-table` - project-local KiCad symbol library binding
- `kicad/lib/irtool_symbols.kicad_sym` - minimal local symbol library for this first-pass design
- `kicad/top.kicad_sch` - legacy top-level note page retained for review/reference
- `kicad/power_ir.kicad_sch` - CR2032, decoupling, IR driver block shell
- `kicad/mcu_keys.kicad_sch` - STM8L051F3P6 core, keys, SWIM/NRST/debug block shell
- `schematic-structure.md` - structured schematic mapping for continued editing

## Active default connection intent
- MCU `IR/GPIO` output -> `100 Ω` gate resistor -> IRLML2502 gate.
- `1 MΩ` gate pull-down from IRLML2502 gate to `GND`.
- IR LED plus `R_LED` connect from `CR2032+` to the MOSFET-controlled cathode path.
- IRLML2502 source connects to `GND`; drain connects to the IR LED cathode return path.
- Provide test/debug points for `SWIM`, `NRST`, `VDD`, and `GND`.

## Scope
- Net and block organization only
- Includes only the necessary local symbols for this project pass; not a full library pack
- Not PCB-ready
- Not a production-ready PCB or complete library deliverable

## Editing intent
- Continue editing these files in KiCad or replace them with fuller KiCad content in later passes.
- Treat the Markdown structure file as the authoritative mapping of blocks, nets, and deferred details for this first pass.
- Use the project-local `irtool_symbols` library as the default symbol source for U1/Q1/BT1/D1/SW/TP and simple passives.
- Open `remote.kicad_pro` / `remote.kicad_sch` as the project entry; the root sheet now references `mcu_keys.kicad_sch` and `power_ir.kicad_sch`.
