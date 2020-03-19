all: blockchain

blockchain: blockchain.o main.o
	gcc -o blockchain blockchain.o main.o -lssl -lcrypto

blockchain.o: blockchain.c
	gcc -o blockchain.o -c blockchain.c -W -Wall -std=c99

main.o: main.c blockchain.h
	gcc -o main.o -c main.c -W -Wall -std=c99

clean:
	rm -rf *.o

mrproper: clean
	rm -rf blockchain