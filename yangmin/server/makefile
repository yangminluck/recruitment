eserver: server.o ManageAddr.o CommUnit.o
	g++ -o eserver server.o ManageAddr.o CommUnit.o -lpthread 
server.o:server.cpp
	g++ -c server.cpp
ManageAddr.o:ManageAddr.cpp
	g++ -c ManageAddr.cpp
CommUnit.o: CommUnit.cpp
	 g++ -c CommUnit.cpp
clean: 
	rm eserver server.o ManageAddr.o CommUnit.o












