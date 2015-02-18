# INC= -I/usr/X11/include/ -I/usr/share/doc/packages/NVIDIA_GLX/include/
# INC= -I/usr/include/X11/ -I/usr/share/doc/NVIDIA_GLX-1.0/include/
INC= -I/usr/include/X11/ -I./include/
# INC= -I/usr/openwin/include/
# LIB= -L/usr/X11/lib 
LIB= -L/usr/X11R6/lib 
 
# LIB= -L/usr/openwin/lib/

# $(MAIN_SOURCE) -- all source and header files are directly or indirectly included in $(MAIN_SOURCE)
MAIN_SOURCE= et-edit.c
# $(VAR_DEFS) -- definitions of global variables
VAR_DEFS= et-variables.c
# $(MAIN_SOURCE) -- directly and indirectly included source files
INCLUDED_SOURCES= $(wildcard include/%.c) 
# $(INDCLUDED_HEADERS) -- directly and indirectly included headers
INDCLUDED_HEADERS= $(wildcard include/%.h)

#$(EXECUTABLE) -- name of executable file
EXECUTABLE= et-edit

all: $(EXECUTABLE)

$(EXECUTABLE): $(MAIN_SOURCE) $(INCLUDED_SOURCES) $(INDCLUDED_HEADERS) makefile
	gcc -g -Wall -pedantic $(INC) $(LIB)  -o $@ $< $(VAR_DEFS) -lX11 -lm  -lGL

src: 
	mv et-edit et-edit.bin; cd ..; tar zcvf et-edit.tgz \
	et-edit/{*.c,*.h,makefile,COPYING,NEWS,et-edit.bin,*.et,t_*.png,*.html}\

