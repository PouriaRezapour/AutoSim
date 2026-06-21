# AutoSim — SimLink HIL Platform

**SimLink** is Hardware-in-the-Loop (HIL) firmware for the STM32H743ZI that acts as a bridge between a PC and a vehicle ECU / device under test (DUT). It simulates sensor and command signals and exchanges them with the PC over **CAN**, **Ethernet (UDP)**, and **SPI**, using a layered, AUTOSAR-inspired architecture.

- **Target MCU:** STM32H743ZITx (Nucleo-H743ZI)
- **IDE / Toolchain:** STM32CubeIDE (Eclipse CDT, GNU ARM GCC)
- **RTOS:** FreeRTOS
- **Network stack:** lwIP (UDP server, DHCP with static-IP fallback)

## Architecture

The application follows a 4-layer split similar to AUTOSAR, keeping hardware details out of application logic:

```
┌─────────────────────────────────────────────┐
│  App      SenSim (Hmi, MsgTblSim, ...)       │  Application / software components
│           DataLink (PcComMgr)                │
│           DevBench (DevTst)                  │
├─────────────────────────────────────────────┤
│  Rte      Scheduler, Rte_<Swc>.c glue code   │  Runtime Environment (SWC↔SWC, SWC↔Bsw)
├─────────────────────────────────────────────┤
│  Bsw      Can · Dio · Eth · Os · Port ·      │  Basic Software (drivers, OS abstraction,
│           Spi · Sys · Det · Isr · Mcu        │  diagnostics)
├─────────────────────────────────────────────┤
│  Mcal     STM32H7xx HAL/CMSIS, BSP,          │  Microcontroller Abstraction Layer
│           FreeRTOS, lwIP (vendored)          │  (third-party / generated code)
└─────────────────────────────────────────────┘
```

`main()` simply calls `SimLink_Init()` (brings up MCAL → BSW → App) and `SimLink_Run()` (starts the RTOS scheduler, which never returns — see `SimLink/Main/Source/SimLink.c`).

## Repository Layout

```
AutoSim/
├── Doc/                      Code templates & naming-convention reference
├── Project/                  STM32CubeIDE project (.project, .cproject, linker script, Debug config)
└── SimLink/                  Firmware source
    ├── Main/                 Entry point (main.c, SimLink_Init/Run)
    ├── App/
    │   ├── SenSim/           Sensor-simulation software components
    │   │   ├── Hmi/          User-button → accumulated value SWC
    │   │   ├── MsgTblSim/    Generic CAN message/signal table framework (+ vehicle-specific tables)
    │   │   ├── CmdSigMgr/    Command signal manager (planned)
    │   │   └── VlvSigObs/    Valve signal observer (planned)
    │   ├── DataLink/
    │   │   └── PcComMgr/     Builds/parses the PC↔target UDP protocol
    │   └── DevBench/
    │       └── DevTst/       Compile-time-gated bench tests (e.g. CAN TX burst test)
    ├── Rte/                  Generated-style glue: scheduler + per-SWC Rte_*.c/.h
    ├── Bsw/                  Can, Det, Dio, Eth, Isr, Mcu, Os, Port, Spi, Sys
    └── Mcal/                 STM32H7xx HAL, CMSIS, BSP, FreeRTOS, lwIP (vendor code)
```

## Module Map

| Layer | Module | Responsibility |
|---|---|---|
| App | `SenSim/Hmi` | Debounced button read, accumulates a float counter, exposes it via the RTE |
| App | `SenSim/MsgTblSim` | Generic `CanFrameDef_t`/`CanSignalDef_t` framework for describing CAN frames & signals; vehicle-specific table extensions (rolling counter / checksum, ECU supervision bitmask) |
| App | `SenSim/CmdSigMgr`, `SenSim/VlvSigObs` | Reserved/scaffolded, not yet implemented |
| App | `DataLink/PcComMgr` | Builds and transmits UDP packets to the PC, parses incoming commands |
| App | `DevBench/DevTst` | Optional bench-test harness, fully compiled out when `DEVTST_ENABLED = 0` |
| Rte | `Rte_Scheduler` | Periodic task dispatch at 1/5/10/20/50/100 ms plus background/Ethernet tasks |
| Bsw | `Can` | FDCAN1 driver: init, start, single-frame TX/RX (Classic CAN) |
| Bsw | `Eth` | LAN8742 PHY driver, lwIP glue, UDP server, DHCP + link-status monitoring |
| Bsw | `Dio` / `Port` | Digital I/O hardware abstraction and GPIO pin-muxing |
| Bsw | `Os` | FreeRTOS task creation and scheduler startup |
| Bsw | `Det` | Default error tracer / diagnostic reporting |
| Bsw | `Spi`, `Sys`, `Isr`, `Mcu` | SPI driver, system/clock setup, ISR routing, MPU/cache/clock bring-up |
| Mcal | `Drivers`, `Middlewares` | Vendored STM32H7xx HAL, CMSIS, BSP, FreeRTOS, lwIP |

## Communication Channels

- **CAN (FDCAN1):** Classic CAN frames, initialized and started during `SimLink_Init()`.
- **Ethernet (UDP):** lwIP-based UDP server with a configurable static IP fallback and DHCP client; used to exchange the SWC-level signals (e.g. button value) and receive commands from the PC.
- **SPI:** Driver scaffolding present in `Bsw/Spi`; wiring into the application layer is still in progress.

## Getting Started

### Prerequisites
- [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html)
- A Nucleo-H743ZI/H753ZI board (ST-Link on-board)
- USB cable, and an Ethernet/CAN connection to the PC or DUT depending on which channel you're testing

### Build & Flash
1. Clone the repository.
2. In STM32CubeIDE: **File → Import → Existing Projects into Workspace**, select the `Project` directory (contains `.project` / `.cproject`).
3. Build the `Debug` configuration.
4. Connect the Nucleo board via USB and flash using the built-in ST-Link debugger (or the included `AutoSim.launch` debug configuration).

### Network Configuration
The board's default static IP and the PC's expected IP are defined in `Bsw/Eth/Include/Eth_Config.h`. Update these to match your PC's network setup before connecting over Ethernet.

## Project Status

This is an actively developed project (modules are individually versioned in their file headers). Notable in-progress areas:
- `CmdSigMgr` and `VlvSigObs` SenSim modules are scaffolded but not yet implemented.
- SPI is wired up at the driver level but not yet integrated into a software component.

## Coding Conventions

`Doc/Templates/` contains the canonical file/module templates and naming-convention reference used throughout the codebase (`Code Template/`, `Naming Conventions/`). New modules should follow these for header structure, port-naming (`Pp`/`Rp` prefixes), and revision-history tables.

## Third-Party Components

`SimLink/Mcal` vendors generated/third-party code, each under its own license:
- **STM32H7xx HAL Driver / CMSIS / BSP** — STMicroelectronics
- **FreeRTOS** — MIT License
- **lwIP** — modified BSD License

## Author

A. Rezapour (Pouria)
