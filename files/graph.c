#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int forks = 0;

struct stack{
	struct stack* next;
	int data;
};

struct Node {
	int vertex;
	int used;
	int size;
	struct stack *head;
} *root, *nullNode, **nodes;

struct Node *createNode(int vertex){
	struct Node *n = malloc(sizeof(struct Node));
	n->vertex = vertex;
	n->head = malloc(sizeof(struct stack));
	n->used = 0;
	n->size = 0;
	return n;
}

void push(struct Node *node, int data){
	struct stack *buf = malloc(sizeof(struct stack));
	buf->data = data;
	if(node->size == 0)
		node->head->next = buf;
	else {
		struct stack *buf2 = node->head->next;
		node->head->next = buf;
		buf->next = buf2;
	}
	node->size++;
}

int pop(struct Node *node){
	if(node->size == 0)
		return -123;
	int data = node->head->next->data;
	if(node->size == 1)
		node->head->next = NULL;
	else
		node->head->next = node->head->next->next;
	node->size--;
	return data;
}

void dfs(struct Node *node){
	int pipefb[2];
	if(pipe(pipefb) == -1){
		perror("pipe");
		exit(1);
	}
	pid_t pid = fork();
	if(pid == 0){
		if(node->used != 1){
			while(node->size > 0){
				int index = pop(node);
				if(nodes[index]->used == 0){
					int pipefb[2];
					if(pipe(pipefb[2]) == 1){
						perror("pipe");
						exit(EXIT_FAILURE);
					}
					if(pid == 0){
						//close(pipefb[0]);
						printf("parent: %d and child:%d (%d -> %d)\n", getppid(), getpid(), node->vertex, nodes[index]->vertex);
						dfs(nodes[index]);
						//wait(0);
						//write(pipefb[1], forks, sizeof(int));
					} else{
						//close(pipefb[1]);
						//read(pipefb[0], forks, sizeof(int));
					}
				}
			}
		}
		close(pipefb[1]);
		
	} else{
		wait(0);
		return forks + 1;
	}
}
	
int main(int argc, char **argv){
	nullNode = createNode(-1);
	root = createNode(0);
	FILE *fb = fopen("graph.in", "r");
	int u, v, vertexes;
	fscanf(fb, "%d", &vertexes);
	nodes = malloc(sizeof(struct Node) * (vertexes) + 1);
	nodes[0] = root;
	for(int i = 1; i <= vertexes; i++){
		nodes[i] = createNode(i);
	}
	while(fscanf(fb, "%d %d", &u, &v) != EOF){
		//printf("%d, %d", u, v);
		push(nodes[u], v);
	}
	nodes[1]->used = 1;
	while(nodes[1]->size > 0){
		int index = pop(nodes[1]);
		dfs(nodes[index]);
	}
	wait(0);
	return 0;
}
