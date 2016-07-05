/*	
 * Created by Pham Van Hoang 
 * 20141812_HEDSPi_K59_HUST
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
*/

#ifndef _HGAPI_H_
#define _HGAPI_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include "dllist.h"
#include "jrb.h"
#include "util.h"
#include "minheap.h"
#include "jval.h"

#define galloc(type) ((type)malloc(sizeof(type))) 
#define VISTITED 1
#define UNVISITED 0

struct heapStruct *h;

int trace[1000], dx[1000], d[1000], tr[1000], kq[1000];

int DFS_NUM[100001];
int StoreTPSort[100001];

typedef struct {
	JRB vertex;
	JRB edge;
} *Graph;

extern Graph CreateGraph();
extern int isAdjacent(Graph , int, int);
extern void addEdge(Graph , int, int, int);
extern void addVertexGen(Graph Graph, int id, char* name);

int checkCycle(Graph, int (*func)(Jval, Jval));

extern int DFS(Graph, int, int, int, int (*func)(Jval, Jval),void (*g_print)(Jval));
extern int DFS_P(Graph, int, int*, int*, int (*func)(Jval, Jval),void (*g_print)(Jval));

extern int BFS(Graph, int, int, int, int (*func)(Jval, Jval),void (*g_print)(Jval));
extern int BFS_P(Graph, int, int*, int*, int (*func)(Jval, Jval),void (*g_print)(Jval));

extern void dis_graph(Graph);
extern char *getVertex(Graph, int);
extern int indegree(Graph, int, sis*);
extern int outdegree(Graph, int, sis*);
extern int vertexSize(Graph);
extern int hasVertex(Graph, int);

extern int hasEdge(Graph, int, int);
extern int edgeSize(Graph);
extern int adjacent(Graph, int, int, sis*);

extern void show_connected(Graph, int);
extern void deleteVertex(Graph, int);

extern void Dijkstra_heap(Graph, int, int*);

extern void DFS2(Graph, int);
extern void topologycal(Graph);

extern void dropGraph(Graph);
extern void printVertex(Jval);
extern int compareFunc(Jval, Jval);
extern int findmin(int , int);
extern char* getNameByKey(Graph , int );

#endif









