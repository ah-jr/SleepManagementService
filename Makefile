
sleep_server: messages.o manager.o main.o 
	g++ -o sleep_server messages.o manager.o main.o -lpthread

main.o: src/main.cpp
	g++ -c src/main.cpp -lpthread

messages.o: src/messaging/messages.cpp
	g++ -c src/messaging/messages.cpp

manager.o: src/entities/manager.cpp
	g++ -c src/entities/manager.cpp

clean:
	rm *.o sleep_server