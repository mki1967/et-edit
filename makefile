# INC= -I/usr/X11/include/ -I/usr/share/doc/packages/NVIDIA_GLX/include/
# INC= -I/usr/include/X11/ -I/usr/share/doc/NVIDIA_GLX-1.0/include/
INC= -I/usr/include/X11/
# INC= -I/usr/openwin/include/
# LIB= -L/usr/X11/lib 
LIB= -L/usr/X11R6/lib 
 
# LIB= -L/usr/openwin/lib/

all: et-edit

et-edit: et-edit.c et-edit.h makefile
	gcc -g -Wall -pedantic $(INC) $(LIB)  -o et-edit et-edit.c -lX11 -lm  -lGL

src: 
	mv et-edit et-edit.bin; cd ..; tar zcvf et-edit.tgz \
	et-edit/{*.c,*.h,makefile,COPYING,NEWS,et-edit.bin,*.et,t_*.png,*.html}\

