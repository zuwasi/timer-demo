# Timer Application - STM32WB5x Embedded Version

This is the embedded version of the Timer application, ported to run on the STM32WB55 microcontroller (ARM Cortex-M4).

## Target Hardware

- **MCU**: STM32WB55 (Cortex-M4, 64MHz, 1MB Flash, 256KB SRAM)
- **UART**: USART1 on PA9 (TX) and PA10 (RX) at 115200 baud

## Building

### Prerequisites

- ARM GCC Toolchain (arm-none-eabi-gcc)
  - Automatically downloaded to `C:\arm-gcc` if using the build script
  - Version: 13.2.1 (Arm GNU Toolchain 13.2.rel1)

### Build Commands

**Windows:**
```batch
build_embedded.bat
```

**Or using Make:**
```batch
make -f Makefile.embedded
```

### Output Files

- `timer_stm32wb5.elf` - Debug ELF file
- `timer_stm32wb5.hex` - Intel HEX for programming
- `timer_stm32wb5.bin` - Raw binary for programming

## Memory Usage

```
   text    data     bss     dec     hex filename
   3716     316    3984    8016    1f50 timer_stm32wb5.elf
```

- **Flash**: ~4KB
- **RAM**: ~4KB

## Changes from Desktop Version

### I/O Layer
- Replaced `stdin/stdout` with UART1 communication
- Uses custom `uart_putchar()`, `uart_getchar_blocking()`, `uart_puts()`
- Echo characters back to terminal for user feedback

### Timer Structure
- Replaced `time_t` with individual `uint8_t` fields for hour/minute
- Removed dependency on `<time.h>`

### Memory Management
- Static allocation instead of `malloc()`
- Pre-allocated array of timer records
- No heap usage

### String Functions
- Custom `simple_atoi()` implementation
- Custom `embedded_memset()` implementation
- No dependency on `<stdlib.h>` or `<string.h>`

## Flashing

### Using ST-Link (STM32CubeProgrammer)
```batch
STM32_Programmer_CLI -c port=SWD -w timer_stm32wb5.bin 0x08000000 -v -rst
```

### Using OpenOCD
```bash
openocd -f interface/stlink.cfg -f target/stm32wbx.cfg \
  -c "program timer_stm32wb5.elf verify reset exit"
```

## Usage

1. Connect a USB-to-UART adapter to PA9 (TX) and PA10 (RX)
2. Open a terminal at 115200 baud, 8N1
3. Reset the board
4. Use the menu to add, remove, and list timers

## Files

| File | Description |
|------|-------------|
| `startup_stm32wb55xx.c` | Vector table and reset handler |
| `system_stm32wb5x.c` | System initialization, UART functions |
| `stm32wb5x.h` | Register definitions |
| `stm32wb5x_flash.ld` | Linker script |
| `build_embedded.bat` | Windows build script |
| `Makefile.embedded` | Makefile for ARM GCC |
