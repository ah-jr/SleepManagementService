
sleep_server: messages.o manager.o main.o 
	g++ -std=c++11 -o sleep_server messages.o manager.o main.o -lpthread

main.o: src/main.cpp
	g++ -std=c++11 -c src/main.cpp -lpthread

messages.o: src/messaging/messages.cpp
	g++ -std=c++11 -c src/messaging/messages.cpp

manager.o: src/entities/manager.cpp
	g++ -std=c++11 -c src/entities/manager.cpp

clean:
	rm *.o sleep_server
