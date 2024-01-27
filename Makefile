CC = gcc
CFLAGS = -Wall -g
LIBS = -lX11 -lXtst
BINDIR = /usr/local/bin/
TARGET = gamepadcon

gamepadcon: gamepadcon.c config.h
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

clean:
	rm -f main

install: $(TARGET)
	cp -f $(TARGET) $(BINDIR)
	chmod 755 $(BINDIR)/$(TARGET)

.PHONY: clean install
