.PHONY : all clear install uninstall install run

src=./src/
inc=./include/
out=./output/

EXENAME=PipesAndSignals
EXECUTABLE=$(addprefix $(out), $(EXENAME))

SRCNAMES=main.c
OBJNAMES=$(SRCNAMES:.c=.o)
SOURCES=$(addprefix $(src), $(SRCNAMES))
OBJECTS=$(addprefix $(out), $(OBJNAMES))

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
	$(CC) $(LDFLAGS) -o $@ $^
	
$(out)main.o: $(src)main.c
	@echo *$@:
	@printf '\t%s\n' $^
	$(CC) $(CCFLAGS) -o $@ $<