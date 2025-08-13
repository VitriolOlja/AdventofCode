
/**
 *
    | is a vertical pipe connecting north and south.
    - is a horizontal pipe connecting east and west.
    L is a 90-degree bend connecting north and east.
    J is a 90-degree bend connecting north and west.
    7 is a 90-degree bend connecting south and west.
    F is a 90-degree bend connecting south and east.
    . is ground; there is no pipe in this tile.
    S is the starting position of the animal; there is a pipe on this tile, but your sketch doesn't show what shape the pipe has.
 */
// 8 different values means we need 3 bits to encode these.
// or we just use chars :)

#define DIR_NORTH (1)
#define DIR_EAST (1 << 1)
#define DIR_SOUTH (1 << 2)
#define DIR_WEST (1 << 3)

#define VERTICAL_PIPE (DIR_NORTH | DIR_SOUTH)
#define HORIZONTAL_PIPE (DIR_EAST | DIR_WEST)
#define NORTH_EAST_PIPE (DIR_NORTH | DIR_EAST)
#define NORTH_WEST_PIPE (DIR_NORTH | DIR_WEST)
#define SOUTH_WEST_PIPE (DIR_SOUTH | DIR_WEST)
#define SOUTH_EAST_PIPE (DIR_SOUTH | DIR_EAST)
#define GROUND_NO_PIPE 0
#define STARTING_POSITION 0 // encode as something?

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct MemoryArena
{
  void *data;
  size_t capacity;
} MemoryArena;

typedef struct QueueNode
{
  int x; // position
  int y; // position
  int prev_x;
  int prev_y;
  size_t steps;

} QueueNode;

typedef struct Queue
{
  QueueNode *data;
  size_t head;
  size_t tail;
  size_t capacity;
} Queue;

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
void queue_push(Queue *queue, const QueueNode n)
{
  queue->data[queue->tail] = n;
  queue->tail = (queue->tail + 1) & (queue->capacity - 1);
}

// we should prob empty check this.
QueueNode queue_pop(Queue *queue)
{
  QueueNode ret = queue->data[queue->head];
  queue->head = (queue->head + 1) & (queue->capacity - 1);
  return ret;
}

bool queue_empty(Queue *queue)
{
  return queue->head == queue->tail;
}

bool queue_full(Queue *queue)
{
  return ((queue->tail + 1) & (queue->capacity - 1)) == queue->head;
}

// I am thinking a graph searching algorithm. a bfs or dfs.

int main(int argc, char *argv[])
{

  if (argc < 2)
  {
    printf("Usage: %s filename\n", argv[0]);
    return 1;
  }

  const char *file_name = argv[1];

  FILE *fptr = fopen(file_name, "r");
  if (!fptr)
  {
    printf("Failed to open file %s", file_name);
    return 1;
  }

  int ch;
  uint32_t column_count = 0;
  while ((ch = fgetc(fptr)) != EOF && ch != '\n')
  {
    column_count++;
  }
  uint32_t row_count = 2;
  while ((ch = fgetc(fptr)) != EOF)
  {
    if (ch == '\n')
    {
      row_count++;
    }
  }

  rewind(fptr);
  // printf("Columns: %" PRIu32 " Rows: %" PRIu32 "\n", column_count, row_count );

  size_t queue_cap = column_count * row_count;
  size_t size = 1;
  while (size < queue_cap)
  {
    size <<= 1;
  }
  char *memory_arena = malloc(column_count * row_count * sizeof(char)        // map elements
                              + row_count * sizeof(char *)                   // map
                              + size * sizeof(QueueNode) // worst case is queue gets filled with all at the same time.
  );

  if (!memory_arena)
  {
    printf("Failed to allocate memory arena. ");
  }

  char **map = (char **)(memory_arena + column_count * row_count);
  for (uint32_t i = 0; i < row_count; i++)
  {
    map[i] = &memory_arena[i * column_count];
  }

  // now lets read the file.
  int start_x, start_y;
  int row = 0;
  int column = 0;
  while ((ch = fgetc(fptr)) != EOF)
  {
    if (ch == '\n')
    {
      row++;
      column = 0;
      continue;
    }
    if (ch == 'S')
    {
      start_x = column;
      start_y = row;
    }
    map[row][column] = (char)ch;

    column++;
  }

  // for (int i = 0; i < (int)row_count; i++) {
  //   for (int j = 0; j < (int)column_count; j++) {
  //     printf("%c", map[i][j]);
  //   }
  //   printf("\n");
  // }

  QueueNode current;

  Queue queue;
  initialize_queue(&queue, row_count * column_count);
  queue.data = (QueueNode *)(memory_arena + (row_count * column_count * sizeof(char)) // map elements
                             + (row_count * sizeof(char *)));                         // map

  // Up
  if (start_y > 0)
  {
    queue_push(&queue, (QueueNode){.x = start_x, .y = start_y - 1, .prev_x = start_x, .prev_y = start_y, .steps = 1});
  }
  // Down
  if (start_y < (int)(row_count - 1))
  {
    queue_push(&queue, (QueueNode){.x = start_x, .y = start_y + 1, .prev_x = start_x, .prev_y = start_y, .steps = 1});
  }
  // left
  if (start_x > 0)
  {
    queue_push(&queue, (QueueNode){.x = start_x - 1, .y = start_y, .prev_x = start_x, .prev_y = start_y, .steps = 1});
  }
  // right
  if (start_x < (int)(column_count - 1))
  {
    queue_push(&queue, (QueueNode){.x = start_x + 1, .y = start_y, .prev_x = start_x, .prev_y = start_y, .steps = 1});
  }

  int biggest_loop = 0;

  while (!queue_empty(&queue))
  {
    current = queue_pop(&queue);

    if (map[current.y][current.x] == 'S')
    {
      biggest_loop = (int)current.steps;
      printf("Found a loop, on %lld steps \n", current.steps);
      continue;
    }

    //for (int i = 0; i < (int)row_count; i++)
    //{
    //  for (int j = 0; j < (int)column_count; j++)
    //  {
    //    if (current.x == j && current.y == i)
    //    {
    //      printf("%lld", current.steps);
    //    }
    //    else
    //    {
    //      printf("%c", map[i][j]);
    //    }
    //  }
    //  puts("\n");
    //}
    //puts("\n\n");

    int dir_x = current.x - current.prev_x;
    int dir_y = current.y - current.prev_y;

    // Up
    if (dir_y == -1)
    {
      // check for F | or 7
      if (map[current.y][current.x] == 'F')
      {
        // push east
        if (current.x < (int)(column_count - 1))
        {
          queue_push(&queue, (QueueNode){.x = current.x + 1, .y = current.y, .prev_x = current.x, .prev_y = current.y, .steps = current.steps + 1});
        }
      }
      else if (map[current.y][current.x] == '|')
      {
        // push up
        queue_push(&queue, (QueueNode){.x = current.x, .y = current.y - 1, .prev_x = current.x, .prev_y = current.y, .steps = current.steps + 1});
      }
      else if (map[current.y][current.x] == '7')
      {
        // push weest
        queue_push(&queue, (QueueNode){.x = current.x - 1, .y = current.y, .prev_x = current.x, .prev_y = current.y, .steps = current.steps + 1});
      }
    }
    // Down
    else if (dir_y == 1)
    {
      // Check for | J or L
      if (map[current.y][current.x] == '|')
      {
        // push down
        queue_push(&queue, (QueueNode){.x = current.x, .y = current.y + 1, .prev_x = current.x, .prev_y = current.y, .steps = current.steps + 1});
      }
      else if (map[current.y][current.x] == 'J')
      {
        // push west
        queue_push(&queue, (QueueNode){.x = current.x - 1, .y = current.y, .prev_x = current.x, .prev_y = current.y, .steps = current.steps + 1});
      }
      else if (map[current.y][current.x] == 'L')
      {
        // push east
        queue_push(&queue, (QueueNode){.x = current.x + 1, .y = current.y, .prev_x = current.x, .prev_y = current.y, .steps = current.steps + 1});
      }
    }
    // Left
    else if (dir_x == -1)
    {
      // check for L - F
      if (map[current.y][current.x] == 'L')
      {
        // push up
        queue_push(&queue, (QueueNode){.x = current.x, .y = current.y - 1, .prev_x = current.x, .prev_y = current.y, .steps = current.steps + 1});
      }
      else if (map[current.y][current.x] == '-')
      {
        // push left
        queue_push(&queue, (QueueNode){.x = current.x - 1, .y = current.y, .prev_x = current.x, .prev_y = current.y, .steps = current.steps + 1});
      }
      else if (map[current.y][current.x] == 'F')
      {
        // push down
        queue_push(&queue, (QueueNode){.x = current.x, .y = current.y + 1, .prev_x = current.x, .prev_y = current.y, .steps = current.steps + 1});
      }
    }
    // Right
    else if (dir_x == 1)
    {
      // checkfor J, -, or 7
      if (map[current.y][current.x] == 'J')
      {
        // push up
        queue_push(&queue, (QueueNode){.x = current.x, .y = current.y - 1, .prev_x = current.x, .prev_y = current.y, .steps = current.steps + 1});
      }
      else if (map[current.y][current.x] == '-')
      {
        // push right
        queue_push(&queue, (QueueNode){.x = current.x + 1, .y = current.y, .prev_x = current.x, .prev_y = current.y, .steps = current.steps + 1});
      }
      else if (map[current.y][current.x] == '7')
      {
        // push down
        queue_push(&queue, (QueueNode){.x = current.x, .y = current.y + 1, .prev_x = current.x, .prev_y = current.y, .steps = current.steps + 1});
      }
    }
    else
    {
      printf("DEBUG: Unexpected direction at (%d,%d). Prev: (%d,%d), dir_x=%d, dir_y=%d, map char='%c'\n",
             current.x, current.y,
             current.prev_x, current.prev_y,
             dir_x, dir_y,
             map[current.y][current.x]);
    }
  }

  printf("Biggest loop found was %d \nhalf is %d\n", biggest_loop, biggest_loop / 2);

  free(memory_arena);
  fclose(fptr);
}