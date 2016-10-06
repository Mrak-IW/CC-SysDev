.PHONY : all clear install uninstall install install-strip run giveExecRights

src=./src/
inc=./include/
out=./output/
installPath=~/opt/

VPATH = $(src):$(inc):$(out)

EXECNAME=PipesAndSignals

SOURCES:=\
main.c \
firstProcessFunction.c \
secondProcessFunction.c

OBJECTS:=$(SOURCES:.c=.o)

SOURCES:=$(addprefix $(src), $(SOURCES))
OBJECTS:=$(addprefix $(out), $(OBJECTS))
EXECUTABLE:=$(addprefix $(out), $(EXECNAME))

CC=gcc
CCFLAGS=-c -Wall -g -ggdb -O0 -I$(inc)
LDFLAGS=

all: $(EXECUTABLE) giveExecRights

run: $(EXECUTABLE) giveExecRights
	@printf '\n%s\n\n' 'Running the program:'
	@$<
	@printf '\n'

install: $(EXECUTABLE) giveExecRights
	mkdir -p $(installPath)
	cp -uf $< $(installPath)
	
install-strip: install
	
uninstall:
	rm -f $(installPath)$(EXECNAME)
	
clear:
	rm -f $(out)*

giveExecRights: $(EXECUTABLE)	
	@chmod 755 $<

$(EXECUTABLE): $(OBJECTS)
	@echo *$@:
	@printf '\t%s\n' $^
	$(CC) $(LDFLAGS) -o $@ $^
	
$(out)main.o: main.c
	@echo *$@:
	@printf '\t%s\n' $^
	$(CC) $(CCFLAGS) -o $@ $<

$(out)firstProcessFunction.o: firstProcessFunction.c
	$(CC) $(CCFLAGS) -o $@ $<
	
$(out)secondProcessFunction.o: secondProcessFunction.c
	$(CC) $(CCFLAGS) -o $@ $<