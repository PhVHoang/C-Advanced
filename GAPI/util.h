#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "jval.h"
#include "jrb.h"
#include "dllist.h"

#define galloc(type) ((type)malloc(sizeof(type)))
typedef int (*CFUNC)(Jval, Jval);
typedef void (*GDIS) (Jval);

typedef struct {
	Jval key;
	Jval val;
}sis;

typedef Dllist Queue;
typedef Dllist Stack;

extern Queue makeQueue();
extern Jval* deQueue(Queue);
extern void endQueue(Queue , Jval, CFUNC);
extern int emptyQueue(Queue);

extern Jval* popStack(Stack);
extern void pushStack(Stack, Jval, CFUNC);
extern Stack makeStack();
extern int emptyStack(Stack);

extern JRB jval_jrb(Jval);

#endif