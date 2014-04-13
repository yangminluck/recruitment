LinkedListTest:LinkedListTest.o LinkedClass.o
	g++ -o LinkedListTest LinkedListTest.o LinkedClass.o
LinkedClass.o:LinkedClass.o LinkedClass.h
	g++ -c LinkedClass.cpp
LinkedListTest.o:LinkedListTest.cpp LinkedClass.h
	g++ -c LinkedListTest.cpp
clean:
	rm LinkedListTest LinkedListTest.o LinkedClass.o -f 
