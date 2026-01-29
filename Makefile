CC = gcc
CFLAGS = -Wall -O2 -s
LDFLAGS = -lssl -lcrypto

TARGET = eni3
SOURCES = main.c recon.c memexec.c network.c crypto.c persistence.c
HEADERS = loader.h recon.h memexec.h network.h crypto.h persistence.h config.h common.h

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES) $(LDFLAGS)

clean:
	rm -f $(TARGET) *.o

.PHONY: all clean
