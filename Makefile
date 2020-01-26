CC=gcc

SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)
TARGET=game

COMPILER_FLAGS := $(shell pkg-config --cflags sdl2 SDL2_image SDL2_ttf SDL2_mixer SDL2_gfx)
LINKER_FLAGS := $(shell pkg-config --libs sdl2 SDL2_image SDL2_ttf SDL2_mixer SDL2_gfx)

# for windows
# COMPILER_FLAGS = -w -Wl,-subsystem,windows
# LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm -lSDL2


all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(COMPILER_FLAGS) $(LINKER_FLAGS)

%.o: %.c %.h
	$(CC) $(CCFLAGS) -c $<

%.o: %.c
	$(CC) $(CCFLAGS) -c $<

clean:
	rm -f *.o $(TARGET)

# replace rm -f by del for windows 
