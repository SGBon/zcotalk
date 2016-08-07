CC=clang
CFLAGS=-c -Wall -Wextra
LDFLAGS=
SOURCES=main.c setup.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=uoitalk

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

%.o : %.c
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean run
run: all
	./$(EXECUTABLE)
clean:
	rm $(OBJECTS) $(EXECUTABLE)
