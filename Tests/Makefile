Tests: AnimationTest BlitTest SplitSurfaceTest
	echo "All tests compiled"

AnimationTest: AnimationTest.o ../Renderer.o ../Animation.o ../Timer.o ../Surface.o  ../Drawer.o ../Logger.o
	g++ -o AnimationTest -lSDL -lSDL_image -lSDL_gfx ../Renderer.o ../Animation.o ../Timer.o ../Surface.o ../Drawer.o ../Logger.o AnimationTest.o

BlitTest: BlitTest.o ../Renderer.o ../Timer.o ../Surface.o  ../Drawer.o ../Logger.o
	g++ -o BlitTest -lSDL -lSDL_image -lSDL_gfx ../Renderer.o ../Animation.o ../Timer.o ../Surface.o ../Drawer.o ../Logger.o BlitTest.o

SplitSurfaceTest: SplitSurfaceTest.o ../Renderer.o ../Timer.o ../Surface.o  ../Drawer.o ../Logger.o
	g++ -o SplitSurfaceTest -lSDL -lSDL_image -lSDL_gfx ../Renderer.o ../Animation.o ../Timer.o ../Surface.o ../Drawer.o ../Logger.o SplitSurfaceTest.o

AnimationTest.o: AnimationTest.cpp
	g++ -g -c AnimationTest.cpp

BlitTest.o: BlitTest.cpp
	g++ -g -c BlitTest.cpp

SplitSurfaceTest.o: SplitSurfaceTest.cpp
	g++ -g -c SplitSurfaceTest.cpp

clean:
	rm -rf *.~ *.o AnimationTest BlitTest SplitSurfaceTest log.txt
