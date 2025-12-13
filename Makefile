# Makefile for Timer application (Linux)

CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99 -g
LDFLAGS =

SOURCES = clock.c driver.c timer.c stdinout.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = timer

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJECTS) $(TARGET)
