CXXFLAGS=-march=native -mtune=native -g
LDFLAGS=
OBJECTS=day15.main.o day15.parse.o day15.game.o
all: day15.exe

day15.exe: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o day15.exe $(OBJECTS) $(LDFLAGS)
