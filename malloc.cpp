#include <string.h>
#include <sys/types.h>
#include <unistd.h>

struct block // Structure of the allocated memory block.
{
  size_t size;
  struct block *next;
  int free;
};

#define META_SIZE sizeof(struct block)

void *global_base = NULL;

struct block *find_free_block(struct block **last, size_t size) // Finds a free block of memory.
{
  struct block *current = (struct block *)global_base;
  while (current && !(current->free && current->size >= size))
  {
    *last = current;
    current = current->next;
  }
  return current;
}

struct block *request_space(struct block *last, size_t size) // Request space from the OS using sbrk().
{
  struct block *block;
  block = (struct block *)sbrk(0);
  void *request = sbrk(size + META_SIZE);
  if (request == (void *)-1)
  {
    return NULL; // sbrk failed.
  }

  if (last)
  { // NULL on first request.
    last->next = block;
  }
  block->size = size;
  block->next = NULL;
  block->free = 0;
  return block;
}

void *Mymalloc(size_t size) // Allocates memory.
{
  struct block *block; // Pointer to the block of memory.

  if (size <= 0)
  {
    return NULL;
  }

  if (!global_base) // First time allocation.
  { // First call.
    block = request_space(NULL, size);
    if (!block)
    {
      return NULL;
    }
    global_base = block;
  }
  else
  {
    struct block *last = (struct block *)global_base; // Pointer to the last block.
    block = find_free_block(&last, size);
    if (!block)
    { // Failed to find free block.
      block = request_space(last, size); // Request space from the OS.
      if (!block)
      {
        return NULL;
      }
    }
    else
    { // Found free block
      block->free = 0;
    }
  }

  return (block + 1); // Return a pointer to the user data.
}

void *Mycalloc(size_t nelem, size_t elsize) 
{
  size_t size = nelem * elsize;
  void *ptr = Mymalloc(size);
  memset(ptr, 0, size);
  return ptr;
}

struct block *get_block_ptr(void *ptr)
{
  return (struct block *)ptr - 1;
}

void Myfree(void *ptr) // Frees memory.
{
  if (!ptr)
  {
    return;
  }
  struct block *block_ptr = get_block_ptr(ptr);
  block_ptr->free = 1;
}