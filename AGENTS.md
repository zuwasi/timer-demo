# AGENTS.md - Timer Demo Linux Project

## Project Overview
This is a Linux port of the Timer Demo application - a simple console-based timer recording system that allows users to schedule recording timers with start/end times and channels.

## Build Commands

### Make (Preferred for Linux):
```bash
make              # Build the project
./timer           # Run the executable
make clean        # Clean build artifacts
```

### CMake (Cross-platform):
```bash
mkdir build
cd build
cmake ..
cmake --build .
./timer          # or timer.exe on Windows
```

### WSL Build (on Windows):
```bash
wsl make
```

## Testing
Currently no automated tests. Manual testing via interactive menu system.

## Code Style & Conventions
- **Language**: C (C99 standard)
- **Naming**: snake_case for functions and variables
- **Indentation**: 4 spaces
- **Comments**: Use C-style block comments `/* */` for function descriptions
- **Headers**: Include guards in all .h files
- **Memory Management**: Always free allocated memory (see `uninit_timer()`)

## Project Structure
```
timer_Linux/
├── clock.c/h       # Time display functions
├── timer.c/h       # Timer record management
├── driver.c        # Main program loop and menu
├── stdinout.c      # Input/output utilities
├── inout.h         # I/O interface
├── consts.h        # Constants (BUF_SIZE, ERROR_CODE)
├── Makefile        # Linux/Unix build
├── CMakeLists.txt  # Cross-platform build
└── build/          # Build output directory
```

## Known Issues & Notes
1. **Format Security Warning**: `stdinout.c:83` has a format-security warning in `print_string()` - uses printf without format specifier
2. **Array Syntax Error**: `timer.c:93` has incorrect C syntax `char[50] buf` (should be `char buf[50]`)
3. **Index Logic**: `delete_timer_record()` has potential off-by-one error at line 115 (`i = idx-1`)
4. **No Input Validation**: User inputs are not validated for range (e.g., hours should be 0-23)
5. **Buffer Overflow Risk**: Fixed-size buffers with sprintf() could overflow
6. **Conditional Compilation**: Uses `#ifdef OUTPUT` and `#ifdef STDINPUT` for different build configurations

## Security Considerations
- All user input should be validated
- Replace sprintf() with snprintf() to prevent buffer overflows
- Fix format string vulnerability in print_string()

## Linux-Specific Changes from Windows Version
- Executable name: `timer` (not `timer.exe`) 
- Removed Windows-specific cpptest integration
- Uses standard POSIX time functions (time.h)
- Compatible with gcc/MinGW compilers

## Future Improvements
- Add input validation for time ranges and channel numbers
- Implement unit tests
- Add error handling for malloc failures
- Fix identified security issues
- Add persistent storage (save/load timers)
