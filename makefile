main:main.o cmd.o PeopleData.o socket.o socketInOut.o TelBook.o
	g++ main.o cmd.o PeopleData.o socket.o socketInOut.o TelBook.o -o main -lpthread
socketInOut.o:socketInOut.cpp socketInOut.h
	g++ -c socketInOut.cpp -o socketInOut.o -g
main.o:main.cpp socket.h
	g++ -c main.cpp -o main.o -g
cmd.o:cmd.cpp cmd.h
	g++ -c cmd.cpp -o cmd.o -g
PeopleData.o:PeopleData.cpp PeopleData.h
	g++ -c PeopleData.cpp -o PeopleData.o -g
TelBook.o:TelBook.cpp TelBook.h
	g++ -c TelBook.cpp -o TelBook.o -g
socket.o:socket.cpp socket.h
	g++ -c socket.cpp -o socket.o -g
clean:
	rm main main.o cmd.o PeopleData.o socket.o socketInOut.o TelBook.o -f
