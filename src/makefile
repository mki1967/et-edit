INC= -I/usr/include/X11/
LIB= -L/usr/X11R6/lib 
 

CPPFLAGS= -I/usr/include/X11/
CFLAGS= -g -Wall -pedantic
LDFLAGS= -L/usr/X11R6/lib
LDLIBS= -lX11 -lm  -lGL

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

