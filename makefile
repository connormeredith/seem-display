CC = gcc
CFLAGS = -Wall
OBJECTS = main.o display.o serial.o memory.o
LIB_OBJECTS = lib/rs232.o
LIB_FLAGS = `sdl2-config --cflags --libs`

all: $(OBJECTS) $(LIB_OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LIB_OBJECTS) -o seem-display $(LIB_FLAGS)

%.o: src/%.c src/%.h
	$(CC) $(CFLAGS) -c $< -o $@