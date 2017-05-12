export LD_LIBRARY_PATH=glnxa64/

CPP = g++ -g -lpthread -l X11 -Lglnxa64/ -lvl -O3 

Stitching: obj/Blend.o obj/Feature.o obj/Match.o obj/Projection.o obj/Warping.o obj/Stitching.o obj/main.o
	$(CPP) obj/*.o -o Stitching

obj/Blend.o: Blend.h Blend.cpp
	$(CPP) -c Blend.cpp -o obj/Blend.o

obj/Feature.o: Feature.h Feature.cpp
	$(CPP) -c Feature.cpp -o obj/Feature.o

obj/Match.o: Match.h Match.cpp
	$(CPP) -c Match.cpp -o obj/Match.o

obj/Projection.o: Projection.h Projection.cpp
	$(CPP) -c Projection.cpp -o obj/Projection.o

obj/Warping.o: Match.h Interpolation.h Warping.cpp
	$(CPP) -c Warping.cpp -o obj/Warping.o

obj/Stitching.o: Stitching.h Stitching.cpp
	$(CPP) -c Stitching.cpp -o obj/Stitching.o

obj/main.o: main.cpp
	$(CPP) -c main.cpp -o obj/main.o

run:
	./Stitching
	
clean:
	rm -f obj/*.o Stitching
