all: clean tcp_server udp_server

tcp_server:
	gcc -o tcp_server tcp_server.c -std=c99

upd_server:
	gcc -o udp_server udp_server.c -std=c99

clean:
	rm -f udp_server tcp_server
