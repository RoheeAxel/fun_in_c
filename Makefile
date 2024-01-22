# Makefile

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall

# Source files
SRCS = $(wildcard *.c)
SRCS += $(wildcard vector/*.c)

HEADERS = $(wildcard *.h)
HEADERS += $(wildcard error/*.h)
HEADERS += $(wildcard function/*.h)
HEADERS += $(wildcard oop/*.h)

# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
TARGET = c+-

# Default target
all: $(TARGET)

# Compile source files into object files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@


# Link object files into executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Clean up object files and executable
clean:
	rm -f $(OBJS) $(TARGET)

debug:
	$(CC) $(CFLAGS) -g $(SRCS) -o $(TARGET)

re: clean all