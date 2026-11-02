CC      = gcc
CFLAGS  = -Wall -Wextra -std=c99
LDLIBS  = -lm
OBJS    = main.o artemis.o validaciones.o

ifeq ($(OS),Windows_NT)
    TARGET = artemis.exe
    RM     = del /Q /F
else
    TARGET = artemis
    RM     = rm -f
endif

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDLIBS)

main.o: main.c artemis.h validaciones.h
artemis.o: artemis.c artemis.h validaciones.h
validaciones.o: validaciones.c validaciones.h

clean:
	-$(RM) $(OBJS) $(TARGET)

.PHONY: all clean
