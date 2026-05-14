# STM8L051F3P6 CR2032 NEC Remote

First-pass engineering assets for the active default STM8L051F3P6 + IRLML2502 + CR2032 + 3-key NEC remote project.

## Active default hardware target
- MCU: `STM8L051F3P6`
- IR low-side NMOS: `IRLML2502`
- Power source: CR2032
- IR driver intent: MCU IR/GPIO output drives `100 Ω` gate series resistor into the IRLML2502 gate; a `1 MΩ` gate pull-down holds the MOSFET off; the IR LED and `R_LED` connect to `CR2032+`, with the IRLML2502 source at `GND` and drain in the LED cathode return path.
- Debug/test access intent: expose `SWIM`, `NRST`, `VDD`, and `GND` test/debug points.

Legacy/background material may still mention STM8L001. Those references are retained only as prior design context unless explicitly updated to the active default above.

## Repository layout
- `hardware/` - editable schematic skeleton and hardware notes
- `firmware/` - compile-first PlatformIO project migrated to the STM8L051F3P6 custom target
- `docs/` - design and bring-up notes
- `.vscode/` - recommended VSCode workspace configuration

## First-pass scope
- Editable schematic skeleton
- Buildable PlatformIO project
- VSCode workspace support
- Git-friendly repository structure

## Out of scope for this pass
- PCB layout
- Manufacturing package
- Bench bring-up evidence
- CI/CD
- Production-ready symbol/footprint library completion

## Build firmware
From the repository root:

```sh
. .venv-pio/bin/activate
cd firmware
pio run
```

## Key references
- `docs/design/stm8l051f3p6-irlml2502-migration.md`
- `.omx/plans/prd-stm8l051f3p6-irlml2502-migration.md`
- Legacy/background: `STM8L001_CR2032_3键NEC红外遥控器完整设计方案.md`
