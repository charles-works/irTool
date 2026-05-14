# STM8L051F3P6 + IRLML2502 Bring-up Checklist

Status: not yet tested.

The active default hardware target is STM8L051F3P6 + IRLML2502. This pass only documents the intended hardware skeleton and debug access points.

Bench bring-up is still pending. Do not treat this repository as claiming measured waveform quality, LED current, IR distance/range, CR2032 load behavior, sleep current, or total power consumption.

Initial bring-up access expected from the skeleton:
- `SWIM`
- `NRST`
- `VDD`
- `GND`

Out of scope for this pass:
- PCB layout or production release package
- Complete symbol/footprint library
- Production firmware validation
- Any measured performance claims


## Firmware smoke check

Run before bench work:

```sh
. ../.venv-pio/bin/activate
cd firmware
pio run
```

Expected build target: `stm8l051f3p6`. Passing build only proves compile/link success; it does not prove carrier timing, NEC decode, current, range, or sleep current.

## Bench checklist, not yet passed

- Confirm SWIM/NRST attach and recovery after flashing.
- Scope IRLML2502 gate waveform and 38 kHz carrier period.
- Decode KEY1/KEY2/KEY3 as `80 7F C0 3F`, `80 7F 40 BF`, and `80 7F 60 9F`.
- Measure CR2032 voltage droop and IR LED peak current with the selected `R_LED`.
- Confirm gate remains off during reset, halt, and SWIM attach with the `1 MΩ` pull-down.
- Measure sleep current after key release.
