#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "dllist.h"
#include "jrb.h"
#include "util.h"

#define galloc(type) ((type)malloc(sizeof(type))) 
#define VISTITED 1
#define UNVISITED 0

const int infinite = 0x3c3c3c3c;
Queue queue; // Store the toposort inreverse order
Stack stack;
int DFS_NUM[100001];
int StoreTPSort[100001];

typedef struct {
	JRB vertex;
	JRB edge;
} *Graph;

Graph CreateGraph() {

	Graph result;
	result = galloc(Graph);
	result->vertex = make_jrb();
	result->edge = make_jrb();
	return result;
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

void printVertex(Jval v) {
	printf("\t%d", jval_i(v));
}

int compareFunc(Jval v1, Jval v2) {
	return jval_i(v1) - jval_i(v2);
}

void Read_all_file(FILE *fp, Graph g) {

	int m,n,i,val,id;
	fscanf(fp, "%d %d\n", &n, &m);
	for (i = 0; i < n; i++) {
		char *s;
		s = (char*)malloc(100*sizeof(char));
		fscanf(fp,"%s %d[^\n]\n",s,&id);
		addVertexGen(g,id,s);
	}
	for (i = 0; i < m; i++)
	{
		fscanf(fp, "%d %d[^\n]\n", &id, &val);
		addEdge(g,id,val,1);
		#ifdef DEBUG
			printf("%d %d\n", id, val);
		#endif
	}
}

int DFS(Graph graph,
				int key, int keyt,int weight,
				int (*func)(Jval,Jval),
				void (*g_print)(Jval))
{
	return DFS_P(graph,key,&keyt,&weight,func,g_print);
}

// DFS with point keyt
int DFS_P(Graph graph,
				int key, int *keyt, int* weight,
				int (*func)(Jval,Jval),
				void (*g_print)(Jval))
{
	JRB search, condition, tmp;
	Stack stack, traversed, tm;
	int res = 0;

    stack = makeStack(); 						  					  // Ngan xep cac dinh doi duyet
	traversed = makeStack(); 									   //Danh sach xep cac dinh da duyet

	pushStack(stack,new_jval_i(key), compareFunc);								//Dua nut dau tien vao danh sach
	if(g_print) printf("DFS:\t");
	while(!emptyStack(stack))
	{
		search = jrb_find_int(graph->edge,
                  			  jval_i(*popStack(stack)));   //Tim khoa key trong danh sach luu do thi
		if(search!=NULL)
		{
			pushStack(traversed,search->key,compareFunc);
			if(g_print) g_print(search->key);           				   	  //In ra dinh duyet qua

			if(keyt!=NULL)
				if(*keyt==jval_i(search->key))
					if(key!=*keyt) break;

			condition = jval_jrb(search->val);//Lay ra danh sach dinh ke dong thoi la dieu kien NULL
      		if(condition){
				jrb_traverse(tmp,condition)	{
					int k = 0;
					dll_traverse(tm,traversed)
						if(func(tmp->key,tm->val)==0)
            				k = 1;
					if(k == 0 && tmp!=NULL)
						pushStack(stack,tmp->key, compareFunc);
					if(keyt!=NULL)
						if(*keyt==jval_i(tmp->key))
							if(key == *keyt) res = 1;
				}
    		}
		}
		if(res==1) break;
	}
	if(g_print) printf("\n");
	free(stack);
	free(traversed);
	return res;
}

int checkCycle(Graph g,int (*func)(Jval, Jval)){
	int res = 0;
	JRB tmp,trav;
	tmp = g->vertex;
	jrb_traverse(trav,tmp){
		res += DFS(g,jval_i(trav->key),
								 jval_i(trav->key),0,func,NULL);
	}
	return res;
}

void DFS2(Graph graph, int u) {
	JRB find = jrb_find_int(graph->edge, u);
	if (find != NULL) {
		DFS_NUM[u] = VISTITED;
		JRB tmp, rev;
		rev = jval_jrb(find->val);
		if (rev != NULL) {
			jrb_traverse(tmp, rev) {
				if (DFS_NUM[jval_i(tmp->key)] == UNVISITED) DFS2(graph, jval_i(tmp->key));
			}
		}
	}
	pushStack(stack, new_jval_i(u), compareFunc);
}

void topologycal(Graph graph) {
	// queue = makeQueue();
	stack = makeStack();
	for (int i = 0; i < 100001; i++)  {
			DFS_NUM[i] = UNVISITED;
	}
	if (checkCycle(graph, compareFunc) != 0) {
		printf("This graph is DAG\n");
		return;
	} 
	else {
		
		JRB tmp,rev;
		if (graph->edge != NULL) {
			jrb_traverse(tmp, graph->edge) {
				if (DFS_NUM[jval_i(tmp->key)] == UNVISITED)
					DFS2(graph, jval_i(tmp->key));
			}
		}
	}
	Jval *w = (Jval*)malloc(sizeof(Jval));
	int i = 0;
	while (!emptyStack(stack)) {
			w = popStack(stack);
			// pushStack(stack, *w, compareFunc);
			StoreTPSort[i] = jval_i(*w);
			i++;
			// printf("%d\t", jval_i(*w));
	}
	for (int j = i-1; j >=0; j--)
	{
		printf("%d\t", StoreTPSort[j]);
	}
	printf("\n");
}

int main(int argc, char const *argv[])
{
	FILE *fp;
	if (strcmp(argv[0], "./pr") == 0 && strcmp(argv[1], "gettp") == 0 && argv[2]) {
		fp = fopen(argv[2], "r+");
		if (fp == NULL) return 0;
		Graph g = CreateGraph();
		Read_all_file(fp, g);
		topologycal(g);
		fclose(fp);
	}
	return 0;
}

