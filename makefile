client:
	gcc client.c -o client367

server:
	gcc server.c -o server367

all: client server

clean:
	rm -f *.o

real_clean: clean
	rm -f client367
	rm -f server367
