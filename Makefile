CXX=g++
CXXFLAGS=-g
LIBS := -lSDL -lSDL_image -lSDL_gfx
OBJECTS := Logger.o Engine.o EventDispatcher.o Tile.o TileSet.o Map.o tinystr.o tinyxml.o tinyxmlerror.o tinyxmlparser.o assist.o Renderer.o Drawer.o Surface.o Animation.o AnimationManager.o AnimationPack.o Unit.o FPSEqualizer.o MapObject.o  Timer.o Scenario.o UnitTypeManager.o Player.o main.o

Unnamed: $(OBJECTS)
	$(CXX) $(OBJECTS) $(LIBS) -o Unnamed

%.o : %.c
	$(CXX) $(CXXFLAGS) -c $(input) -o $(output)

clean:
	rm -rf *~ *.o *.swp log.txt Unnamed
