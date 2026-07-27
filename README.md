# AjayOS v3

> A distributed operating system prototype for AVR microcontrollers.

![Status](https://img.shields.io/badge/Status-Active-success)
![Platform](https://img.shields.io/badge/Platform-Arduino_Uno_&_Nano-blue)
![Language](https://img.shields.io/badge/Language-C++-orange)
![License](https://img.shields.io/badge/License-MIT-green)

---

## Overview

AjayOS is a distributed operating system prototype built from scratch for AVR microcontrollers.

Unlike traditional Arduino projects, AjayOS explores real operating system concepts including:

- Kernel Design
- Command Line Interface (CLI)
- Virtual Memory
- Paging
- Swapping
- Distributed Processing
- Inter-Processor Communication (IPC)

The primary objective of this project is educational—to understand how modern operating systems work by implementing their core concepts on resource-constrained hardware.

---

## Features

### Implemented

- Interactive Shell
- Modular Kernel
- CPU Manager
- IPC over I²C
- Virtual Memory
- Paging
- EEPROM-backed Swapping
- Distributed Multi-Processor Support
- Memory Statistics
- Boot System

### Planned

- Filesystem
- Process Scheduler
- Multitasking
- AntiC Runtime
- Networking
- Graphics Processor
- Device Drivers

---

## System Architecture

                    User
                      │
                 Shell (CLI)
                      │
                 AjayOS Kernel
       ┌──────────┬──────────┬──────────┐
       │ CPU Mgr  │ IPC Mgr  │ VM Mgr   │
       └──────────┴──────────┴──────────┘
                      │
                   I²C Bus
                      │
          ┌───────────┴───────────┐
          │                       │
     Worker CPU             Future Services

---

## Hardware

| Component | Purpose |
|-----------|----------|
| Arduino Uno | Kernel Processor |
| Arduino Nano | Worker Processor |
| EEPROM | Swap Storage |
| I²C | Inter-Processor Communication |

---

## Implemented Operating System Concepts

- Kernel
- Shell
- Memory Management
- Virtual Memory
- Paging
- Swapping
- IPC
- Distributed Processing
- CPU Management

---

## Commands

| Command | Description |
|----------|-------------|
| help | List commands |
| cpu | Display CPU information |
| ping | Test IPC |
| vm | Display virtual memory |
| echo | Print text |
| clear | Clear terminal |

---

## Project Structure

```text
kernel/
memory/
ipc/
cpu/
shell/
commands/
protocol/
```

---

## Building

Requirements

- Arduino IDE
- Arduino Uno
- Arduino Nano
- Wire Library

Upload the kernel firmware to the Uno and the worker firmware to the Nano.

---

## Roadmap

- [x] Kernel
- [x] Shell
- [x] IPC
- [x] Virtual Memory
- [x] Paging
- [x] Swapping
- [ ] Filesystem
- [ ] Process Scheduler
- [ ] AntiC Runtime
- [ ] Networking

---

## License

MIT License

---

## Author

**Ajay B**

Computer Science Engineering Student

Building operating systems from scratch for fun.