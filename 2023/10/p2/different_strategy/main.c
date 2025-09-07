
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdbool.h>
#include "hash.h"

#include "../queue.h"

#define DIR_NORTH (1)
#define DIR_EAST (1 << 1)
#define DIR_SOUTH (1 << 2)
#define DIR_WEST (1 << 3)

// 0001 <-> 0100
// 0010 <-> 1000

// its a circular shift by 2
#define OPPOSITE_DIR(dir) (((dir & 3) << 2) | ((dir & 12) >> 2))

#define VERTICAL_PIPE (DIR_NORTH | DIR_SOUTH) // 0b0101
#define HORIZONTAL_PIPE (DIR_EAST | DIR_WEST) // ob1o1o
#define NORTH_EAST_PIPE (DIR_NORTH | DIR_EAST)
#define NORTH_WEST_PIPE (DIR_NORTH | DIR_WEST)
#define SOUTH_WEST_PIPE (DIR_SOUTH | DIR_WEST)
#define SOUTH_EAST_PIPE (DIR_SOUTH | DIR_EAST)
#define GROUND_NO_PIPE 0
#define STARTING_POSITION 0 // encode as something?

static uint8_t dir[4] = {DIR_NORTH, DIR_EAST, DIR_SOUTH, DIR_WEST};

uint8_t char_to_pipe(int c)
{
  switch (c)
  {
  case '|':
    return VERTICAL_PIPE;
  case '-':
    return HORIZONTAL_PIPE;
  case 'F':
    return SOUTH_EAST_PIPE; // top-left corner
  case '7':
    return SOUTH_WEST_PIPE; // top-right corner
  case 'L':
    return NORTH_EAST_PIPE; // bottom-left corner
  case 'J':
    return NORTH_WEST_PIPE; // bottom-right corner
  case 'S':
    return DIR_NORTH | DIR_EAST | DIR_SOUTH | DIR_WEST; // cross
  case '.':
    return GROUND_NO_PIPE;
  default:
    return 0; // unknown
  }
}

char pipe_to_char(uint8_t pipe)
{
  switch (pipe)
  {
  case VERTICAL_PIPE:
    return '|';
  case HORIZONTAL_PIPE:
    return '-';
  case SOUTH_EAST_PIPE: // top-left corner
    return 'F';
  case SOUTH_WEST_PIPE: // top-right corner
    return '7';
  case NORTH_EAST_PIPE: // bottom-left corner
    return 'L';
  case NORTH_WEST_PIPE: // bottom-right corner
    return 'J';
  case (DIR_NORTH | DIR_EAST | DIR_SOUTH | DIR_WEST): // cross
    return 'S';
  case GROUND_NO_PIPE:
    return '.';
  default:
    return '?'; // unknown
  }
}

bool is_vertical_line_tile(uint8_t tile)
{
  return (tile & (DIR_NORTH | DIR_SOUTH)) && !(tile & DIR_WEST);
}

bool is_horizontal_line_tile(uint8_t tile)
{
  return (tile & (DIR_EAST | DIR_WEST)) && !(tile & DIR_SOUTH);
}

static const int dx[16] = {
    [DIR_NORTH] = 0,
    [DIR_EAST] = 1,
    [DIR_SOUTH] = 0,
    [DIR_WEST] = -1,
};
static const int dy[16] = {
    [DIR_NORTH] = -1,
    [DIR_EAST] = 0,
    [DIR_SOUTH] = 1,
    [DIR_WEST] = 0,
};

typedef struct MemoryArena
{
  void *data;
  size_t capacity;
} MemoryArena;

typedef struct VisitedNode
{
  size_t cost;
  struct VisitedNode *previous;
} VisitedNode;

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

  uint8_t *memory_arena = malloc(
      column_count * row_count * sizeof(uint8_t)       // map elements
      + row_count * sizeof(uint8_t *)                  // map row pointers
      + column_count * row_count * sizeof(VisitedNode) // visited elements
      + row_count * sizeof(VisitedNode *)              // visited row pointers
      + 9 * column_count * row_count * sizeof(uint8_t) // plus-size map elements
      + 3 * row_count * sizeof(uint8_t *)              // plus-size map row pointers.
  );

  if (!memory_arena)
  {
    printf("Failed to allocate memory arena.\n");
    exit(1);
  }

  uint8_t *ptr = memory_arena;

  // map elements
  uint8_t *map_elements = ptr;
  ptr += column_count * row_count * sizeof(uint8_t);

  // map row pointers
  uint8_t **map = (uint8_t **)ptr;
  ptr += row_count * sizeof(uint8_t *);

  // assign row pointers for map
  for (uint32_t i = 0; i < row_count; i++)
  {
    map[i] = &map_elements[i * column_count];
  }

  // visited elements
  VisitedNode *visited_elements = (VisitedNode *)ptr;
  ptr += column_count * row_count * sizeof(VisitedNode);

  // visited row pointers
  VisitedNode **visited = (VisitedNode **)ptr;
  ptr += row_count * sizeof(VisitedNode *);

  // assign row pointers for visited
  for (uint32_t i = 0; i < row_count; i++)
  {
    visited[i] = &visited_elements[i * column_count];
  }

  // plus-size map elements
  uint8_t *plus_size_map_elements = ptr;
  ptr += 9 * column_count * row_count * sizeof(uint8_t);

  for (uint32_t i = 0; i < 9 * column_count * row_count; i++)
  {
    plus_size_map_elements[i] = 0; // zero out all memory
  }

  // plus-size map row pointers
  uint8_t **plus_size_map = (uint8_t **)ptr;
  ptr += 3 * row_count * sizeof(uint8_t *);

  // assign row pointers for map
  for (uint32_t i = 0; i < 3 * row_count; i++)
  {
    plus_size_map[i] = &plus_size_map_elements[3 * i * column_count];
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
    map[row][column] = char_to_pipe(ch);

    column++;
  }

  fclose(fptr);

  // lets just check for all directions around it for a pipe connection and follow it.

  DynamicList path;
  initialize_list(&path, 30);

  int pos_x = start_x;
  int pos_y = start_y;
  uint8_t incoming_direction = 0;
  uint8_t start_dirs = 0;

  while (1)
  {
    bool found_dir = false;

    /* Append current position */
    ListNode node = {pos_x, pos_y};
    append(&path, &node);

    /* Check all directions */
    for (int i = 0; i < 4; i++)
    {
      uint8_t d = dir[i];
      if ((map[pos_y][pos_x] & d) && !(incoming_direction & d))
      {
        int nx = pos_x + dx[d];
        int ny = pos_y + dy[d];

        /* Check bounds */
        if (nx >= 0 && nx < (int)column_count && ny >= 0 && ny < (int)row_count &&
            (map[ny][nx] & OPPOSITE_DIR(d)))
        {

          if (path.current == 1)
          { // first step from start
            start_dirs |= d;
          }

          pos_x = nx;
          pos_y = ny;
          incoming_direction = OPPOSITE_DIR(d);
          found_dir = true;
          break;
        }
      }
    }

    if (!found_dir)
    {
      break; // dead end
    }

    if (pos_x == start_x && pos_y == start_y && path.current > 1)
    {
      start_dirs |= incoming_direction;
      break; // looped
    }
  }

  printf("Starting block is %c\n", pipe_to_char(start_dirs));

  map[start_y][start_x] = start_dirs;

  // Run through the path and for each map the char at the position
  // and add them to

  /**
   * so
   * | maps to
   *          .|.
              .|.
              .|.

    7 maps to ...
              -7.
              .|.

    F maps to ...
              .F-
              .|.

    J maps to .|.
              -J.
              ...


    L maps to .|.
              .L-
              ...

    - maps to ...
              ---
              ...

    . maps to ...
              ...
              ...
   */
  for (size_t i = 0; i < path.current; i++)
  {

    int x = path.data[i].x;
    int y = path.data[i].y;
    if (x >= 0 && x < (int)column_count && y >= 0 && y < (int)row_count)
    {
      // this should be true for all.

      switch (map[y][x])
      {
      case (DIR_NORTH | DIR_EAST | DIR_SOUTH | DIR_WEST):
        // S
        break;
      case (VERTICAL_PIPE):
        // plus_size_map[3*y][3*x] should be the top left corner.
        plus_size_map[3 * y][3 * x] = GROUND_NO_PIPE;
        plus_size_map[3 * y][3 * x + 1] = VERTICAL_PIPE;
        plus_size_map[3 * y][3 * x + 2] = GROUND_NO_PIPE;

        plus_size_map[3 * y + 1][3 * x] = GROUND_NO_PIPE;
        plus_size_map[3 * y + 1][3 * x + 1] = VERTICAL_PIPE;
        plus_size_map[3 * y + 1][3 * x + 2] = GROUND_NO_PIPE;

        plus_size_map[3 * y + 2][3 * x] = GROUND_NO_PIPE;
        plus_size_map[3 * y + 2][3 * x + 1] = VERTICAL_PIPE;
        plus_size_map[3 * y + 2][3 * x + 2] = GROUND_NO_PIPE;

        break;
      case (HORIZONTAL_PIPE):
        plus_size_map[3 * y][3 * x] = GROUND_NO_PIPE;
        plus_size_map[3 * y][3 * x + 1] = GROUND_NO_PIPE;
        plus_size_map[3 * y][3 * x + 2] = GROUND_NO_PIPE;

        plus_size_map[3 * y + 1][3 * x] = HORIZONTAL_PIPE;
        plus_size_map[3 * y + 1][3 * x + 1] = HORIZONTAL_PIPE;
        plus_size_map[3 * y + 1][3 * x + 2] = HORIZONTAL_PIPE;

        plus_size_map[3 * y + 2][3 * x] = GROUND_NO_PIPE;
        plus_size_map[3 * y + 2][3 * x + 1] = GROUND_NO_PIPE;
        plus_size_map[3 * y + 2][3 * x + 2] = GROUND_NO_PIPE;

        break;
      case (NORTH_EAST_PIPE):
        plus_size_map[3 * y][3 * x] = GROUND_NO_PIPE;
        plus_size_map[3 * y][3 * x + 1] = VERTICAL_PIPE;
        plus_size_map[3 * y][3 * x + 2] = GROUND_NO_PIPE;

        plus_size_map[3 * y + 1][3 * x] = GROUND_NO_PIPE;
        plus_size_map[3 * y + 1][3 * x + 1] = NORTH_EAST_PIPE; // center pipe
        plus_size_map[3 * y + 1][3 * x + 2] = HORIZONTAL_PIPE;

        plus_size_map[3 * y + 2][3 * x] = GROUND_NO_PIPE;
        plus_size_map[3 * y + 2][3 * x + 1] = GROUND_NO_PIPE;
        plus_size_map[3 * y + 2][3 * x + 2] = GROUND_NO_PIPE;
        break;
      case (NORTH_WEST_PIPE):
        plus_size_map[3 * y][3 * x] = GROUND_NO_PIPE;
        plus_size_map[3 * y][3 * x + 1] = VERTICAL_PIPE; // North pipe
        plus_size_map[3 * y][3 * x + 2] = GROUND_NO_PIPE;

        plus_size_map[3 * y + 1][3 * x] = HORIZONTAL_PIPE;     // West pipe
        plus_size_map[3 * y + 1][3 * x + 1] = NORTH_WEST_PIPE; // center pipe
        plus_size_map[3 * y + 1][3 * x + 2] = GROUND_NO_PIPE;

        plus_size_map[3 * y + 2][3 * x] = GROUND_NO_PIPE;
        plus_size_map[3 * y + 2][3 * x + 1] = GROUND_NO_PIPE;
        plus_size_map[3 * y + 2][3 * x + 2] = GROUND_NO_PIPE;
        break;
      case (SOUTH_EAST_PIPE):
        plus_size_map[3 * y][3 * x] = GROUND_NO_PIPE;
        plus_size_map[3 * y][3 * x + 1] = GROUND_NO_PIPE;
        plus_size_map[3 * y][3 * x + 2] = GROUND_NO_PIPE;

        plus_size_map[3 * y + 1][3 * x] = GROUND_NO_PIPE;
        plus_size_map[3 * y + 1][3 * x + 1] = SOUTH_EAST_PIPE; // center pipe
        plus_size_map[3 * y + 1][3 * x + 2] = HORIZONTAL_PIPE; // East pipe

        plus_size_map[3 * y + 2][3 * x] = GROUND_NO_PIPE;
        plus_size_map[3 * y + 2][3 * x + 1] = VERTICAL_PIPE; // South pipe
        plus_size_map[3 * y + 2][3 * x + 2] = GROUND_NO_PIPE;
        break;
      case (SOUTH_WEST_PIPE):
        plus_size_map[3 * y][3 * x] = GROUND_NO_PIPE;
        plus_size_map[3 * y][3 * x + 1] = GROUND_NO_PIPE;
        plus_size_map[3 * y][3 * x + 2] = GROUND_NO_PIPE;

        plus_size_map[3 * y + 1][3 * x] = HORIZONTAL_PIPE;     // West pipe
        plus_size_map[3 * y + 1][3 * x + 1] = SOUTH_WEST_PIPE; // center pipe
        plus_size_map[3 * y + 1][3 * x + 2] = GROUND_NO_PIPE;

        plus_size_map[3 * y + 2][3 * x] = GROUND_NO_PIPE;
        plus_size_map[3 * y + 2][3 * x + 1] = VERTICAL_PIPE; // South pipe
        plus_size_map[3 * y + 2][3 * x + 2] = GROUND_NO_PIPE;
        break;
      case (GROUND_NO_PIPE):
        plus_size_map[3 * y][3 * x] = GROUND_NO_PIPE;
        plus_size_map[3 * y][3 * x + 1] = GROUND_NO_PIPE;
        plus_size_map[3 * y][3 * x + 2] = GROUND_NO_PIPE;

        plus_size_map[3 * y + 1][3 * x] = GROUND_NO_PIPE;     // West pipe
        plus_size_map[3 * y + 1][3 * x + 1] = GROUND_NO_PIPE; // center pipe
        plus_size_map[3 * y + 1][3 * x + 2] = GROUND_NO_PIPE;

        plus_size_map[3 * y + 2][3 * x] = GROUND_NO_PIPE;
        plus_size_map[3 * y + 2][3 * x + 1] = GROUND_NO_PIPE; // South pipe
        plus_size_map[3 * y + 2][3 * x + 2] = GROUND_NO_PIPE;
        break;
      }
    }
  }

  // for (uint32_t i = 0; i < 3 * row_count; i++) {
  //   for (uint32_t j = 0; j < 3 * column_count; j++) {
  //     printf("%c", pipe_to_char(plus_size_map[i][j]));
  //   }
  //   printf("\n");
  // }

  // okay now we can do a flood fill.
  // lets just pick the top left corner.

  // for this we want a queue

  Queue queue;

  initialize_queue(&queue, 9 * row_count * column_count); // max is having all nodes at the same time.

  queue.data = malloc(queue.capacity * sizeof(QueueNode));

  if (!queue.data)
  {
    free_list(&path);

    free(memory_arena);
    return 1;
  }

  queue_push(&queue, &(QueueNode){.x = 0, .y = 0, .steps = 0});
  uint64_t number_of_empty_outside = 0;

  // HashSet seen;
  // initialize_hashset(&seen, 16);
  int32_t width = 3 * (int32_t)column_count;
  size_t row_bytes = ((size_t)width + 7) / 8;                                 // number of bytes per row
  uint8_t *seen = calloc(row_bytes * 3 * (size_t)row_count, sizeof(uint8_t)); // 1 bit per cell

  size_t n_bit_index = (size_t)start_y * row_bytes + (size_t)(start_x / 8);
  uint8_t n_mask = 1U << (start_x % 8);
  
  seen[n_bit_index] |= n_mask; // mark as seen immediately

  while (!queue_empty(&queue))
  {

    QueueNode node;
    queue_pop(&queue, &node);
    // printf("{Pop} {%" PRId32", %" PRId32 "}\n", node.x, node.y);

    number_of_empty_outside++;
    plus_size_map[node.y][node.x] = 32;

    for (int i = 0; i < 4; i++)
    {
      uint8_t d = dir[i];

      int32_t nx = (int32_t)(node.x + dx[d]);
      int32_t ny = (int32_t)(node.y + dy[d]);

      if (nx >= 0 && nx < (int32_t)(3 * column_count) && ny >= 0 && ny < (int32_t)(3 * row_count))
      {
        // its within the map.

        size_t n_bit_index = (size_t)ny * row_bytes + (size_t)(nx / 8);
        uint8_t n_mask = 1U << (nx % 8);

        // check if not seen before.
        if (plus_size_map[ny][nx] == GROUND_NO_PIPE && !(seen[n_bit_index] & n_mask))
        {
          seen[n_bit_index] |= n_mask; // mark as seen immediately
          queue_push(&queue, &(QueueNode){.x = nx, .y = ny});
        }
      }
    }
  }

  for (uint32_t i = 0; i < 3 * row_count; i++)
  {
    for (uint32_t j = 0; j < 3 * column_count; j++)
    {
      printf("%c", pipe_to_char(plus_size_map[i][j]));
    }
    printf("\n");
  }

  // printf("Path.current = %.1f\n", (double)path.current);
  // double sumt = (double)(((double)number_of_empty_outside - 3 * (double)path.current));
  // printf("before division: %.6f\n", sumt );
  // sumt/= 9.0;

  // printf("sum = %.6f\n", sumt);  // prints with 6 decimals

  // frist i though i could do some math to figure it out but i think i need to loop over each 9x9 in the big one and see which of them is
  // all outside.
  uint64_t empty_in_map = 0;
  for (uint32_t y = 0; y < row_count; y++)
  {
    for (uint32_t x = 0; x < column_count; x++)
    {
      if (plus_size_map[3 * y][3 * x] == GROUND_NO_PIPE &&
          plus_size_map[3 * y][3 * x + 1] == GROUND_NO_PIPE &&
          plus_size_map[3 * y][3 * x + 2] == GROUND_NO_PIPE &&

          plus_size_map[3 * y + 1][3 * x] == GROUND_NO_PIPE &&
          plus_size_map[3 * y + 1][3 * x + 1] == GROUND_NO_PIPE &&
          plus_size_map[3 * y + 1][3 * x + 2] == GROUND_NO_PIPE &&

          plus_size_map[3 * y + 2][3 * x] == GROUND_NO_PIPE &&
          plus_size_map[3 * y + 2][3 * x + 1] == GROUND_NO_PIPE &&
          plus_size_map[3 * y + 2][3 * x + 2] == GROUND_NO_PIPE)
      {
        empty_in_map++;
      }
    }
  }

  // uint64_t inside = row_count * column_count - empty_outside_in_map - (uint64_t)path.current;
  printf("How many inside? %" PRIu64 "", empty_in_map);

  free_list(&path);

  free(memory_arena);

  return 0;
}