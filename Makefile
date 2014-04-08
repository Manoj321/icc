CC = g++

PROG = matsort
CFLAGS =`pkg-config --cflags opencv`
SRCS = $(PROG).cpp
INCLUDE = -I/usr/local/include/libfreenect
FREE_LIBS = -L/usr/local/lib -lfreenect -lfreenect_sync
LIBS2 =`pkg-config --libs opencv`

ifeq ($(shell uname),Darwin)
	LIBS = -framework OpenGL -framework GLUT
else
	LIBS = -lglut -lGLU
endif

all: $(PROG)

$(PROG):	$(SRCS)
	$(CC) $(INCLUDE)  $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS) $(FREE_LIBS) $(LIBS2)

clean:
	rm -f $(PROG)
