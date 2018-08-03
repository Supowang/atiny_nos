#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mem_manager.h"
#ifdef NOS_MEM_MANAGER_EN
#define LOG(fmt, arg...) printf(fmt"\r\n", ##arg)

#define BLOCK_COUNT (BLOCK0_COUNT + BLOCK1_COUNT + BLOCK2_COUNT + BLOCK3_COUNT + BLOCK4_COUNT + BLOCK5_COUNT + BLOCK6_COUNT)
#define BLOCK_SIZE (BLOCK0_COUNT * BLOCK0_SIZE + BLOCK1_COUNT * BLOCK1_SIZE + BLOCK2_COUNT * BLOCK2_SIZE + BLOCK3_COUNT * BLOCK3_SIZE + BLOCK4_COUNT * BLOCK4_SIZE + BLOCK5_COUNT * BLOCK5_SIZE + BLOCK6_COUNT * BLOCK6_SIZE)

#define BLOCK_CAT_MAX 7
#define MEM_CAT_BITLEN 8
static unsigned char mem_pool[BLOCK_SIZE] = {0};
static unsigned char mem_cat[(BLOCK_COUNT -1 ) / MEM_CAT_BITLEN + 1] = {0};  //1: free 0: used

typedef struct _block_stru {
  int size;
  int block_count;
}block_stru;

block_stru block_mem[BLOCK_CAT_MAX] = {
  {BLOCK0_SIZE, BLOCK0_COUNT},
  {BLOCK1_SIZE, BLOCK1_COUNT},
  {BLOCK2_SIZE, BLOCK2_COUNT},
  {BLOCK3_SIZE, BLOCK3_COUNT},
  {BLOCK4_SIZE, BLOCK4_COUNT},
  {BLOCK5_SIZE, BLOCK5_COUNT},
  {BLOCK6_SIZE, BLOCK6_COUNT}
};

int isfree(int idx)
{
  return (mem_cat[idx / MEM_CAT_BITLEN] & (1 << (idx % MEM_CAT_BITLEN))) == (1 << (idx % MEM_CAT_BITLEN));
}

int block_inuse(int idx)
{
  mem_cat[idx / MEM_CAT_BITLEN] &= ~(1 << (idx % MEM_CAT_BITLEN));
  return 0;
}

int block_unuse(int idx)
{
  mem_cat[idx / MEM_CAT_BITLEN] |= (1 << (idx % MEM_CAT_BITLEN));
  return 0;
}

void mem_manager_init()
{
  int i;
  for (i = 0; i < BLOCK_COUNT; i++){
    block_unuse(i);
  }
}
void * mem_malloc(int size)
{
  int i = 0, j = 0;
  unsigned char * p = mem_pool;
  if (size <= 0)
    return NULL;
  
  for (i = 0; i < BLOCK_CAT_MAX; i++){
    if (size <= block_mem[i].size){
      int count_end = j + block_mem[i].block_count;
      while(j < count_end){
	if (isfree(j)){
	  block_inuse(j);
	  unsigned char * pp = p;
	  return pp;
	}
	else {
	  p += block_mem[i].size;
	}
	j++;
      }
    }
    else {
      p += (block_mem[i].block_count * block_mem[i].size);
      j += block_mem[i].block_count;
    }
  }

  return NULL;
}

void mem_free(void * p)
{
  int i, j;
  int block_idx = 0;
  unsigned char * pp = mem_pool;
  for (i = 0; i < BLOCK_CAT_MAX; i++){
    for (j = 0; j < block_mem[i].block_count; j++){
      if (p == pp){
	block_unuse(block_idx);
	return;
      }
      else {
	block_idx ++;
	pp += block_mem[i].size;
      }
    }
  }
}

#endif /* NOS_MEM_MANAGER_EN */
