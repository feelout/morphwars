Tests: Prereq AnimationTest AnimationPackTest BlitTest SplitSurfaceTest AnimationManagerTest TilesetTest MapTest ShadowedSurfaceTest ButtonTest
	echo "All tests compiled"

Prereq:
	rm ../main.o

AnimationTest: AnimationTest.o ../Renderer.o ../Animation.o ../Timer.o ../Surface.o  ../Drawer.o ../Logger.o 
	g++ -o AnimationTest -lSDL -lSDL_image -lSDL_gfx ../*.o AnimationTest.o

AnimationPackTest: AnimationPackTest.o
	g++ -o AnimationPackTest -lSDL -lSDL_image -lSDL_gfx ../*.o AnimationPackTest.o

MapTest: MapTest.o ../Renderer.o ../Animation.o ../Timer.o ../Surface.o  ../Drawer.o ../Logger.o ../Map.o ../Scenario.o
	g++ -o MapTest -lSDL -lSDL_image -lSDL_gfx ../*.o MapTest.o

ButtonTest: ButtonTest.o
	g++ -o ButtonTest -lSDL -lSDL_image -lSDL_gfx ../*.o ButtonTest.o

BlitTest: BlitTest.o ../Renderer.o ../Timer.o ../Surface.o  ../Drawer.o ../Logger.o
	g++ -o BlitTest -lSDL -lSDL_image -lSDL_gfx ../*.o BlitTest.o

SplitSurfaceTest: SplitSurfaceTest.o ../Renderer.o ../Timer.o ../Surface.o  ../Drawer.o ../Logger.o
	g++ -o SplitSurfaceTest -lSDL -lSDL_image -lSDL_gfx ../*.o SplitSurfaceTest.o

ShadowedSurfaceTest: ShadowedSurfaceTest.o ../Renderer.o ../Timer.o ../Surface.o  ../Drawer.o ../Logger.o
	g++ -o ShadowedSurfaceTest -lSDL -lSDL_image -lSDL_gfx ../*.o ShadowedSurfaceTest.o

ShadowedSurfaceTest.o: ShadowedSurfaceTest.cpp
	g++ -g -c ShadowedSurfaceTest.cpp

TilesetTest: TilesetTest.o ../Renderer.o ../Timer.o ../Surface.o  ../Drawer.o ../Logger.o ../Tile.o ../TileSet.o
	g++ -o TilesetTest -lSDL -lSDL_image -lSDL_gfx ../*.o TilesetTest.o

TilesetTest.o: TilesetTest.cpp
	g++ -g -c TilesetTest.cpp

AnimationTest.o: AnimationTest.cpp
	g++ -g -c AnimationTest.cpp

AnimationPackTest.o: AnimationPackTest.cpp
	g++ -g -c AnimationPackTest.cpp

MapTest.o: MapTest.cpp
	g++ -g -c MapTest.cpp

AnimationManagerTest: ../Renderer.o ../Animation.o ../Timer.o ../Drawer.o ../Surface.o ../Logger.o ../AnimationManager.o AnimationManagerTest.o
	g++ -o AnimationManagerTest -lSDL -lSDL_image -lSDL_gfx ../*.o AnimationManagerTest.o

AnimationManagerTest.o: AnimationManagerTest.cpp
	g++ -g -c AnimationManagerTest.cpp

BlitTest.o: BlitTest.cpp
	g++ -g -c BlitTest.cpp

SplitSurfaceTest.o: SplitSurfaceTest.cpp
	g++ -g -c SplitSurfaceTest.cpp

Buttontest.o: ButtonTest.cpp
	g++ -g -c ButtonTest.cpp

clean:
	rm -rf *.~ *.o AnimationTest AnimationPackTest BlitTest SplitSurfaceTest AnimationManagerTest TilesetTest MapTest ShadowedSurfaceTest ButtonTest log.txt
