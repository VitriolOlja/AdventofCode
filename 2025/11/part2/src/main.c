#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdint.h>
#include <string.h>
#include "graph.h"

// I am thinking for part 2 that

// we got 2 ways of going from svr to out via dac and fft

// either we go
// svr -> fft -> dac -> out
// or
//  svr -> dac -> fft -> out
//  if we have 2 ways from svr -> dac and 2 from dac -> fft it should be multiplcation principle number of paths
//  and these should be exclusive I think. Maybe.
// in hindsight this is correct, but i dont think this is most efficient with the ways to out seen

////svr -> fft -> dac -> out
// uint32_t svr_fft = dfs(indices[svr], bitset, unique_count, indices[fft]);
// puts("first dfs\n");
// uint32_t fft_dac = dfs(indices[fft], bitset, unique_count, indices[dac]);
// puts("second dfs\n");
// uint32_t dac_out = dfs(indices[dac], bitset, unique_count, indices[out]);
// puts("third dfs\n");
//
// printf("swr_fft: %u \n", svr_fft);
// printf("fft_dac: %u \n", fft_dac);
// printf("dac_out: %u \n", dac_out);
//
//// svr -> dac -> fft -> out
// uint32_t svr_dac = dfs(indices[svr], bitset, unique_count, indices[dac]);
// uint32_t dac_fft = dfs(indices[dac], bitset, unique_count, indices[fft]);
// uint32_t fft_out = dfs(indices[fft], bitset, unique_count, indices[out]);
//
// printf("svr_dac: %u \n", svr_dac);
// printf("dac_fft: %u \n", dac_fft);
// printf("fft_out: %u \n", fft_out);
//

/**
 * current_index -> the index of the node i am on.
 * bitset -> if i have been here before.
 * ways_to_out here we save the number of ways we can get to out from this node.
 * count = the width and height of ways to out and the bitset.
 * out = the node we want to go to.
 *
 * dfs(indices[svr], graph, ways_to_out, seen, unique_count, indices[out]);
 */
uint64_t dfs(uint32_t current_index, Graph *graph, uint32_t *ways_to_out, uint64_t *seen, uint32_t out)
{
  //printf("current: %d   -  out: %d\n", current_index, out);
  if (current_index == out)
  {
    return 1;
  }
  if (ways_to_out[current_index] != UINT32_MAX)
  {
    return ways_to_out[current_index];
  }
  seen[current_index >> 6] |= 1ULL << (current_index & 63);

  uint64_t sum_of_paths = 0;

  for (GraphNode *n = graph->adjacency_list[current_index]; n; n = n->next)
  {
    sum_of_paths += dfs(n->vertex, graph, ways_to_out, seen, out);
  }

  ways_to_out[current_index] = sum_of_paths;
  return sum_of_paths;
}

void reset_stuff(uint32_t* ways_to_out, uint64_t *seen, uint32_t unique_count, size_t seensize) {
  for (size_t i = 0; i < unique_count; i++)
  {
    ways_to_out[i] = UINT32_MAX;
  } 

  memset(seen, 0, seensize);

}

int main()
{

  FILE *fptr = fopen("data/input.txt", "r");
  if (!fptr)
  {
    printf("could not find file input.txt\n");
    return 1;
  }

  int ch, last_ch;
  // 26^3 = 17576 like 70 kB memory for a direct access table of the chars. not terrible.
  // it might be acceptable. if we want a matrix though it might be a bit much.
  // so maybe we make a direct access table -> and inside it we put a index to each
  // not sure if it is fine but well try to get it on the stack. A uint16 or uint8 might have worked here aswell.
  // not really that many entries.
  uint32_t indices[17576];
  for (size_t i = 0; i < 17576; i++)
  {
    indices[i] = UINT32_MAX;
  } // UINT32_MAX is sentinel value

  // uint32_t out = ('o' - 'a') * 26 * 26 + ('u' - 'a') * 26 + ('t' - 'a');
  // printf("out: %u\n", out);
  // gives 10003

  // uint32_t you = ('y' - 'a') * 26 * 26 + ('o' - 'a') * 26 + ('u' - 'a');
  // printf("you: %u\n", you); //gives 16608
  // const uint32_t you = 16608;

  // uint32_t svr = ('s' - 'a') * 26 * 26 + ('v' - 'a') * 26 + ('r' - 'a');
  // printf("svr: %u\n", svr); //gives 12731

  // uint32_t fft = ('f' - 'a') * 26 * 26 + ('f' - 'a') * 26 + ('t' - 'a');
  // printf("fft: %u\n", fft); //gives 3529

  // uint32_t dac = ('d' - 'a') * 26 * 26 + ('a' - 'a') * 26 + ('c' - 'a');
  // printf("dac: %u\n", dac); //gives 2030

  // printf("reading size\n");
  uint32_t current_index = 0;
  uint32_t unique_count = 0;
  while ((ch = fgetc(fptr)) != EOF)
  {
    if ('a' <= ch && ch <= 'z')
    {
      //printf("%c", ch);
      current_index = current_index * 26 + (ch - 'a');
    }
    else if (ch == ':')
    {
      if (indices[current_index] == UINT32_MAX) {
        //printf(": %d (0)\n", unique_count);
        indices[current_index] = unique_count;
        unique_count++;
      }
      else {
        //printf("\n");
      }
      current_index = 0;
      fgetc(fptr);
    }
    else if (ch == ' ')
    {
      if (indices[current_index] == UINT32_MAX)
      {
        // i havent seen this before so we need to save this also.
        //printf(": %d (1) \n", unique_count);
        indices[current_index] = unique_count;
        unique_count++;
      }
      else {
        //printf("\n");
      }
      current_index = 0;
    }
    else if (ch == '\n')
    {
      if (indices[current_index] == UINT32_MAX)
      {
        //printf(": %d (2) \n", unique_count);
        indices[current_index] = unique_count;
        unique_count++;
      }
      else {
        //printf("\n");
      }
      current_index = 0;
    }
  }


  Graph *graph = createGraph(unique_count);
  // printf("unique: %d\n", unique_count);
  fseek(fptr, 0, SEEK_SET);

  current_index = 0;
  size_t current_parent_index = 0;
  while ((ch = fgetc(fptr)) != EOF)
  {
    if ('a' <= ch && ch <= 'z')
    {
      //printf("%c", ch);
      current_index = current_index * 26 + (ch - 'a');
    }
    else if (ch == ':')
    {
      current_parent_index = indices[current_index];
      current_index = 0;
      fgetc(fptr);
    }
    else if (ch == ' ')
    {
      size_t mapped_child = indices[current_index];
      addEdge(graph, current_parent_index, mapped_child);
      current_index = 0;
    }
    else if (ch == '\n')
    {
      size_t mapped_child = indices[current_index];
      addEdge(graph, current_parent_index, mapped_child);
      current_index = 0;
    }
  }

  //for (size_t i = 0; i < graph->number_of_vertices; i++) {
  //  printf("%zu: ", i);
  //  for (GraphNode *n = graph->adjacency_list[i]; n; n = n->next)
  //      printf("%zu ", n->vertex);
  //  printf("\n");
  //}
  // we need a seen access table, but with how many counts in each there are.
  uint32_t *ways_to_out = malloc(sizeof(uint32_t) * unique_count);

  for (size_t i = 0; i < unique_count; i++)
  {
    ways_to_out[i] = UINT32_MAX;
  } 
  size_t required_memory = (unique_count + 63) >> 6; // /64

  uint64_t *seen = calloc(required_memory, sizeof(uint64_t));

  const uint32_t out = 10003;
  const uint32_t dac = 2030;
  const uint32_t fft = 3529;
  const uint32_t svr = 12731;
  // printf("svr: %d, out: %d\n", indices[svr], indices[out]);
  uint64_t svr_fft = dfs(indices[svr], graph, ways_to_out, seen, indices[fft]);
  reset_stuff(ways_to_out, seen, unique_count, required_memory);
  uint64_t fft_dac = dfs(indices[fft], graph, ways_to_out, seen, indices[dac]);
  reset_stuff(ways_to_out, seen, unique_count, required_memory);
  uint64_t dac_out = dfs(indices[dac], graph, ways_to_out, seen, indices[out]);
  reset_stuff(ways_to_out, seen, unique_count, required_memory);
  
  uint64_t svr_dac = dfs(indices[svr], graph, ways_to_out, seen, indices[dac]);
  reset_stuff(ways_to_out, seen, unique_count, required_memory);
  uint64_t dac_fft = dfs(indices[dac], graph, ways_to_out, seen, indices[fft]);
  reset_stuff(ways_to_out, seen, unique_count, required_memory);
  uint64_t fft_out = dfs(indices[fft], graph, ways_to_out, seen, indices[out]);
  uint64_t num_paths = svr_fft * fft_dac * dac_out + svr_dac * dac_fft * fft_out;

  printf("number of paths  %llu", num_paths);

  free(ways_to_out);
  free(seen);
  free_graph(graph);
  fclose(fptr);
  return 0;
}
