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

SOURCES := $(addprefix $(src), $(srcNames))
OBJECTS := $(addprefix $(out), $(objNames))
EXECUTABLE := $(addprefix $(out), $(execName))

allObjNames = $(shell ls -1 $(out) | grep .*\.o$) 
ALLOBJECTS = $(addprefix $(out), $(allObjNames))
		
CC := gcc
CCFLAGS := -c -Wall -g -ggdb -O0 -I$(inc)
LDFLAGS =

export 

all: $(EXECUTABLE) giveExecRights
#	@printf "\n\tMaking ALL\n"
#	@printf "\tOBJECTS: %s\n" $(OBJECTS)
#	@printf "\tSUBOBJECTS: %s\n" $(SUBOBJECTS)
#	@printf "\tEXECUTABLE: %s\n" $(EXECUTABLE)

run: $(EXECUTABLE) giveExecRights
	@printf '\n%s\n\n' 'Running the program:'
	@$<
	@printf '\n'
	
install: $(EXECUTABLE) giveExecRights
	mkdir -p $(installPath)
	cp -uf $< $(installPath)
	
install-strip: install
	strip -d $(installPath)$(execName)
	
clean:
#	@printf "\n\tRemoving temporary files\n"
	rm -rf $(out)
	
giveExecRights: $(EXECUTABLE)	
	@chmod 755 $<
	
subm1:
#	@printf "\n\tCalling make M1\n"
	$(MAKE) -C $(src)m1

subm2:
#	@printf "\n\tCalling make M2\n"
	$(MAKE) -C $(src)m2
	
$(out):
#	@printf "\n\tCreating output directory:\n\t%s\n" '$@'
	mkdir -p '$@'

$(EXECUTABLE): subm1 subm2 $(OBJECTS)
#	@printf "\tOBJECTS: %s\n" $(OBJECTS)
#	@printf "\tALLOBJECTS: %s\n" $(ALLOBJECTS)
	@printf "\n\tLINKING: %s\n" '$@'
	$(CC) $(LDFLAGS) -o '$@' $(ALLOBJECTS)
	
$(out)main.o: main.c $(out)
	@printf "\n\tCOMPILING: %s\n" '$@'
	$(CC) $(CCFLAGS) -o '$@' '$<'
	