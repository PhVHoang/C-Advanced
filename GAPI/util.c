#include "util.h"

extern JRB jval_jrb(Jval v) {

	return (JRB)jval_v(v);
}

Queue makeQueue() {
	return new_dllist();
}

Jval* deQueue(Queue q) {
	Jval* v;
	if (dll_empty(q) || dll_empty(dll_first(q))) v = NULL;
	else {
		v = (Jval*)malloc(sizeof(Jval));
		memcpy(v,&dll_first(q)->val, sizeof(Jval));
		dll_delete_node(dll_first(q));
	}
	return v;
}

void endQueue(Queue l, Jval val, CFUNC cmp) {
	Queue ptr;
	int k = 0;
	dll_traverse(ptr, l)
	if (cmp(ptr->val,val) == 0) k == 1;
	if (k != 1) dll_append(l,val);
}

int emptyQueue(Queue q) {
	return dll_empty(q);
}

Stack makeStack() {

	return new_dllist();
}

Jval* popStack(Stack q) {

	Jval* v;
	if (dll_empty(q) || dll_empty(dll_first(q))) v = NULL;
	else {
		v = (Jval*)malloc(sizeof(Jval));
		memcpy(v, &dll_first(q)->val, sizeof(Jval));
		dll_delete_node(dll_first(q));
	}
	return v;
}

void pushStack(Stack l, Jval val, CFUNC cmp) {
	Stack res;
	int k = 0;
	dll_traverse(res, l)
		if (cmp(res->val,val) == 0) k = 1;
	if (k != 1) dll_append(l,val);
}

int emptyStack(Stack s) {

	return dll_empty(s);
}






