# Compilador a utilizar
CC = g++

# Libraries
LIBS = -lmingw32 -lSDLmain -lSDL -lSDL_image -lSDL_ttf -lSDL_mixer

# Codigo
SRC = main.cpp timer.cpp simplegui.cpp gamesprites.cpp

# Object files
OBJS = $(SRC:.cpp=.o)

# Nombre del ejecutable
EXE = juego_naves

all: $(SRC) $(EXE)

$(OBJS):
	g++ -IC:/SDL-1.2.15/include -c $(SRC)

$(EXE): $(OBJS)
	g++ -LC:/SDL-1.2.15/lib -o $@ $(OBJS) $(LIBS)
	
clean:
	rm -rf *.o $(EXE).exe