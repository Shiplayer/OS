all: clean task
	make -C ./client
	make -C ./server
	make -C ./files

task: rm_task
	gcc -o task task2.c -std=c99 -w

rm_task:
	rm -f task

clean:
	rm -f task
	make clean -C ./client
	make clean -C ./server
	make clean -C ./files
