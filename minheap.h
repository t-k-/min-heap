#include <stdint.h>

#define HEAP_VAL uint32_t

#ifndef BOOL
#define BOOL uint32_t
#endif

typedef HEAP_VAL (*heap_val_fun)(void*);

struct heap {
	void       **array;
	uint32_t     size;
	uint32_t     end;
	heap_val_fun val;
};

struct heap heap_create(uint32_t, heap_val_fun);
void        heap_destory(struct heap*);
BOOL        heap_full(struct heap*);
uint32_t    heap_size(struct heap*);
void        heap_push(struct heap*, void*);
HEAP_VAL    heap_top_val(struct heap*);
void        heap_print_tr(struct heap*);
void        heap_print_arr(struct heap*);
void        minheap_heapify(struct heap*);
void        minheap_insert(struct heap*, void*);
void        minheap_delete(struct heap*, uint32_t);
void        minheap_sort(struct heap*);
void        heap_sort_desc(struct heap*);
void        heap_test(void);
