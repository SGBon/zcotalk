CC=clang
CFLAGS=-c -Wall -Wextra -g3
LDFLAGS=-g3 -lpthread
SOURCES=main.c setup.c util.c messenger.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=zcotalk

all: $(SOURCES) $(EXECUTABLE) depend

depend: .depend

.depend: $(SOURCES)
	rm -f ./.depend
	$(CC) $(CFLAGS) -MM $^ -MF ./.depend;

include .depend

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

%.o : %.c
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean run
run: all
	./$(EXECUTABLE)
clean:
	rm $(OBJECTS) $(EXECUTABLE)
