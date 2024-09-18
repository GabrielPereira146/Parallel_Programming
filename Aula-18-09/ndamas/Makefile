
CC = gcc

CFLAGS = -O3
LDFLAGS = -fopenmp

TARGET = ndamas.x 

SOURCE = ndamas.c

ALL: $(TARGET)

ndamas.x: $(SOURCE)
	$(CC) $(CFLAGS) $< -o$@ $(LDFLAGS)

clean:
	rm -f $(TARGET) *.o
