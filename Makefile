CXX=g++
CXXFLAGS=-g
LIBS := -lmingw32 -lSDLmain -lSDL -lSDL_image -lSDL_gfx -lSDL_ttf -mwindows
OBJECTS := Logger.o Engine.o EventDispatcher.o Tile.o TileSet.o Map.o tinystr.o tinyxml.o tinyxmlerror.o tinyxmlparser.o assist.o Renderer.o Drawer.o Surface.o Animation.o AnimationManager.o AnimationPack.o Unit.o FPSEqualizer.o MapObject.o  Timer.o Scenario.o UnitTypeManager.o Player.o Order.o OrderManager.o Force.o AStar.o Widget.o Container.o Button.o EngineState.o StateStack.o Menu.o List.o FontSystem.o ScenarioSelector.o Minimap.o SidePanel.o PlayerController.o MouseSelector.o OrderPack.o main.o

Unnamed: $(OBJECTS)
	$(CXX) $(OBJECTS) $(LIBS) -o Unnamed

%.o : %.h %.c
	$(CXX) $(CXXFLAGS) -c $(input) -o $(output)

clean:
	rm -rf *~ *.o *.swp log.txt Unnamed.exe
