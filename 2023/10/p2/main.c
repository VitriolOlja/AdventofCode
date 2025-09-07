
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
// like a clock -> (dir < 1) & 15 to turn clockwise.
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

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdbool.h>
#include "queue.h"
#include "dynamic_list.h"

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

bool is_vertical_line_tile(uint8_t tile) {
    return (tile & (DIR_NORTH | DIR_SOUTH)) && !(tile & DIR_WEST);
}

bool is_horizontal_line_tile(uint8_t tile) {
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

// Function to count crossings in one direction
int count_line_crossings(size_t x, size_t y, int dx, int dy,
                         size_t column_count, size_t row_count,
                         uint8_t **map, bool (*is_line_tile)(uint8_t))
{
    int crossings = 0;
    int xx = (int)x + dx;
    int yy = (int)y + dy;

    while (xx >= 0 && xx < (int)column_count && yy >= 0 && yy < (int)row_count) {
        if (is_line_tile(map[yy][xx])) {
            crossings++;
        }
        xx += dx;
        yy += dy;
    }

    return crossings;
}

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
      + size * sizeof(QueueNode)                       // queue storage
      + column_count * row_count * sizeof(VisitedNode) // visited elements
      + row_count * sizeof(VisitedNode *)              // visited row pointers
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

  // visited elements
  VisitedNode *visited_elements = (VisitedNode *)ptr;
  ptr += column_count * row_count * sizeof(VisitedNode);

  // visited row pointers
  VisitedNode **visited = (VisitedNode **)ptr;
  ptr += row_count * sizeof(VisitedNode *);

  // assign row pointers for map
  for (uint32_t i = 0; i < row_count; i++)
  {
    map[i] = &map_elements[i * column_count];
  }

  // assign row pointers for visited
  for (uint32_t i = 0; i < row_count; i++)
  {
    visited[i] = &visited_elements[i * column_count];
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

  // lets just check for all directions around it for a pipe connection and follow it.

  DynamicList path;
  initialize_list(&path, 30);

  int pos_x = start_x;
  int pos_y = start_y;
  uint8_t incoming_direction = 0;

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
          pos_x = nx;
          pos_y = ny;
          incoming_direction = OPPOSITE_DIR(d);
          found_dir = true;
          break;
        }
      }
    }

    if (!found_dir)
      break; // dead end

    if (pos_x == start_x && pos_y == start_y && path.current > 1)
      break; // looped
  }

  uint8_t **loop_tiles = malloc(row_count * sizeof(uint8_t*));
  for (size_t i = 0; i < row_count; i++)
      loop_tiles[i] = malloc(column_count * sizeof(uint8_t));

  // 1 = loop, 2 = inside, 3 = outside, 0 = empty (not part of loop)
  for (size_t y = 0; y < row_count; y++)
    for (size_t x = 0; x < column_count; x++)
      loop_tiles[y][x] = 0;

  // Mark the loop tiles
  for (size_t i = 0; i < path.current; i++)
  {
    int x = path.data[i].x;
    int y = path.data[i].y;
    if (x >= 0 && x < (int)column_count && y >= 0 && y < (int)row_count)
      loop_tiles[y][x] = 1;
  }

  int inside_sum = 0;

  // Raycast each empty tile in 4 directions
  for (size_t y = 0; y < row_count; y++)
  {
    for (size_t x = 0; x < column_count; x++)
    {
      if (loop_tiles[y][x] == 1) // skip loop tiles
        continue;


      int crossings;

      if (y == (size_t) start_y) {
        //go up
        crossings = count_line_crossings(x, y, 0, -1, column_count, row_count, map, is_horizontal_line_tile);
      }
      else {
        //go right
         crossings = count_line_crossings(x, y, 1, 0, column_count, row_count, map, is_vertical_line_tile);
      }
      

      if (crossings % 2 == 1 )
      {
        loop_tiles[y][x] = 2; // inside
        inside_sum++;
      }
      else
      {
        loop_tiles[y][x] = 3; // outside
      }
    }
  }

  // Print map
  for (size_t y = 0; y < row_count; y++)
  {
    for (size_t x = 0; x < column_count; x++)
    {
      if (loop_tiles[y][x] == 1)
        putchar('#'); // loop tile
      else if (loop_tiles[y][x] == 2)
        putchar('1'); // inside
      else
        putchar('0'); // outside
    }
    putchar('\n');
  }

  printf("Inside tiles = %d\n", inside_sum);

  free_list(&path);

  for (size_t i = 0; i < row_count; i++) {
    free(loop_tiles[i]);   // free each row
  }
  free(loop_tiles);          // free the array of pointers

  free(memory_arena);
  fclose(fptr);
}
