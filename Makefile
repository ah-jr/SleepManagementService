
out: messages.o main.o discovery.o
	gcc -o out messages.o main.o discovery.o

main.o: src/main.c
	gcc -c src/main.c

messages.o: src/messaging/messages.c
	gcc -c src/messaging/messages.c

discovery.o: src/services/discovery.c
	gcc -c src/services/discovery.c

clean:
	rm *.o out