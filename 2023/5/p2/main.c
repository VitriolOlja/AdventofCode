#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Initial thought was a hash table..

/**
 *  But reconsidering, we should definitely have a binary search tree.
 *  We make a node for each Range a struct.
 *  Whenever we receive a new range, we append add it to the binary tree.
 *
 * When we search the BST we look until we find a valid range - if there is no valid range
 * We know that we map the number to itself.
 */

/**
 * My first thought for part 2 was just to load the seeds over a range and add another for loop.
 * This does not work wonders though, as it becomes another loop and therefore seems to increase the time by quite a lot. 
 * 
 * Another solution idea is that we somehow save and cache the last range that we used - It is very likely that since the increase is
 * incremental - that the next number is still contained within the same range. 
 * So we only search whenever that is not a viable range anymore. 
 * 
 * I think we need to keep one of these caches for each map from source to destination. 
 * So we need to keep a list of BstNode* 
 * 
 * Instead of previous assignment as such. 
 * BstNode *currentNode = *map_fields[f];
 * 
 * We can Keep a BstNode *currentNodes[MAP_COUNT];
 * 
 * each iteration we check that 
 *    currentNodes[f] != NULL && 
 *    currentNumber >= currentNodes[f]->data.source_range_start && 
 *    currentNumber < currentNodes[f]->data.source_range_start + currentNodes[f]->range_length
 * 
 * if it is -> we simply use that mapping and continue, else we make a new search. 
 * 
 * This will take advantage of temporal locality (tendency of a computer program to access the same memory location repeatedly. )
 */

#define SEED_DATA_TYPE uint32_t
typedef struct Range
{
  SEED_DATA_TYPE destination_range_start;
  SEED_DATA_TYPE source_range_start;
  SEED_DATA_TYPE range_length;
} Range;

typedef struct BstNode
{
  Range data;
  struct BstNode *left_child, *right_child;
} BstNode;

#define RANGE_INITIAL_CAPACITY 100
#define NODE_INITIAL_CAPACITY 200
#define SEED_INITIAL_CAPACITY 25

typedef struct MemoryArena
{
  // Range* range_arena_start;
  // size_t range_arena_index;
  // size_t range_arena_capacity; //lets not make the Range a pointer in the bstnode but rather just data.

  BstNode *node_arena_start;
  size_t node_arena_index;
  size_t node_arena_capacity;

  SEED_DATA_TYPE *seed_arena_start;
  size_t seed_arena_index;
  size_t seed_arena_capacity;
} MemoryArena;

/**
 * Contains all root nodes of the binary search trees.
 */
#define MAP_COUNT 7
typedef struct Maps
{
  BstNode *seed_to_soil;
  BstNode *soil_to_fertilizer;
  BstNode *fertilizer_to_water;
  BstNode *water_to_light;
  BstNode *light_to_temperature;
  BstNode *temperature_to_humidity;
  BstNode *humidity_to_location;
} Maps;

/**
 * Read_Ranges(BstNode* root)
 * Function that reads ranges and appends them to a bst.
 *    (destination, source, length)
 *                50 98 2
 *                52 50 48
 */
void read_ranges(FILE *fptr, MemoryArena *memory_arena, BstNode *root)
{
  SEED_DATA_TYPE ranges[3];
  char ch;
  // printf("%c \n",fgetc(fptr));

  // read the first and make it root.
  fscanf(fptr, "%lu %lu %lu\n", &ranges[0], &ranges[1], &ranges[2]);
  root->data.destination_range_start = ranges[0];
  root->data.source_range_start = ranges[1];
  root->data.range_length = ranges[2];

  // read the value - > search the bst -> place it. while we have more rows.
  while (fscanf(fptr, "%lu %lu %lu\n", &ranges[0], &ranges[1], &ranges[2]) == 3)
  {
    // printf("%d %d %d \n", ranges[0], ranges[1], ranges[2]);

    SEED_DATA_TYPE destination = ranges[0];
    SEED_DATA_TYPE source = ranges[1];
    SEED_DATA_TYPE length = ranges[2];

    // we sort the bst on source range -> because we will want to search it based on each source.

    BstNode *currentNode = root; // set it to currentNode.
    while (currentNode)
    {
      if (source >= currentNode->data.source_range_start)
      {
        // we are right child

        // if there is not a right child.
        if (!currentNode->right_child)
        {
          if (memory_arena->node_arena_index >= memory_arena->node_arena_capacity)
          {
            printf("Out of memory in the arena. give it more memory");
            return;
          }
          // we point right child to a  valid node in the arena.
          currentNode->right_child = &memory_arena->node_arena_start[memory_arena->node_arena_index++];

          // and add the data values.
          currentNode->right_child->data.destination_range_start = destination;
          currentNode->right_child->data.source_range_start = source;
          currentNode->right_child->data.range_length = length;

          break;
        }
        currentNode = currentNode->right_child;
      }
      else
      {
        // we are left child

        if (!currentNode->left_child)
        {
          // if there is no leftchild we add one. first check that we have enough memory.
          if (memory_arena->node_arena_index >= memory_arena->node_arena_capacity)
          {
            printf("Out of memory in the arena. give it more memory");
            return;
          }

          currentNode->left_child = &memory_arena->node_arena_start[memory_arena->node_arena_index++];
          currentNode->left_child->data.destination_range_start = destination;
          currentNode->left_child->data.source_range_start = source;
          currentNode->left_child->data.range_length = length;
          break;
        }

        currentNode = currentNode->left_child;
      }
    }
  }
}

void read_all_maps(FILE *fptr, MemoryArena *memory_arena, Maps *maps)
{

  // 7 different maps
  BstNode **map_fields[] = {
      &maps->seed_to_soil,
      &maps->soil_to_fertilizer,
      &maps->fertilizer_to_water,
      &maps->water_to_light,
      &maps->light_to_temperature,
      &maps->temperature_to_humidity,
      &maps->humidity_to_location};
  char ch;

  for (int i = 0; i < MAP_COUNT; i++)
  {
    while ((ch = fgetc(fptr)) != EOF && ch != ':')
      ;          // discard up to ':'
    fgetc(fptr); // discard the \n

    *map_fields[i] = &memory_arena->node_arena_start[memory_arena->node_arena_index++];
    read_ranges(fptr, memory_arena, *map_fields[i]);
  }
}

#define FILE_NAME "input.txt"
int main(void)
{
  MemoryArena memory_arena;

  // we calloc it so that we can do nullchecks on the binary searhc tree.
  void *tmp = calloc(1, NODE_INITIAL_CAPACITY * sizeof(BstNode) + SEED_INITIAL_CAPACITY * sizeof(SEED_DATA_TYPE));

  if (!tmp)
  {
    printf("Failed to allocate memory arena for task. \n");
    return 1;
  }

  memory_arena.node_arena_capacity = NODE_INITIAL_CAPACITY;
  memory_arena.node_arena_index = 0;
  memory_arena.node_arena_start = tmp;
  memory_arena.seed_arena_capacity = SEED_INITIAL_CAPACITY;
  memory_arena.seed_arena_index = 0;
  memory_arena.seed_arena_start = (SEED_DATA_TYPE *)((char *)tmp + NODE_INITIAL_CAPACITY * sizeof(BstNode));

  // start reading the file.

  FILE *fptr = fopen(FILE_NAME, "r");

  if (!fptr)
  {
    printf("Failed to open file. \n");
    free(tmp);
    return 1;
  }

  char ch;
  while ((ch = fgetc(fptr)) != EOF && ch != ':')
    ; // discard up to ':'

  // Get the seeds
  SEED_DATA_TYPE num;
  while (fscanf(fptr, "%lu", &num) == 1)
  {
    // I can just use the memory arena as a dynamic list directly here because it is a simple data structure.
    // but maybe i shouldnt.
    memory_arena.seed_arena_start[memory_arena.seed_arena_index] = num;
    memory_arena.seed_arena_index++;
  }

  Maps maps;
  read_all_maps(fptr, &memory_arena, &maps);

  // now we should have what we need to find the location what we need.

  BstNode **map_fields[MAP_COUNT] = {
          &maps.seed_to_soil,
          &maps.soil_to_fertilizer,
          &maps.fertilizer_to_water,
          &maps.water_to_light,
          &maps.light_to_temperature,
          &maps.temperature_to_humidity,
          &maps.humidity_to_location};

  // lets find the lowest location number then
  SEED_DATA_TYPE location = UINT32_MAX;

  BstNode *lastUsedNodes[MAP_COUNT] = {NULL};

  // for every other seed
  for (int i = 0; i < memory_arena.seed_arena_index; i += 2)
  {
    // the following seed memory_arena.seed_arena_start[i + 1];
    // defines the range of numbers
    for (int j = 0; j < memory_arena.seed_arena_start[i + 1]; j++)
    {
      // so the seed bcomes memory_arena.seed_arena_start[i] + j;
      SEED_DATA_TYPE seed = memory_arena.seed_arena_start[i] + j;

      // current NUmber keeps track of the source for each
      // and each map converts it to its destination.
      // at the end currentNumber will equal the location.
      SEED_DATA_TYPE currentNumber = seed;
      for (int f = 0; f < MAP_COUNT; f++)
      {

        // check here if the range is currently viable
        BstNode *currentNode = lastUsedNodes[f];

        if (currentNode && currentNumber >= currentNode->data.source_range_start &&
            currentNumber < currentNode->data.source_range_start + currentNode->data.range_length) {
          currentNumber = currentNumber - currentNode->data.source_range_start + currentNode->data.destination_range_start;
          continue;
        }
        
        //cache miss - reset node to root and do another search
        currentNode = *map_fields[f];

        // now search the binary search tree.
        while (currentNode)
        {
          if (currentNumber >= currentNode->data.source_range_start)
          {
            // either it is a right child or it is within range.

            // check if within range
            if (currentNumber < currentNode->data.source_range_start + currentNode->data.range_length)
            {
              // found range.
              currentNumber = currentNumber - currentNode->data.source_range_start + currentNode->data.destination_range_start;
              lastUsedNodes[f] = currentNode;
              break;
            }
            // else
            // it is a potential right child if it exists.
            if (currentNode->right_child)
            {
              currentNode = currentNode->right_child;
              continue;
            }
            // else
            //  right child does not exist. the number maps to itself.
            lastUsedNodes[f] = currentNode;
            break;
          }

          // currentNumber < currentNode->data.source_range_start

          // not possible for it to be within range -> because lower than start
          // already indicates out of range.

          // check if we have a left child

          if (currentNode->left_child)
          {
            currentNode = currentNode->left_child;
            continue;
          }
          // else we have no left child
          // we map to ourselves.
          lastUsedNodes[f] = currentNode;
          break;
        }
      }

      if (currentNumber < location)
      {
        location = currentNumber;
      }
      //printf("Seed %lu: location: %lu \n", seed, currentNumber);
    }
  }

  printf("location %lu \n", location);

  fclose(fptr);
  free(tmp);
  return 0;
}
