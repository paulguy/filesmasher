OBJS   = range.o main.o
TARGET = filesmasher
CFLAGS = -D_LARGEFILE64_SOURCE -pedantic -Wall -Wextra

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJS)

all: $(TARGET)

clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: clean
