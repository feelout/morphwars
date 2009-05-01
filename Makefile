CXX=g++
CXXFLAGS=-g
LIBS := -lSDL
OBJECTS := Logger.o Engine.o EventDispatcher.o Tile.o TileSet.o Map.o tinystr.o tinyxml.o tinyxmlerror.o tinyxmlparser.o main.o

Unnamed: $(OBJECTS)
	$(CXX) $(OBJECTS) $(LIBS) -o Unnamed

%.o : %.c
	$(CXX) $(CXXFLAGS) -c $(input) -o $(output)

clean:
	rm -rf *~ *.o *.swp log.txt Unnamed
