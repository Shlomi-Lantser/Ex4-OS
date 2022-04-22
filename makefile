client: client

server: server_new

client: client.o
	g++ -o client client.o 

server: server.o
	g++ -o server server.o -lpthread

client.o: client.cpp
	g++ -c client.cpp  

server.o: server_new.cpp
	g++ -pthread server_new.cpp -o server



clean:
	rm -f *.o server client