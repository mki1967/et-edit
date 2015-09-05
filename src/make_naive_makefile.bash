#!/bin/bash
# Generates a sequence of implicit rules for creating '.o' files.
# The generated rules are to be included in the naive makefile

cat - <<EOF
# A naive makefile with implicit rules generated by make_naive_makefile.bash
CPPFLAGS= -I/usr/include/X11/
CFLAGS= -g -Wall -pedantic
LDFLAGS= -L/usr/X11R6/lib
LDLIBS= -lX11 -lm  -lGL

all: et-edit

.PHONY: all clean

clean:
	-rm rm -f *.o core
EOF

echo '# The rules for making .o files from .c files'
# for x in $(find -name '*.c') ; do echo ${x/'.c'/'.o'} ':' $x include/*.h ; done 
for x in *.c ; do echo ${x/'.c'/'.o'} ':' $x *.h ; done 

echo '# $(OBJS) denotes the set of .o files'
# for x in $(find -name '*.c') ; do echo 'OBJS+= ' ${x/'.c'/'.o'}  ; done
for x in *.c ; do echo 'OBJS+= ' ${x/'.c'/'.o'}  ; done

echo '# Implicit rule for the executable et-edit'
echo 'et-edit: $(OBJS)'

