# SylionX — CANFD2USB

[![License: MIT](https://img.shields.io/badge/Software-MIT-blue.svg)](LICENSE-MIT.txt)
[![License: CERN OHL-S](https://img.shields.io/badge/Hardware-CERN%20OHL--S%20v2-yellow.svg)](LICENSE-CERN-OHL-S-v2.txt)

An open source USB to CAN FD adapter for real-time CAN bus monitoring, 
diagnostics, and frame injection.

## Repository Structure

| Folder | Contents | License |
|--------|----------|---------|
| `CANFD2USB_Hardware` | KiCad schematics, PCB layout, BOM, gerbers | CERN OHL-S v2 |
| `CANFD2USB_Firmware` | TI MSPM0G3507 C firmware | MIT |
| `CANtrace` | CANtrace Qt6/C++ desktop app | MIT |

## Hardware — CANFD2USB
- Microcontroller: TI MSPM0G3507
- CAN Protocol: Classic CAN + CAN FD (ISO 11898-1)
- Data Rate: Up to 5 Mbps (CAN FD)
- Interface: USB-C (USB 2.0)
- Power: USB powered (5V)
- OS: Windows (Linux planned)

## Getting Started
See [docs/quickstart.md](docs/quickstart.md) for build and setup instructions.

## License
- **Software & Firmware** — [MIT License](LICENSE-MIT.txt)
- **Hardware** — [CERN Open Hardware Licence v2 - Strongly Reciprocal](LICENSE-CERN-OHL-S-v2.txt)

## Contributing
Pull requests are welcome. See [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

---
© 2026 SylionX / Amal Raj KC
