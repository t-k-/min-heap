#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define HEAP_VAL uint32_t
#define BOOL uint32_t

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

#define COL_BEG "\033[1m\033[34m"
#define COL_END "\033[0m"

struct heap heap_create(uint32_t sz, heap_val_fun fun)
{
	struct heap h;
	h.array = calloc(sz, sizeof(void*));
	h.size = sz;
	h.end = 0;
	h.val = fun;

	return h;
}

void heap_destory(struct heap *h)
{
	free(h->array);
}

BOOL heap_full(struct heap *h)
{
	return (h->end == h->size);
}

uint32_t heap_size(struct heap *h)
{
	return h->end;
}

void heap_push(struct heap *h, void *ptr)
{
	h->array[h->end ++] = ptr;	
}

HEAP_VAL heap_top_val(struct heap *h)
{
	return h->val(h->array[0]);
}

static __inline uint32_t right_son(uint32_t i)
{
	return (i << 1) + 2;
}

static 
void _heap_print_tr(struct heap *h, uint32_t i, uint32_t depth)
{
	uint32_t j;
	for (j = 0; j < depth; j++)
		printf("  ");
	printf("[%u]:%u\n", i, h->val(h->array[i]));

	j = right_son(i);
	if (j < h->end)
		_heap_print_tr(h, j, depth + 1);

	j = j - 1; /* left son */
	if (j < h->end)
		_heap_print_tr(h, j, depth + 1);
}

void heap_print_tr(struct heap *h)
{
	if (h->end)
		_heap_print_tr(h, 0, 0);
}

void heap_print_arr(struct heap *h)
{
	uint32_t i;
	for (i = 0; i < h->size; i++) {
		if (i == h->end)
			printf("| ");

		if (NULL == h->array[i])
			printf("nil ");
		else
			printf("%u ", h->val(h->array[i]));
	}
	printf("\n");
}

static __inline 
uint32_t swap(struct heap *h, uint32_t i, uint32_t j)
{
	void *tmp = h->array[i];
	h->array[i] = h->array[j];
	h->array[j] = tmp;
}

static __inline
uint32_t min_son(struct heap *h, uint32_t i)
{
	HEAP_VAL j_val;
	uint32_t j, min = i; 
	
	j = right_son(i);
	if (j < h->end) {
		j_val = h->val(h->array[j]);
		if (h->val(h->array[min]) > j_val)
			min = j;
	}
	
	j = j - 1; /* left son */
	if (j < h->end) {
		j_val = h->val(h->array[j]);
		if (h->val(h->array[min]) > j_val)
			min = j;
	}

	return min;
}

static __inline 
void min_shift_down(struct heap *h, uint32_t i)
{
	uint32_t min;

	while (1) {
		min = min_son(h, i);

		if (min != i) {
			swap(h, min, i);
			i = min;
		} else {
			break;
		}
	}
}

void minheap_heapify(struct heap *h)
{
	uint32_t i = (h->end >> 1) - 1;
	/* for each i that is non-leaf */

	while (1) {
		min_shift_down(h, i);

		if (i == 0)
			break;
		else
			i --;
	}
}

void minheap_delete(struct heap *h, uint32_t i)
{
	if (i < h->end) {
		h->end --;
		swap(h, i, h->end);
		min_shift_down(h, i);
	}
}

void minheap_sort(struct heap *h)
{
	while (h->end)
		minheap_delete(h, 0);
}

void heap_sort_desc(struct heap *h)
{
	minheap_heapify(h);
	minheap_sort(h);
}

void minheap_insert(struct heap *h, void *ptr)
{
	uint32_t j, i = h->end;
	h->array[i] = ptr;
	h->end ++;

	while (i) {
		/* shift up... */
		j = (i - 1) >> 1; /* father */
		if (h->val(h->array[j]) > h->val(h->array[i]))
			swap(h, i, j);
		i = j;
	}
}

/* test */
static HEAP_VAL heap_test_callbk(void *ptr)
{
	return *((uint32_t*)ptr);
}

void heap_test()
{
	uint32_t cnt, i, data[] = 
	                    {14, 2, 22, 13, 23, 10, 90, 36, 108, 12,
	                      9, 91, 1, 51, 11, 3, 15, 80, 3, 78, 53,
	                      5, 12, 21, 65, 70, 4};
	const uint32_t data_len = sizeof(data)/sizeof(uint32_t);
	struct heap heap = heap_create(32, &heap_test_callbk);

	printf(COL_BEG "push data...\n" COL_END);
	for (i = 0; i < data_len; i++)
		if (!heap_full(&heap))
			heap_push(&heap, &data[i]);

	printf("data len = %u, heap size = %u.\n", data_len, 
	       heap_size(&heap));

	printf(COL_BEG "heap tree:\n" COL_END);
	heap_print_tr(&heap);

	printf(COL_BEG "heap array:\n" COL_END);
	heap_print_arr(&heap);

	printf(COL_BEG "after heapify:\n" COL_END);
	minheap_heapify(&heap);
	heap_print_tr(&heap);

	cnt = 0;
	printf(COL_BEG "ranking emulation...:\n" COL_END);
	while (cnt < 100) {
		i = (i + 1) % data_len;
		if (!heap_full(&heap)) {
			printf("insert %d\n", data[i]);
			minheap_insert(&heap, &data[i]);
		} else {
			HEAP_VAL val = heap_top_val(&heap);
			if (val < data[i]) {
				printf("replace %d with %d\n", val, data[i]);
				minheap_delete(&heap, 0);
				minheap_insert(&heap, &data[i]);
			}
		}
		cnt ++;
	}

	printf(COL_BEG "a heavy heap tree now:\n" COL_END);
	heap_print_tr(&heap);

	minheap_sort(&heap);
	printf(COL_BEG "heap array after min-heap sort:\n" COL_END);
	heap_print_arr(&heap);

	printf(COL_BEG "a new heap...\n" COL_END);
	heap_destory(&heap);
	heap = heap_create(32, &heap_test_callbk);
	for (i = 0; i < data_len; i++)
		if (!heap_full(&heap))
			heap_push(&heap, &data[i]);
	
	printf(COL_BEG "heap array:\n" COL_END);
	heap_print_arr(&heap);

	heap_sort_desc(&heap);
	printf(COL_BEG "heap array after heap sort:\n" COL_END);
	heap_print_arr(&heap);

	printf(COL_BEG "heap destory...\n" COL_END);
	heap_print_tr(&heap);
	heap_destory(&heap);
}

int main()
{
	heap_test();
	return 0;
}
