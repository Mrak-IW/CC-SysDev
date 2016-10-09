.PHONY : all clear install uninstall install install-strip run giveExecRights

srcDir := .
incDir := include
outDir := output

wdir := $(shell pwd | sed "s/ /\\\ /g")
src := ./
inc := include/
out ?= output/

srcNames = $(shell ls -1 $(src) | grep *.c) 

objNames = $(srcNames:.c=.o) 

SOURCES := $(addprefix $(src), $(srcNames))
OBJECTS := $(addprefix $(out), $(objNames))

CC := gcc
CCFLAGS := -c -Wall -g -ggdb -O0 -I$(inc)
LDFLAGS :=

VPATH = $(src):$(inc):$(out)

all: $(OBJECTS)
#	@printf "\n\tMAKING %s:\n" '$(shell pwd)'
#	@printf "\tDEPENDS: %s\n" $(OBJECTS)
	
$(out):
#	@printf "\n\tCreating output directory:\n\t%s\n" '$@'
	@mkdir -p '$@'
	
$(out)secondProcessFunction.o: secondProcessFunction.c $(out)
#	@printf "\n\tCOMPILING: %s\n" '$@'
	$(CC) $(CCFLAGS) -o '$@' '$<'