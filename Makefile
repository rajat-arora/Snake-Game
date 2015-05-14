# Linux (default)
EXE = Anakonda
LDFLAGS = -lGL -lGLU -lglut -lm
CFLAGS=-g -Wall -std=c99
CC=gcc

# Windows (cygwin)
ifeq "$(OS)" "Windows_NT"
    EXE = Anakonda.exe
        LDFLAGS = -lfreeglut -lglu32 -lopengl32
endif

# OS X
OS := $(shell uname)
ifeq ($(OS), Darwin)
        LDFLAGS = -framework Carbon -framework OpenGL -framework GLUT
endif

t1: main.c scene_control.c snake_control.c font_control.c particle_ctrl.c texture_mgr.c
	$(CC) main.c scene_control.c snake_control.c font_control.c particle_ctrl.c texture_mgr.c -o $(EXE) $(CFLAGS) $(LDFLAGS)

clean:
	rm ./*.o ./*.exe ./$(EXE)
