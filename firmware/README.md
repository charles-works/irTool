# Firmware

Compile-first PlatformIO project for the STM8L051F3P6 CR2032 NEC remote.

## Active target
- MCU: STM8L051F3P6, custom PlatformIO board `stm8l051f3p6_custom`
- Supply: CR2032, 1.8 V..3.6 V class; pulse droop must be bench-validated
- IR driver: PA0/SWIM/BEEP/IR_TIM candidate output to IRLML2502 low-side MOSFET
- NEC codes: KEY1 `80 7F C0 3F`, KEY2 `80 7F 40 BF`, KEY3 `80 7F 60 9F`

## Build

```sh
. ../.venv-pio/bin/activate
pio run
```

## Implementation notes
- Frameworkless C is kept for compile reliability.
- NEC timing is first-pass bit-bang and is not bench-calibrated yet.
- Key wake/halt current and SWIM/NRST attach behavior must be verified on hardware.
- Historical STM8L001 files were the previous target and are not the active default.
