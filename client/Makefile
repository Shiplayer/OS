all: clean tcp_client udp_client
	make -C ./new

tcp_client:
	gcc -o $@ $@.c -std=c99 -w

upd_client:
	gcc -o $@ $@.c -std=c99 -w

clean:
	rm -f udp_client tcp_client
	make clean -C ./new
