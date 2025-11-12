CC := gcc
# Include sources from root, mapa, menus, and movimentacao directories
SRC := $(wildcard *.c) \
       $(wildcard mapa/*.c) \
       $(wildcard menus/*.c) \
       $(wildcard movimentacao/*.c) \
       $(wildcard battle/*.c)
OBJ := $(SRC:.c=.o)
TARGET := magic_battle.exe

CFLAGS := -O2 -std=c99 -Wall -Wextra -I.
LIBS := -lraylib -lopengl32 -lgdi32 -lwinmm
RM := rm -f

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	./$(TARGET)

clean:
	$(RM) $(OBJ) $(TARGET)



# No UCRT64 use: mingw32-make e mingw32-make run
