
out: messages.o manager.o participant.o main.o 
	g++ -o out messages.o manager.o participant.o main.o -lpthread

main.o: src/main.cpp
	g++ -c src/main.cpp -lpthread

messages.o: src/messaging/messages.cpp
	g++ -c src/messaging/messages.cpp

manager.o: src/entities/manager.cpp
	g++ -c src/entities/manager.cpp

participant.o: src/entities/participant.cpp
	g++ -c src/entities/participant.cpp

clean:
	rm *.o out