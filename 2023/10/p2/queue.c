#include "queue.h"

// it initializes to the smallest power of two greater than or equal to cap.
void initialize_queue(Queue *queue, size_t cap)
{
  queue->head = 0; // for popping
  queue->tail = 0; // for pushing

  size_t size = 1;
  while (size < cap)
  {
    size <<= 1;
  }
  queue->capacity = size;
}

// we should prob full check this.
void queue_push(Queue *queue, const QueueNode *push_node)
{
  QueueNode *slot = &queue->data[queue->tail];
  slot->x = push_node->x;
  slot->y = push_node->y;
  queue->tail = (queue->tail + 1) & (queue->capacity - 1);
}

// we should prob empty check this.
void queue_pop(Queue *queue, QueueNode *return_node)
{
  *return_node = queue->data[queue->head];
  queue->head = (queue->head + 1) & (queue->capacity - 1);
}

bool queue_empty(Queue *queue)
{
  return queue->head == queue->tail;
}

bool queue_full(Queue *queue)
{
  return ((queue->tail + 1) & (queue->capacity - 1)) == queue->head;
}