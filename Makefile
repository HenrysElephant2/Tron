# Main Executable
EXE=Main

# Main target
all: $(EXE)

#  MinGW
ifeq "$(OS)" "Windows_NT"
CFLG=-O3 -Wall
LIBS=-lglut32cu -lglu32 -lopengl32
CLEAN=del *.exe *.o *.a
else
#  OSX
ifeq "$(shell uname)" "Darwin"
CFLG=-O3 -Wall -Wno-deprecated-declarations
LIBS=-framework GLUT -framework OpenGL -framework SDL2
#  Linux/Unix/Solaris
else
CFLG=-O3 -Wall
LIBS=-lglut -lGLU -lGL -lm -lSDL2
endif
#  OSX/Linux/Unix/Solaris
CLEAN=rm -f $(EXE) *.o *.a
endif

# Dependencies
Vector.o: Vector.cpp Vector.h
Matrix.o: Matrix.cpp Matrix.h
Hitbox.o: Hitbox.cpp Hitbox.h
TrailSegment.o: TrailSegment.cpp TrailSegment.h
Trail.o: Trail.cpp Trail.h
Player.o: Player.cpp Player.h
Tile.o: Tile.cpp Tile.h
Map.o: Map.cpp Map.h
GameState.o: GameState.cpp GameState.h
HitboxTest.o: HitboxTest.cpp HitboxTest.h
Gameplay.o: Gameplay.cpp Gameplay.h
Model.o: Model.cpp Model.h
LoadModel.o: LoadModel.c LoadModel.h
loadtexbmp.o: loadtexbmp.c LoadModel.h

# Compile rules
.c.o:
	gcc -c $(CFLG) $<
.cpp.o:
	g++ -std=c++11 -c $(CFLG) $<

#  Link
Main:Main.o Vector.o Matrix.o Hitbox.o TrailSegment.o Trail.o Player.o Tile.o Map.o GameState.o HitboxTest.o Gameplay.o Model.o LoadModel.o loadtexbmp.o
	g++ -O3 -o $@ $^   $(LIBS)

#  Clean
clean:
	$(CLEAN)
