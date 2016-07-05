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

 #include "HGAPI.h"

 Graph CreateGraph()
{
	Graph res;
	res = galloc(Graph);
	res->edge = make_jrb();
	res->vertex = make_jrb();             				  
	return res;
}
Stack stack;
Queue queue;

void printVertex(Jval v) {
	printf("\t%d", jval_i(v));
}

int compareFunc(Jval v1, Jval v2) {
	return jval_i(v1) - jval_i(v2);
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

char *getVertex(Graph graph, int id) {
	if(graph!=NULL) {
		JRB sr = NULL,res = NULL;
		sr = jrb_find_int(graph->edge,id);                    				   
		if(sr!=NULL) {           			    		
			res = jrb_find_int(graph->vertex,id);
			if(res==NULL) return NULL;             				
			else return jval_s(res->val);    
		} else return NULL;
	} else return NULL;
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

int DFS(Graph graph,
				int key, int keyt,int weight,
				int (*func)(Jval,Jval),
				void (*g_print)(Jval))
{
	return DFS_P(graph,key,&keyt,&weight,func,g_print);
}

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
			pushStack(traversed,search->key, compareFunc);
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

int BFS(Graph graph,
        int key, int keyt, int weight,
        int (*func)(Jval, Jval),
        void (*g_print)(Jval))
{
	return BFS_P(graph,key,&keyt,&weight,func,g_print);
}

int BFS_P(Graph graph, int key, int* keyt,int* weight, int (*func)(Jval,Jval), void (*g_print)(Jval)){
	Queue q, tralvered, temp;
	JRB search,link,condition;
	int res = 0;

	q = makeQueue();										 //Khoi tao danh sach cac dinh doi duyet
	tralvered = makeQueue();									  //Khoi tao danh sach dinh da duyet

	endQueue(q,new_jval_i(key), compareFunc); 							  //Khoi tao danh sach chua nut dau tien
	if(g_print) printf("BFS:\t");
	while(!emptyQueue(q)){

		search = jrb_find_int(graph->edge,
							  jval_i(*deQueue(q))); 		     //Tim phan tu chua key trong do thi

		if(search!=NULL){
			endQueue(tralvered,search->key,compareFunc);
			if(g_print) g_print(search->key);

			if(keyt!=NULL)
				if(*keyt==jval_i(search->key))
					if(key!=*keyt) break;

			condition = jval_jrb(search->val);//Lay ra danh sach dinh ke dong thoi la dieu kien NULL
			if(condition){
				jrb_traverse(link,condition){
					int condi = 0;
					dll_traverse(temp, tralvered)
						if(func(link->key,temp->val)==0)
							condi = 1;
					if(condi == 0 && link!=NULL)
						endQueue(q,link->key, compareFunc);
					if(keyt!=NULL)
						if(*keyt==jval_i(link->key))
							if(key == *keyt) res = 1;
				}
			}
		}
		if(res == 1) break;
	}
	if(g_print) printf("\n");
	free(q);
	free(tralvered);
	return res;
}

int indegree(Graph graph, int key,
                      sis* output)
{
  	int i=0;
	JRB tmp, trav,search;
	tmp = graph->edge;
	jrb_traverse(trav,tmp){
		if(jval_i(trav->key)!=key && jval_v(trav->val)!=NULL){
			if((search = jrb_find_int(jval_jrb(trav->val),key)))
				if(output) output[i++].key = trav->key; } }
	return i;
}

int outdegree(Graph graph, int key, sis* output)
{
	JRB tem,trav;
	int i = 0;
	tem = jrb_find_int(graph->edge,key);
	tem = jval_jrb(tem->val);
	if(tem!=NULL){
		jrb_traverse(trav,tem){
			if(output) output[i].key = trav->key;
			i++; } }
	return i;
}

int vertexSize(Graph graph)
{
	JRB tmp,run;
	int size = 0;
	tmp = (JRB)graph->vertex;
	jrb_traverse(run,tmp) size++;
	return size;
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

int hasEdge(Graph graph, int key, int keyt)
{
	int res = 0,rres = 0;

	JRB search = jrb_find_int(graph->edge,key);
	if(search && jval_v(search->val))
		if(jrb_find_int(jval_jrb(search->val),keyt)) res = 1;
	else res = 0;

	search = NULL;

	search = jrb_find_int(graph->edge,keyt);
	if(search && jval_v(search->val))
		if(jrb_find_int(jval_jrb(search->val),key)) rres = 2;
	else rres =0;

	return res + rres;
}

int edgeSize(Graph graph){
  int size = 0;
	JRB trav,tem,subtrav, tmp;
	tem = graph->edge;
	jrb_traverse(trav,tem) {
		if(jval_v(trav->val)) {
			tmp = jval_jrb(trav->val);
			jrb_traverse(subtrav,tmp) size++;
		}
	} return size;
}

void dropGraph(Graph graph)
{
	jrb_free_tree(graph->edge);
	jrb_free_tree(graph->vertex);
	free(graph);
}

void show_connected(Graph g, int key)
{
	sis in[100], out[100];
	int inp,outp,i,j;

	inp = indegree(g,key,in);
	outp = outdegree(g,key,out);
	printf("Connected to %d:",key);
	for(i=0;i<inp;i++) printf("%d\t",jval_i(in[i].key));
	for(j=0;i<outp;j++) printf("%d\t",jval_i(out[j].key));
	printf("\n");
}


int hasVertex(Graph graph, int key){
	if(graph){
		char c;
		printf("Dinh %d khong ton tai ! \nBan co muon them dinh vao do thi khong y/n?",key);
		c = getchar(); 
		if(c == 'y' || c == 'Y'){
			addVertexGen(graph,key,NULL);
		}
	}
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
		for (int j = i; j >= 1; j--) printf("%s\n", getNameByKey(graph, kq[j]));
	}
}

void dis_graph(Graph g){
	JRB trav, tm, mtrav, pedge;
	pedge = g->edge;
	printf("Display all data in graph struct here\n");
	jrb_traverse(trav,pedge){
		printf("Vertex %d:",jval_i(trav->key));
		if(jval_v(trav->val)!=NULL){
			tm = jval_jrb(trav->val);
			jrb_traverse(mtrav,tm)
				printf("\t%d",jval_i(mtrav->key));
		} printf("\n");
	}
}

int findmin(int a, int b) {
	if (a > b) return b;
	else return a;
}


