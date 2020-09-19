// Graph.c ... implementation of Graph ADT
// Written by John Shepherd, May 2013

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Graph.h"
#include "Queue.h"

// graph representation (adjacency matrix)
typedef struct GraphRep {
	int nV;		 // #vertices
	int nE;		 // #edges
	int **edges; // matrix of weights (0 == no edge)
} GraphRep;

// check validity of Vertex
int validV (Graph g, Vertex v)
{
	return (g != NULL && v >= 0 && v < g->nV);
}

// make an edge
static Edge mkEdge (Graph g, Vertex v, Vertex w)
{
	assert (g != NULL && validV (g, v) && validV (g, w));
	Edge new = {v, w}; // struct assignment
	return new;
}

// insert an Edge
// - sets (v,w) and (w,v)
void insertEdge (Graph g, Vertex v, Vertex w, int wt)
{
	assert (g != NULL && validV (g, v) && validV (g, w));

	if (g->edges[v][w] != 0 && g->edges[w][v] != 0)
		// an edge already exists; do nothing.
		return;

	g->edges[v][w] = wt;
	g->edges[w][v] = wt;
	g->nE++;
}

// remove an Edge
// - unsets (v,w) and (w,v)
void removeEdge (Graph g, Vertex v, Vertex w)
{
	assert (g != NULL && validV (g, v) && validV (g, w));
	if (g->edges[v][w] == 0 && g->edges[w][v] == 0)
		// an edge doesn't exist; do nothing.
		return;

	g->edges[v][w] = 0;
	g->edges[w][v] = 0;
	g->nE--;
}

// create an empty graph
Graph newGraph (int nV)
{
	assert (nV > 0);

	GraphRep *new = malloc (sizeof *new);
	assert (new != NULL);
	*new = (GraphRep){ .nV = nV, .nE = 0 };

	new->edges = calloc ((size_t) nV, sizeof (int *));
	assert (new->edges != NULL);
	for (int v = 0; v < nV; v++) {
		new->edges[v] = calloc ((size_t) nV, sizeof (int));
		assert (new->edges[v] != 0);
	}

	return new;
}

// free memory associated with graph
void dropGraph (Graph g)
{
	assert (g != NULL);
	for (int v = 0; v < g->nV; v++)
		free (g->edges[v]);
	free (g->edges);
	free (g);
}

// display graph, using names for vertices
void showGraph (Graph g, char **names)
{
	assert (g != NULL);
	printf ("#vertices=%d, #edges=%d\n\n", g->nV, g->nE);
	int v, w;
	for (v = 0; v < g->nV; v++) {
		printf ("%d %s\n", v, names[v]);
		for (w = 0; w < g->nV; w++) {
			if (g->edges[v][w]) {
				printf ("\t%s (%d)\n", names[w], g->edges[v][w]);
			}
		}
		printf ("\n");
	}
}

// find a path between two vertices using breadth-first traversal
// only allow edges whose weight is less than "max"
int findPath (Graph g, Vertex src, Vertex dest, int max, int *path)
{
	assert (g != NULL);
	Vertex i;
    int *visited = malloc(g->nV * sizeof(int));
	//Make all elements in
	for (i = 0;i < g->nV;i++){
		visited[i] = -1;
	}

	// Create a new graph
	Queue Q = newQueue();
    visited[src] = src;
    int foundPath = 0;
	// Add the source to the queue
	QueueJoin (Q, src);

	while (!QueueIsEmpty(Q) && foundPath == 0){
		//Pop the first element from queue
		Vertex currVertex = QueueLeave(Q);

		//We aren't given an adjacency matrix - so we need to use the edge thing given
		// graph->edges[currVertex][currIndex];
        if (currVertex == dest) {
            foundPath = 1;
    		continue;
		}

		for (Vertex j = 0; j < g->nV; j++){
			//This checks if any vertexs in the graph are adjacent to current vertex
			// edges[][] is matrix of weights (0 == no edge)

            // only allow edges whose weight is less than "max"
			if (g->edges[currVertex][j] != 0 && visited[j] == -1 && g->edges[currVertex][j] < max){
                visited[j] = currVertex;
                QueueJoin(Q, j);
		    }
	}

    if (foundPath == 1) {
    Vertex d = dest;
    int len = 0;
    
    while (d != src) {
        //path[p] = d;
        len++;
        d = visited[d];
        //d++;
    }
    len++;
    int p = len - 1;
    d = dest;

    while (p >= 0 ) {
        path[p] = d;
        d = visited[d];
        p--;
    }

    //path[p] = d;
    free(visited);
    return len;
    }
    free(visited);

    return 0;
}
