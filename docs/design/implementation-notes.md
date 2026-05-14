# Implementation Notes

- Active default hardware target is STM8L051F3P6 with IRLML2502 as the fixed low-side NMOS.
- `../../STM8L001_CR2032_3键NEC红外遥控器完整设计方案.md` is retained as legacy/background context only where it still mentions STM8L001.
- First firmware pass is compile-first, not feature-complete.
- First hardware pass is schematic-skeleton-first, not PCB-ready.
- This migration updates documentation, hardware skeleton text, and the compile-first firmware project. Board layout, production files, complete KiCad library binding, and bench-validated behavior are not included.
- Hardware skeleton connection intent:
  - MCU IR/GPIO -> 100 Ω gate resistor -> IRLML2502 gate.
  - 1 MΩ pull-down from IRLML2502 gate to GND.
  - IR LED and R_LED connect to CR2032+.
  - IRLML2502 source connects to GND; drain sits in the LED cathode return path.
  - SWIM, NRST, VDD, and GND test/debug points are exposed for bring-up access.


## Datasheet facts frozen for this first pass

- Active MCU/package target: `STM8L051F3P6`, TSSOP-20.
- Supply class: 1.8 V to 3.6 V; CR2032 use still requires pulse-load voltage-droop validation.
- Debug access: preserve `SWIM`, `NRST`, `VDD`, and `GND` access points.
- IR option: `PA0/SWIM/BEEP/IR_TIM` is kept as the first-pass IR output candidate; if bring-up shows SWIM interference, a plain GPIO bit-bang IR output may be substituted and documented.

## IRLML2502 gate-network tradeoff

Default gate pull-down is `1 MΩ` to minimize static battery drain. A stronger value such as `100 kΩ` improves noise immunity and gate discharge during reset/halt/SWIM attach but increases leakage. The `1 MΩ` default is not bench-validated yet; verify no false IR turn-on during reset, halt, and SWIM attach.
