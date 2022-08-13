
out: messages.o main.o discovery.o
	g++ -o out messages.o main.o discovery.o -lpthread

main.o: src/main.cpp
	g++ -c src/main.cpp -lpthread

messages.o: src/messaging/messages.cpp
	g++ -c src/messaging/messages.cpp

discovery.o: src/services/discovery.cpp
	g++ -c src/services/discovery.cpp 

clean:
	rm *.o out