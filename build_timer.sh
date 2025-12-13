#!/bin/bash
#
# Build script for Timer application (Linux)
#

set -e

CC=${CC:-gcc}
CFLAGS="-Wall -Wextra -pedantic -std=c99 -g"
LDFLAGS=""
OUTPUT="timer"

SOURCES="clock.c driver.c timer.c stdinout.c"

echo "=== Building Timer Application ==="
echo "Compiler: $CC"
echo "Flags: $CFLAGS"
echo ""

# Clean previous build
if [ -f "$OUTPUT" ]; then
    echo "Cleaning previous build..."
    rm -f "$OUTPUT"
fi

# Compile
echo "Compiling..."
$CC $CFLAGS -o $OUTPUT $SOURCES $LDFLAGS

if [ -f "$OUTPUT" ]; then
    echo ""
    echo "=== Build successful! ==="
    echo "Run with: ./$OUTPUT"
else
    echo "Build failed!"
    exit 1
fi
