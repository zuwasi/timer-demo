@echo off
REM Build script for Timer application - STM32WB5x Embedded Version

set ARM_GCC=C:\arm-gcc\arm-gnu-toolchain-13.2.Rel1-mingw-w64-i686-arm-none-eabi\bin
set CC=%ARM_GCC%\arm-none-eabi-gcc.exe
set OBJCOPY=%ARM_GCC%\arm-none-eabi-objcopy.exe
set SIZE=%ARM_GCC%\arm-none-eabi-size.exe

set MCU=-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard
set CFLAGS=%MCU% -Wall -Wextra -std=c99 -g -O2 -ffunction-sections -fdata-sections -DSTM32WB55xx
set LDFLAGS=%MCU% -Tstm32wb5x_flash.ld -Wl,--gc-sections -nostartfiles --specs=nosys.specs

echo === Building Timer for STM32WB5x ===
echo.

echo Compiling startup_stm32wb55xx.c...
%CC% %CFLAGS% -c -o startup_stm32wb55xx.o startup_stm32wb55xx.c
if errorlevel 1 goto error

echo Compiling system_stm32wb5x.c...
%CC% %CFLAGS% -c -o system_stm32wb5x.o system_stm32wb5x.c
if errorlevel 1 goto error

echo Compiling clock.c...
%CC% %CFLAGS% -c -o clock.o clock.c
if errorlevel 1 goto error

echo Compiling driver.c...
%CC% %CFLAGS% -c -o driver.o driver.c
if errorlevel 1 goto error

echo Compiling timer.c...
%CC% %CFLAGS% -c -o timer.o timer.c
if errorlevel 1 goto error

echo Compiling stdinout.c...
%CC% %CFLAGS% -c -o stdinout.o stdinout.c
if errorlevel 1 goto error

echo Linking...
%CC% %LDFLAGS% -o timer_stm32wb5.elf startup_stm32wb55xx.o system_stm32wb5x.o clock.o driver.o timer.o stdinout.o
if errorlevel 1 goto error

echo Generating binary...
%OBJCOPY% -O binary timer_stm32wb5.elf timer_stm32wb5.bin
%OBJCOPY% -O ihex timer_stm32wb5.elf timer_stm32wb5.hex

echo.
echo === Build Successful ===
%SIZE% timer_stm32wb5.elf

goto end

:error
echo.
echo === Build Failed ===
exit /b 1

:end
