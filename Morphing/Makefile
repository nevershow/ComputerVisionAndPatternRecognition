CPP = g++ -g -lpthread -l X11 -O3
	
Morphing: bin/triangle.o bin/delaunay.o bin/dlib.o bin/morphing.o bin/main.o
	$(CPP) bin/*.o -o Morphing

bin/triangle.o: triangle.h triangle.cpp
	$(CPP) -c triangle.cpp -o bin/triangle.o

bin/delaunay.o: delaunay.h delaunay.cpp triangle.h
	$(CPP) -c delaunay.cpp -o bin/delaunay.o

bin/dlib.o:
	$(CPP) -c dlib/all/source.cpp -o bin/dlib.o

bin/morphing.o: morphing.h morphing.cpp
	$(CPP) -c morphing.cpp -o bin/morphing.o

bin/main.o: main.cpp
	$(CPP) -c main.cpp -o bin/main.o

run:
	./Morphing
	
clean:
	rm -f bin/*.o Morphing
