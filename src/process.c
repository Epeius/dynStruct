#include "dr_api.h"
#include "../includes/allocs.h"
#include "../includes/block_utils.h"

// todo get the symbol of the function who has accessed to the memory
void	print_orig(orig_t *orig, const char *type)
{
  while (orig)
    {
      dr_printf("\t\t\t %d bytes was %s by %p %d times\n", orig->size, type,
		orig->addr, orig->nb_hit);
      orig  = orig->next;
    }
}

void	print_access(malloc_t *block)
{
  access_t	*access_read = block->read;
  access_t	*access_write = block->write;

  while (access_read)
    {
      dr_printf("\t was read at offset %d (%lu times)\n", access_read->offset,
		access_read->total_hits);
      dr_printf("\tdetails :\n");
      print_orig(access_read->origs, "read");
      access_read = access_read->next;
    }

  while (access_write)
    {
      dr_printf("\t was write at offset %d (%lu times)\n", access_write->offset,
		access_write->total_hits);
      dr_printf("\tdetails :\n");
      print_orig(access_write->origs, "write");
      access_write = access_write->next;
    }
}

void	process_recover(void)
{
  malloc_t      *block = blocks;
  malloc_t      *tmp;

  while (block)
    {
      tmp = block->next;
      dr_printf("block : %p-%p(0x%x) ", block->start, block->end, block->size);
      if (block->flag & FREE)
	dr_printf("was free\n");
      else
        dr_printf("was not free\n");
      print_access(block);
      free_malloc_block(block);
      block = tmp;
    }
  blocks = NULL;
}