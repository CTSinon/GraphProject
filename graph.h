#ifndef GRAPH_H
#define GRAPH_H


#define MAX_AUXI

struct graph;

typedef struct {
    /* destructor of the class */
    int (*destroyGraph)(struct graph *);

    /* they are implementations of some basic operations on graph */
    int (*numberOfEdges)(struct graph *);
    int (*numberOfVertices)(struct graph *);
    int *(*graphDFS)(struct graph *, int, int);
    int *(*graphBFS)(struct graph *, int, int);
    int *(*graphDijkstra)(struct graph *, int, int, unsigned long long **);
    float (*closenessCentrality)(struct graph *, int);
} graphVtable;

typedef struct edges {
    int to;
    int nextID;
    int weight;
} graphEdges;


#ifndef GRAPH
#define GRAPH

typedef struct graph {
    graphEdges *_edgeList;
    int *_vertexList;
    int *_involvedVertices;
    int _edgeNum;
    int _vertexNum;
    int _vertexMax;

    graphVtable vtable;
} Graph;

/* point to an instance of Graph class */
Graph *mainGraph;

/* factory function to product graph class from file input */
Graph *initGraph(char *);

#endif
#endif