
out: messages.o main.o 
	gcc -o out messages.o main.o

main.o: src/main.c
	gcc -c src/main.c

messages.o: src/messaging/messages.c
	gcc -c src/messaging/messages.c

clean:
	rm *.o out