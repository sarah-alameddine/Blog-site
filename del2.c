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

static int realPath(Vertex src, Vertex dest, int *visited, int nV, int *path);
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

// looks through visited and finds he shortest path
//then adds this path to array path to store it
static int realPath(Vertex src, Vertex dest, int *visited, int nV, int *path) {

    // Record the route in a queue.
    Queue route = newQueue();
    // Backtrack through the visited array, recording each vertex in the queue.
    int current = dest;
    int hops = 0;
    while(current != src) {
            QueueJoin(route, current);
            current = visited[current];
            hops++;
    }
    // Add the source to the route.
    QueueJoin(route, src);
    hops++;

    // Convert the queue into an array of vertices in order from src to dest.
    for (int i = hops - 1; i >= 0; i--) {
        path[i] = QueueLeave(route);
    }
    free(visited);
    dropQueue(route);
    return hops;
}

// find a path between two vertices using breadth-first traversal
// only allow edges whose weight is less than "max"
int findPath(Graph g, Vertex src, Vertex dest, int max, int *path)
{
    assert(g != NULL);
    // Special case: if travelling to the starting destination, just return it.
    if (src == dest) {
            path[0] = src;
            return 1;
    }
    // Create a queue of nodes to visit
    Queue Q = newQueue();
     //add the src first into the queue
    QueueJoin(Q, src);

    // create a parent array and store eaxh element as -1
    int *visited = malloc(sizeof(int)*g->nV);
    int v;
    for(v = 0; v < g->nV; v++) {
            visited[v] = -1;
    }

   //Go through the queue
    while (!QueueIsEmpty(Q)) {
        // Take next vertex to visit from the queue.
        Vertex curr = QueueLeave(Q);

        // If this vertex is the destination, we're done, so return the path.
        if (curr == dest) {
                int nV = g->nV;
                dropQueue(Q);
                return realPath(src, dest, visited, nV, path);
        }

        // Loop through the children of the current vertex.
        int child;
        for (child = 0; child < g->nV; child++) {
            //check if there is no edge,if distance to the vertex is greater than the plane can fly
            //Also check if the vertex doesnt have -1 ie if the vertex has already been visited
            if((g->edges[curr][child] == 0) || (g->edges[curr][child] > max)
            || (visited[child] != -1) ){
                //dont want to add these exceptions to queue
            } else{
                 // Add the child to the queue.
                QueueJoin(Q, child);

                //find the index of the child in the array and add the parent node as its element
                visited[child] = curr;
            }
       }
    }
    dropGraph (g);
    dropQueue(Q);
    for(int i = 0; i < g->nV; ++i)
	    free(visited[i]);
    free(visited);
    // If there is no path, return a length of 0.
    return 0;
}
