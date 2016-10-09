.PHONY : all clean install uninstall install install-strip run giveExecRights

srcDir := src
incDir := include
outDir := output
execName = PipesAndSignals

wdir := $(shell pwd | sed "s/ /\\\ /g")
src := $(wdir)/$(srcDir)/
inc := $(wdir)/$(incDir)/
out := $(wdir)/$(outDir)/
installPath = ~/opt/

VPATH = $(srcDir):$(incDir):$(outDir)

srcNames:= \
	main.c 

objNames = $(srcNames:.c=.o) 
	
subObjNames = $(shell ls -1 $(out) | grep .*\.o$) 
	
SOURCES := $(addprefix $(src), $(srcNames))
OBJECTS := $(addprefix $(out), $(objNames))
EXECUTABLE := $(addprefix $(out), $(execName))

SUBOBJECTS = $(addprefix $(out), $(subObjNames))

CC := gcc
CCFLAGS := -c -Wall -g -ggdb -O0 -I$(inc)
LDFLAGS =

export 

all: subm1 subm2 $(EXECUTABLE)
#	@printf "\n\tMaking ALL\n"
#	@printf "\tOBJECTS: %s\n" $(OBJECTS)
#	@printf "\tSUBOBJECTS: %s\n" $(SUBOBJECTS)
#	@printf "\tEXECUTABLE: %s\n" $(EXECUTABLE)
	
subm1:
#	@printf "\n\tCalling make M1\n"
	$(MAKE) -C $(src)m1

subm2:
#	@printf "\n\tCalling make M2\n"
	$(MAKE) -C $(src)m2
	
$(out):
#	@printf "\n\tCreating output directory:\n\t%s\n" '$@'
	mkdir -p '$@'

clean:
#	@printf "\n\tRemoving temporary files\n"
	rm -rf $(out)

$(EXECUTABLE): $(OBJECTS) $(SUBOBJECTS)
#	@printf "\tOBJECTS: %s\n" $(OBJECTS)
#	@printf "\tSUBOBJECTS: %s\n" $(SUBOBJECTS)
#	@printf "\n\tLINKING: %s\n" '$^'
	$(CC) $(LDFLAGS) -o '$@' $^
	
$(out)main.o: main.c $(out)
	@printf "\n\tCOMPILING: %s\n" '$@'
	$(CC) $(CCFLAGS) -o '$@' '$<'
	
test:
	@printf "\n\tVPATH: %s\n" '$(VPATH)'
	