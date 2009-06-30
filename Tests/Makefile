Tests: AnimationTest BlitTest SplitSurfaceTest AnimationManagerTest TilesetTest
	echo "All tests compiled"

AnimationTest: AnimationTest.o ../Renderer.o ../Animation.o ../Timer.o ../Surface.o  ../Drawer.o ../Logger.o
	g++ -o AnimationTest -lSDL -lSDL_image -lSDL_gfx ../Renderer.o ../Animation.o ../Timer.o ../Surface.o ../Drawer.o ../Logger.o AnimationTest.o

BlitTest: BlitTest.o ../Renderer.o ../Timer.o ../Surface.o  ../Drawer.o ../Logger.o
	g++ -o BlitTest -lSDL -lSDL_image -lSDL_gfx ../Renderer.o ../Animation.o ../Timer.o ../Surface.o ../Drawer.o ../Logger.o BlitTest.o

SplitSurfaceTest: SplitSurfaceTest.o ../Renderer.o ../Timer.o ../Surface.o  ../Drawer.o ../Logger.o
	g++ -o SplitSurfaceTest -lSDL -lSDL_image -lSDL_gfx ../Renderer.o ../Animation.o ../Timer.o ../Surface.o ../Drawer.o ../Logger.o SplitSurfaceTest.o

TilesetTest: TilesetTest.o ../Renderer.o ../Timer.o ../Surface.o  ../Drawer.o ../Logger.o ../Tile.o ../TileSet.o
	g++ -o TilesetTest -lSDL -lSDL_image -lSDL_gfx ../Renderer.o ../Animation.o ../Timer.o ../Surface.o ../Drawer.o ../Logger.o ../Tile.o ../TileSet.o TilesetTest.o

TilesetTest.o: TilesetTest.cpp
	g++ -g -c TilesetTest.cpp

AnimationTest.o: AnimationTest.cpp
	g++ -g -c AnimationTest.cpp

AnimationManagerTest: ../Renderer.o ../Animation.o ../Timer.o ../Drawer.o ../Surface.o ../Logger.o ../AnimationManager.o AnimationManagerTest.o
	g++ -o AnimationManagerTest -lSDL -lSDL_image -lSDL_gfx ../Renderer.o ../Animation.o ../Timer.o ../Drawer.o ../Surface.o ../Logger.o ../AnimationManager.o AnimationManagerTest.o

AnimationManagerTest.o: AnimationManagerTest.cpp
	g++ -g -c AnimationManagerTest.cpp

BlitTest.o: BlitTest.cpp
	g++ -g -c BlitTest.cpp

SplitSurfaceTest.o: SplitSurfaceTest.cpp
	g++ -g -c SplitSurfaceTest.cpp

clean:
	rm -rf *.~ *.o AnimationTest BlitTest SplitSurfaceTest AnimationManagerTest TilesetTest log.txt
