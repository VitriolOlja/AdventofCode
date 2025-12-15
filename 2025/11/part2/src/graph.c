#include "graph.h"
#include <stdlib.h>


Graph *createGraph(size_t number_of_vertices)
{
  Graph *graph = (Graph *)malloc(sizeof(Graph));
  graph->number_of_vertices = number_of_vertices;
  graph->adjacency_list = (GraphNode **)malloc(number_of_vertices * sizeof(GraphNode *));

  for (int i = 0; i < number_of_vertices; i++)
  {
    graph->adjacency_list[i] = NULL; 
  }
  return graph;
}

void addEdge(Graph *graph, size_t src, size_t dest) {
  // Add edge from src to dest
  GraphNode* node = (GraphNode*) malloc(sizeof(GraphNode));
  node->vertex = dest;
  node->next = graph->adjacency_list[src];
  graph->adjacency_list[src] = node;
}

void free_graph(Graph* graph)
{
  for (int v = 0; v < graph->number_of_vertices; v++) {
    GraphNode* current = graph->adjacency_list[v];
    while (current != NULL) {
      GraphNode* nextone = current->next;
      free(current);
      current = nextone;
    }
  }
  free(graph->adjacency_list);
  free(graph);
}