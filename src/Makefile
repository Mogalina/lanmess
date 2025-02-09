CC = gcc
CFLAGS = -Wall -Wextra -pthread
LDFLAGS = -lncurses

SRCS = main.c network.c ui.c chat.c
OBJS = $(SRCS:.c=.o)
TARGET = lanmess

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

