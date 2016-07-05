#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "dllist.h"
#include "jrb.h"

#define galloc(type) ((type)malloc(sizeof(type))) 

// #define DEBUG

typedef struct {
	JRB vertex;
	JRB edge;
} *Graph;

typedef Dllist Queue;
typedef Dllist Stack;

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

int isAdjacent(Graph g, int v1, int v2) {
	JRB node, subtree;
	node = jrb_find_int(g,v1);
	if (node == NULL) return 0;
	subtree  = (JRB) jval_v(node->val);
	if (jrb_find_int(subtree, v2) == NULL) return 0;
	return 1;
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

int getKeyByVal(Graph graph, char* name) {
	JRB node;

	jrb_traverse(node, graph->vertex) {
		if(strcmp(jval_s(node->val), name) == 0) {
			return jval_i(node->key);
		}
	}

	return -1;
}

char* getNameByKey(Graph graph, int id) {
	if (graph != NULL) {
		JRB node, res;
		node = jrb_find_int(graph->edge,id);
		if (node != NULL) {
			res = jrb_find_int(graph->vertex, id);
			if (res == NULL) return NULL;
			else return (jval_s(res->val));
		} else return NULL;
	} else return NULL;
}

void Readfile_into_graph_n2i(FILE *fp, Graph g) {

	int m,n,i,j,id;
	
	fscanf(fp, "%d %d\n", &n, &m);
	for (i = 0; i < n; i++) {
		char *s;
		s = (char*)malloc(100*sizeof(char));
		fscanf(fp, "%s %d[^\n]\n", s, &id);
		addVertexGen(g,id, s);
	}
	
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

void get_Co_citation(Graph g, int id1, int id2) {
	JRB node_id1 = NULL, node_id2 = NULL;
	node_id1 = jrb_find_int(g->edge, id1);
	node_id2 = jrb_find_int(g->edge, id2);
	if (node_id1 == NULL || node_id2 == NULL) return;
	
	else {
	
		if (jval_v(node_id1->val) != NULL && jval_v(node_id2->val) != NULL) {
			JRB node_tr1, node_tr2, t, tt;
			t = (JRB)jval_v(node_id1->val);
			tt = (JRB)jval_v(node_id2->val);
			jrb_traverse(node_tr1, t) {
				jrb_traverse(node_tr2, tt)
				if (jval_i(node_tr1->key) == jval_i(node_tr2->key))
					printf("%s\n", getNameByKey(g, jval_i(node_tr1->key)));
			}
		}
	}
}

void get_Co_refrence(Graph g, int id1, int id2) {
	JRB subedge , tm ;
	jrb_traverse(subedge, g->edge) {
		if (jval_v(subedge->val) != NULL) {
			tm = (JRB)jval_v(subedge->val);
			if (jrb_find_int(tm,id1) != NULL && jrb_find_int(tm, id2) != NULL) 
			 	printf("%s\n", getNameByKey(g, jval_i(subedge->key)));
		}
	}

}

void get_Odg(Graph g, int k) {
	JRB subedge;
	jrb_traverse(subedge, g->edge) {
		if (jval_v(subedge->val) != NULL) {
			JRB tm = (JRB)jval_v(subedge->val);
			JRB tr;
			int i = 0;
			jrb_traverse(tr, tm) {
				i++;
			} 
			if (i > k) printf("%s\n",getNameByKey(g, jval_i(subedge->key)) );
		}
	}
}

void get_Rel(Graph g, int id) {
	JRB search,tt;
	int index[100001];
	memset(index, 0, sizeof(index));
	index[id] = 1;
	search = jrb_find_int(g->edge, id);
	if (search != NULL) {
		if (jval_v(search->val) != NULL) {
			JRB tm, tmm;
			tmm = (JRB)jval_v(search->val);
			jrb_traverse(tm, tmm) {
				if (index[jval_i(tm->key)] == 0) {
				printf("%s\n", getNameByKey(g, jval_i(tm->key)));
				index[jval_i(tm->key)] = 1;
				}
				tt = jrb_find_int(g->edge, jval_i(tm->key));
				if (tt != NULL) {
					if (jval_v(tt->val) != NULL) {
						JRB mtm, mtmm;
						mtmm = (JRB) jval_v(tt->val);
						jrb_traverse(mtm, mtmm) {
							if (index[jval_i(mtm->key)] == 0) {
								printf("%s\n", getNameByKey(g, jval_i(mtm->key)));
								index[jval_i(mtm->key)] = 1;
							}
						}
					}
				}
			}
		}
	}
}

int main(int argc, char const *argv[])
{
	FILE *fp;

	if (argc == 1 || (strcmp(argv[1],"help") == 0) && (strcmp(argv[0],"./pr") ==0)) {
		printf("Supported command: help,i2n,n2i,cc,cr,odg,rel.\n");
	}
	else if (argc == 1 || strcmp(argv[1],"about") == 0) {
		printf("C advanced 20142 final exam.\n");
	}
	else if (strcmp(argv[0], "./pr") == 0 && strcmp(argv[1], "n2i") == 0 && argv[2] != NULL && argv[3] != NULL) {
		fp = fopen(argv[2], "r+");
		if (fp == NULL) return 0;
		Graph g = CreateGraph();
		Readfile_into_graph_n2i(fp,g);
		printf("%d\n", getKeyByVal(g,argv[3]));
		fclose(fp);
	}
	else if (strcmp(argv[0],"./pr") == 0 && strcmp(argv[1],"i2n") == 0 && argv[2] != NULL && argv[3] != NULL) {
		fp = fopen(argv[2], "r+");
		if (fp == NULL) return 0;
		Graph g = CreateGraph();
		Readfile_into_graph_n2i(fp,g);
		printf("%s\n", getNameByKey(g,atoi(argv[3])));
		fclose(fp);
	} 
	else if (strcmp(argv[0], "./pr") == 0 && strcmp(argv[1],"cc") == 0 && argv[2] != NULL && argv[3] != NULL && argv[4] != NULL) {
		fp = fopen(argv[2],"r+");
		if (fp == NULL) return 0;
		Graph g = CreateGraph();
		Read_all_file(fp,g);
		get_Co_citation(g, atoi(argv[3]), atoi(argv[4]));
		fclose(fp);
	} else if (strcmp(argv[0], "./pr") == 0 && strcmp(argv[1],"cr") == 0 && argv[2] != NULL && argv[3] != NULL && argv[4] != NULL) {
		fp = fopen(argv[2],"r+");
		if (fp == NULL) return 0;
		Graph g = CreateGraph();
		Read_all_file(fp,g);
		get_Co_refrence(g, atoi(argv[3]), atoi(argv[4]));
		fclose(fp);
	} else if (strcmp(argv[0], "./pr") == 0 && strcmp(argv[1],"odg") == 0 && argv[2] != NULL && argv[3] != NULL) {
		fp = fopen(argv[2],"r+");
		if (fp == NULL) return 0;
		Graph g = CreateGraph();
		Read_all_file(fp,g);
		get_Odg(g, atoi(argv[3]));
		fclose(fp);
	}
	 else if (strcmp(argv[0], "./pr") == 0 && strcmp(argv[1],"rel") == 0 && argv[2] != NULL && argv[3] != NULL) {
		fp = fopen(argv[2],"r+");
		if (fp == NULL) return 0;
		Graph g = CreateGraph();
		Read_all_file(fp,g);
		get_Rel(g, atoi(argv[3]));
		fclose(fp);
	}
	return 0;
}




