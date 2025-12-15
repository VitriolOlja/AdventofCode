#ifndef GRAPH_H
#define GRAPH_H
#include <stdint.h>
#include <stddef.h>

typedef struct GraphNode
{
  size_t vertex;
  struct GraphNode *next;
} GraphNode;

typedef struct Graph
{
  size_t number_of_vertices;
  GraphNode **adjacency_list;
} Graph;

Graph *createGraph(size_t number_of_vertices);

void addEdge(Graph *graph, size_t src, size_t dest);

void free_graph(Graph* graph);

#endif /* GRAPH_H */