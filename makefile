client:
	gcc client.c -o client

server:
	gcc server.c -o server

clean:
	rm -f *.o

real_clean: clean
	rm -f client
	rm -f server
