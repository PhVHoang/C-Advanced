#include "HGAPI.h"

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
	}	else 
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