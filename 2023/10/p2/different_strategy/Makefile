
# Compiler and Flags
CC = gcc
CFLAGS = -Wall -Wextra -Wconversion -Wformat=2 -Wpedantic -Werror -g

#src and obj
SRCS = main.c ../dynamic_list.c
OBJS = $SRCS:.c=.o

# output binary
TARGET = main.exe

# default target
all: $(TARGET)


$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Rule for .c → .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJS) $(TARGET)