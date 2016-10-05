.PHONY : all clear install uninstall install run

src=./src/
inc=./include/
out=./output/

VPATH = $(src):$(inc):$(out)

EXECUTABLE=PipesAndSignals

SOURCES=main.c
OBJECTS=$(SOURCES:.c=.o)

CC=gcc
CCFLAGS=-c -Wall -g -ggdb -O0
LDFLAGS=

all: $(EXECUTABLE)

run: $(EXECUTABLE)
	@chmod 755 $<
	@printf '\n%s\n\n' 'Running the program:'
	@$<
	@printf '\n'

$(EXECUTABLE): $(OBJECTS)
	@echo *$@:
	@printf '\t%s\n' $^
	$(CC) $(LDFLAGS) -o $(out)$@ $^
	
main.o: main.c
	@echo *$@:
	@printf '\t%s\n' $^
	$(CC) $(CCFLAGS) -o $(out)$@ $<