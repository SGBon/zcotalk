CC=clang
CFLAGS=-c -Wall -Wextra -g3
LIBS=-lpthread -lncurses
LDFLAGS=$(LIBS)
SOURCES=main.c setup.c util.c messenger.c wm.c
OBJECTS=$(SOURCES:.c=.o)
DEPEND=.depend
EXECUTABLE=zcotalk

all: $(SOURCES) $(EXECUTABLE) depend

depend: $(DEPEND)

$(DEPEND): $(SOURCES)
	rm -f $@
	$(CC) $(CFLAGS) -MM $^ > $@;

include .depend

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

%.o : %.c
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean run
run: all
	./$(EXECUTABLE)
clean:
	rm $(OBJECTS) $(EXECUTABLE) $(DEPEND)
