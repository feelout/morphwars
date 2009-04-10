CXX=g++
CXXFLAGS=-g
LIBS := -lSDL
OBJECTS := Logger.o Engine.o main.o

Unnamed: $(OBJECTS)
	$(CXX) $(OBJECTS) $(LIBS) -o Unnamed

%.o : %.c
	$(CXX) $(CXXFLAGS) -c $(input) -o $(output)

clean:
	rm -rf *~ *.o
