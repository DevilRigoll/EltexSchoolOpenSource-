#include <stdlib.h>
#include <string.h>
#include "queue.h"

Queue initQ() {
	Queue q;
	q.size = 0;
	q.head = q.tail = NULL;
	return q;
}

void qPushBack(Queue * q, Msg msg) {
	
	struct Node * n = (struct Node *)malloc(sizeof(struct Node));
	n->elem = msg;
	n->next = NULL;

	if (q->size == 0) {
		q->head = q->tail = n;
		q->size = 1;
		return;
	}
	
	q->size++;
	q->tail->next = n;
	q->tail = n;
}

void qPopFront(Queue * q, Msg * msg) {
	if (q->head == NULL)
		return;

	q->size--;
	struct Node * n = q->head;
	q->head = q->head->next;
	(*msg) = n->elem;

	free(n);	
}

void qClear(Queue * q) {
	if (q->head == NULL)
		return;

	while(q->head != NULL) {
		struct Node * n = q->head;
		q->head = q->head->next;
		free(n);
	}
	
}


