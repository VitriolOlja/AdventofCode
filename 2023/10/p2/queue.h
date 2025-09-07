#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdbool.h>
#include "dynamic_list.h"



typedef struct QueueNode
{
  int x; // position
  int y; // position
  uint8_t direction;
  size_t steps;
} QueueNode;

typedef struct Queue
{
  QueueNode *data;
  size_t head;
  size_t tail;
  size_t capacity;
} Queue;


void initialize_queue(Queue *queue, size_t cap);
void queue_push(Queue *queue, const QueueNode *push_node);
void queue_pop(Queue *queue, QueueNode *return_node);

bool queue_empty(Queue *queue);
bool queue_full(Queue *queue);

#endif /*QUEUE_H*/