#include "HGAPI.h"
// #define DEBUG

void read_all_file(FILE *fp, Graph graph) {
	int n,m,i,val,id,w;
	fscanf(fp, "%d\n", &n);
	for (i = 0; i < n; i++) {
		char *s;
		s = (char*)malloc(20*sizeof(char));
		fscanf(fp,"%d %s[^\n]\n",&id, s);
		addVertexGen(graph, id, s);
	}
	fscanf(fp,"%d\n", &m);
	for (i = 0; i < m; i++) {
		fscanf(fp, "%d %d %d[^\n]\n", &id, &val, &w);
		addEdge(graph,id, val, w);
		addEdge(graph, val, id, w);
	}

}


int print_w(Graph graph, int id, int val) {
	JRB tmp, rev;
	if (graph != NULL && id != val) {
		rev = jrb_find_int(graph->edge, id);
		if (rev != NULL) 
			tmp = jrb_find_int(jval_jrb(rev->val),val);
			if (tmp == NULL) return -1;
			else return jval_i(tmp->val);
    }
	
}

void print_w2(Graph graph, int id, int val) {
	JRB tmp, rev;
	if (graph != NULL && id != val) {
		rev = jrb_find_int(graph->edge, id);
		if (jval_jrb(rev->val) != NULL) {
			tmp = jrb_find_int(jval_jrb(rev->val),val);
			if (tmp == NULL) {
				printf("-1\n");
				return;

			}
			else printf("%d\n",jval_i(tmp->val)) ;
		}
		else {
			printf("-1\n");
			return;
		}
    }
	
}

void print_n(Graph graph, int id) {
	if (graph != NULL ) {
		int count = 0;
		JRB rev, tmp;
		rev = jrb_find_int(graph->edge, id);
		if (jval_jrb(rev->val) !=NULL) {
			jrb_traverse(tmp, jval_jrb(rev->val)) {
				count++;
			}
			printf("%d\n", count);
			jrb_traverse(tmp, jval_jrb(rev->val)) {
				printf("%d %s %d\n",jval_i(tmp->key), getNameByKey(graph, jval_i(tmp->key)),print_w(graph,id, jval_i(tmp->key)));
			}
		}
		else return;
	}
}
void print_v(Graph graph) {
	JRB tmp;
	if (graph != NULL) {
		if (graph->vertex != NULL) {
			jrb_traverse(tmp, graph->vertex) {
				printf("%d %s\n", jval_i(tmp->key), getNameByKey(graph,jval_i(tmp->key)));
			}
		}
	}
}

int main(int argc, char const *argv[])
{
	FILE *fp;
	if (argv[0] != NULL && strcmp(argv[1], "-h") == 0)
	{
		printf("C-Advanced, HK20152\n");
	}
	else if (argv[0] != NULL && strcmp(argv[1],"-w") == 0 && argv[2] != NULL && argv[3] != NULL && argv[4] != NULL) {
		fp = fopen(argv[2], "r+");
		if (fp == NULL) return 0;
		Graph g  = CreateGraph();
		read_all_file(fp,g);
		// printf("%d\n", print_w(g, atoi(argv[3]), atoi(argv[4])));
		print_w2(g, atoi(argv[3]), atoi(argv[4]));
		fclose(fp);
	} else if (argv[0] != NULL && strcmp(argv[1],"-p") == 0 && argv[2] != NULL && argv[3] != NULL && argv[4] != NULL) {
		int a = atoi(argv[4]);
		int b = atoi(argv[3]);
		fp = fopen(argv[2], "r+");
		if (fp == NULL) return 0;
		Graph g  = CreateGraph();
		read_all_file(fp,g);
		Dijkstra_heap(g, b,&a); 
		fclose(fp);
	}
	else if (argv[0] != NULL && strcmp(argv[1],"-n") == 0 && argv[2] != NULL && argv[3] != NULL) {
		fp = fopen(argv[2], "r+");
		if (fp == NULL) return 0;
		Graph g  = CreateGraph();
		read_all_file(fp,g);
		print_n(g, atoi(argv[3]));
		fclose(fp);
	}
	else if (argv[0] != NULL && strcmp(argv[1], "-s") == 0 && argv[2] != NULL) {
		int i = 3;
		int j = 0;
		int StoreArgv[10000];
		int count_s = 0;
		while (1) {
			StoreArgv[j] = atoi(argv[i]);
			i++;
			j++;
			if (argv[i] == NULL) break;
		}
		printf("%d\n",j);
		fp = fopen(argv[2], "r+");
		if (fp == NULL) return 0;
		Graph g  = CreateGraph();
		read_all_file(fp,g);
		int k,l;
		for (k = 0; k < j; k++) {
			printf("%d %s\n", StoreArgv[k],getNameByKey(g, StoreArgv[k]));
		}
		JRB rev;
		for (k = 0; k < j-1; k++) 
		{
			JRB find = jrb_find_int(g->edge, StoreArgv[k]);
			for (l = k+1; l < j; l++) {
				if (k != l) {
					rev = jval_jrb(find->val);
					if (rev != NULL)
					{
						if (jrb_find_int(rev, StoreArgv[l]) != NULL) count_s++;
					}
				}
			}
			
		}
		printf("%d\n", count_s);
		for (k = 0; k < j-1; k++) 
		{
			JRB find = jrb_find_int(g->edge, StoreArgv[k]);
			for (l = k+1; l < j; l++) {
				if (k != l) {
					rev = jval_jrb(find->val);
					if (rev != NULL)
					{
						if (jrb_find_int(rev, StoreArgv[l]) != NULL) 
							printf("%d %d %d\n", StoreArgv[k], StoreArgv[l], print_w(g, StoreArgv[k], StoreArgv[l]));
					}
				}
			}
			
		}
	} else if (argv[0] != NULL && strcmp(argv[1],"-v") == 0 && argv[2] != NULL) {
		fp = fopen(argv[2], "r+");
		if (fp == NULL) return 0;
		Graph g  = CreateGraph();
		read_all_file(fp,g);
		print_v(g);
		fclose(fp);
		}
		
	return 0;
}






