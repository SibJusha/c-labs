#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct edge {
    int value;
    int to;
} edge;

typedef struct node {
    int numEdges;
    int capacity;
    struct edge* link;
} node;

typedef struct graph {
    int numVertices;
    struct node* verts;
} graph;

void init_edge(edge* a, int where, int val) {
    a->to = where;
    a->value = val;
}

int find_min(int *arr, int n) {
    int min = INT_MAX;
    int min_ind = -1;
    for (int i = 0; i < n; i++) {
        if (arr[i] < min) {
            min = arr[i];
            min_ind = i;
        }
    }
    return min_ind;
}


void relax(graph *g, int a, int b, int *dis, int *pre, int i) {
    if (dis[b] > dis[a] + g->verts[a].link[i].value) {
        dis[b] = dis[a] + g->verts[a].link[i].value;
        pre[b] = a;
    }
}

void way(int *pre, int a) {
    if (pre[a] == -1) {
        printf("%d ", a);
        return;
    }
    way(pre, pre[a]);
    printf("%d ", a);
}

void dijkstra(graph *g, int start, int where) {
    int *pre = (int *) malloc(g->numVertices * sizeof(int));
    int *dis = (int *) malloc(g->numVertices * sizeof(int));
    int *going = (int *) malloc(g->numVertices * sizeof(int));
    for (int i = 0; i < g->numVertices; i++) {
        pre[i] = -1;
        going[i] = dis[i] = INT_MAX;
    }
    going[start] = dis[start] = 0;
    for (int i = 0; i < g->numVertices; i++) {
        int u = find_min(going, g->numVertices);
        going[u] = INT_MAX;
        for (int j = 0; j < g->verts[u].numEdges; j++) {
            int t = g->verts[u].link[j].to;
            relax(g, u, t, dis, pre, j);
            going[t] = dis[t];
        }
    }
    printf("%d\n", dis[where]);
    way(pre, where);
    free(dis);
    free(pre);
}

int main() {
    int k, n, m;
    scanf("%d %d %d", &k, &n, &m);
    graph *maps = (graph*) malloc(sizeof(graph));
    maps->numVertices = n;
    maps->verts = (node*) malloc(n * sizeof(node));
    for (int i = 0; i < n; i++) {
        maps->verts[i].numEdges = 0;
        maps->verts[i].capacity = 1;
        maps->verts[i].link = (edge*) malloc(sizeof(edge));
    }
    for (int i = 0; i < m; i++) {
        int x, y, t; // from to time
        scanf("%d %d %d", &x, &y, &t);
        if (maps->verts[x].numEdges >= maps->verts[x].capacity) {
            maps->verts[x].link = (edge*) realloc(maps->verts[x].link,
                              2 * maps->verts[x].capacity * sizeof(edge));
            maps->verts[x].capacity *= 2;
        }
        init_edge(&maps->verts[x].link[maps->verts[x].numEdges++], y, t);
    }
    dijkstra(maps, 0, k);
    for (int i = 0; i < maps->numVertices; i++) {
        free(maps->verts[i].link);
    }
    free(maps->verts);
    free(maps);
    return 0;
}
