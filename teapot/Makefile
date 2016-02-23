CC = gcc
C  = c

CFLAGS = -g -std=gnu99

ifeq ("$(shell uname)", "Darwin")
  LDFLAGS = -framework Foundation -framework GLUT -framework OpenGL -lm
else
  ifeq ("$(shell uname)", "Linux")
    LDFLAGS = -L /usr/lib64/ -lglut -lGL -lGLU -lm
  endif
endif

PROJECT	= teapot
OBJECTS	= teapot.o

${PROJECT}:	${OBJECTS}
	${CC} ${CFLAGS} -o ${PROJECT} ${OBJECTS} ${LDFLAGS}

%.o:	%.${C}
	${CC} ${CFLAGS} -c *.${C}

clean:
	rm core.*; rm *.o; rm *~; rm ${PROJECT}
