#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include "dllist.h"
#include "jrb.h"
#include "minheap.h"
// #include "heap.h"

#define galloc(type) ((type)malloc(sizeof(type))) 

// #define DEBUG

typedef struct {
	JRB vertex;
	JRB edge;
} *Graph;

typedef Dllist Queue;
typedef Dllist Stack;
typedef int (*CompareFunction)(Jval, Jval);
struct heapStruct *h;
int trace[1000], dx[1000], d[1000], tr[1000], kq[1000];

Graph CreateGraph() {

	Graph result;
	result = galloc(Graph);
	result->vertex = make_jrb();
	result->edge = make_jrb();
	return result;
}

extern JRB jval_jrb(Jval v) {
	return (JRB)jval_v(v);
}

void addEdge(Graph g, int v1, int v2, int weight) {

	if (g != NULL && v1 != v2) {
		if (g->edge != NULL) {
			JRB search, subtree;
			search = jrb_find_int(g->edge, v1);
			if (search == NULL) {
				subtree = make_jrb();
				jrb_insert_int(subtree, v2, new_jval_i(weight));
				jrb_insert_int(g->edge, subtree, new_jval_v(subtree));
			}
			else {
				if (jval_v(search->val) == NULL) {
					search->val = new_jval_v(make_jrb());
					jrb_insert_int(jval_jrb(search->val),v2, new_jval_i(weight));

				}
				else {
					subtree = jrb_find_int(jval_jrb(search->val), v2);
					if (subtree == NULL) 
						jrb_insert_int(jval_jrb(search->val), v2, new_jval_i(weight));
				}
			}

			search = jrb_find_int(g->edge, v2);
			if (search == NULL)	
				jrb_insert_int(g->edge, v2, new_jval_v(NULL));
		}
		else {

			JRB new = make_jrb();
			g->edge = make_jrb();
			jrb_insert_int(new, v2, new_jval_v(NULL));
			jrb_insert_int(g->edge, v2, new_jval_v(new));
		}
		if (g->vertex != NULL) {
			if (jrb_find_int(g->vertex, v1) == NULL) 
				jrb_insert_int(g->vertex, v1, new_jval_v(NULL));
			if (jrb_find_int(g->vertex, v2) == NULL)
				jrb_insert_int(g->vertex, v2, new_jval_v(NULL));
		} else {
			g->vertex = make_jrb();
			jrb_insert_int(g->vertex, v1, new_jval_v(NULL));
			jrb_insert_int(g->vertex, v2, new_jval_v(NULL));
		}
	}
} 

void addVertexGen(Graph graph, int id, char* name) {

	JRB tem = NULL;
	tem = jrb_find_int(graph->vertex, id);
	if (tem != NULL) tem->val = new_jval_v(name);
	else 
		jrb_insert_int(graph->vertex, id, new_jval_v(name));
	JRB temm = NULL;
	temm = jrb_find_int(graph->edge, id);
	if (temm == NULL) jrb_insert_int(graph->edge, id, new_jval_v(NULL));
}

void Read_all_file(FILE *fp, Graph g) {

	int m,n,i,val,id,w;
	fscanf(fp, "%d %d\n", &n, &m);
	for (i = 0; i < n; i++) {
		char *s;
		s = (char*)malloc(100*sizeof(char));
		fscanf(fp,"%s %d[^\n]\n",s,&id);
		addVertexGen(g,id,s);
	}
	for (i = 0; i < m; i++)
	{
		fscanf(fp, "%d %d %d[^\n]\n", &id, &val, &w);
		addEdge(g,id,val,w);
		#ifdef DEBUG
			printf("%d %d\n", id, val);
		#endif
	}
}

void Dijkstra_heap(Graph graph, int s, int *t) {

	JRB jrb_s, jrb_t;
	
	jrb_s = jrb_find_int(graph->edge, s);
	jrb_t = jrb_find_int(graph->edge, *t);
	if (jrb_s == NULL || jrb_t == NULL) return;
	// memset(d, INT_MAX, sizeof(d));
	// memset(dx, 0, sizeof(dx));
	for (int i = 0; i < 1000; i++) d[i] = INT_MAX;
		for (int i = 0; i < 1000; i++) dx[i] = 0;

	h = initHeap();
	insert(h,s);
	do {
		int u = removeMin(h);
		if (u == *t) break;
		dx[u] = 1;
		JRB jrb_u = jrb_find_int(graph->edge, u);
		JRB tm_u = jval_jrb(jrb_u->val);
		JRB tmm_u;
		if (tm_u != NULL) {
			jrb_traverse(tmm_u,tm_u) {
				if (dx[jval_i(tmm_u->key)] == 0 && d[jval_i(tmm_u->key)] > d[u] + jval_i(tmm_u->val))
				{
					d[jval_i(tmm_u->key)] = d[u] + jval_i(tmm_u->val);
					tr[jval_i(tmm_u->key)] = u;
					insert(h, jval_i(tmm_u->key));
					heapify(h);
				}					
			}
		}
	} while (h->size > 0);

	if (d[*t] == INT_MAX) return;
	else {
		int i = 0;
		while (*t != s) {
			i++;
			kq[i] = *t;
			*t = tr[*t];
		}
		i++;
		kq[i] = s;
		
		int sum_min = 0;
		for (int j = i; j >= 2; j--) {
			JRB tmm_j = jrb_find_int(graph->edge, kq[j]);
			JRB adj_j = jrb_find_int(jval_jrb(tmm_j->val), kq[j-1]);
			sum_min += jval_i(adj_j->val);
		}
		printf("%d\n", sum_min);
		for (int j = i; j >= 1; j--) printf("%d\t", kq[j]);
		printf("\n");
	}
}

int main(int argc, char const *argv[])
{	
	FILE *fp;
	if (strcmp(argv[0], "./pr") == 0 && strcmp(argv[1],"djk") == 0 && argv[2] != NULL && argv[3] != NULL && argv[4] != NULL) {
		fp = fopen(argv[2], "r+");
		if (fp == NULL) return 0;
		Graph g = CreateGraph();
		Read_all_file(fp,g);
		int a = atoi(argv[3]), b = atoi(argv[4]);
		Dijkstra_heap(g, a, &b);
		fclose(fp);
	}
	return 0;
}




