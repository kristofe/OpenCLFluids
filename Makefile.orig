C = gcc
CFLAGS= -std=gnu99
LIBS = -lrt -lOpenCL -lGL -I/usr/local/include -I/usr/include


EXECUTABLES = fluidsim

linux: main.c cl-helper.c 
	gcc $^ -m64 -std=gnu99 -lrt -lOpenCL -lglut -lGLU -lGL -o fluidsim
mac: main.c cl-helper.c
	gcc -std=gnu99 main.c cl-helper.c  -o fluidsim -framework OpenCL -framework GLUT -framework OpenGL 

%.o : %.c %.h
	gcc -c -std=gnu99 $<

clean:
	rm -f $(EXECUTABLES) *.o