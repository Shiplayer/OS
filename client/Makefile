all: tcp_client

tcp_client: tcp_client.o
	gcc tcp_client.o -o tcp_client

tcp_client.o:
	gcc -c -g tcp_client.c -o tcp_client.o -std=c99

clean:
	rm -f tcp_client.o tcp_client
