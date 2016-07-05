TARGET = p
CC = gcc
OBJS = ck.o HGAPI.o jrb.o dllist.o jval.o util.o minheap.o

${TARGET}: ${OBJS}
		${CC} -o ${TARGET} ${OBJS}
main:
	cc - c ck.c
graph:
	cc -c HGAPI.c
jval:
	cc -c jval.c
jrb:
	cc -c jrb.c
dllist:
	cc -c dllist.c
util:
	cc -c util.c
minheap:
	cc - c minheap.c
clean:
	rm -f *.o ${TARGET}