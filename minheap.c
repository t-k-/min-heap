#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define HEAP_VAL uint32_t
#define BOOL uint32_t

typedef HEAP_VAL (*heap_val_fun)(void*);

struct minheap {
	void       **heap;
	uint32_t     size;
	uint32_t     end;
	heap_val_fun val;
};

struct minheap heap_create(uint32_t sz, heap_val_fun fun)
{
	struct minheap h;
	h.heap = calloc(sz, sizeof(void*));
	h.size = sz;
	h.end = 0;
	h.val = fun;

	return h;
}

void heap_destory(struct minheap *h)
{
	free(h->heap);
}

BOOL heap_full(struct minheap *h)
{
	return (h->end == h->size);
}

void heap_push(struct minheap *h, void *ptr)
{
	if (!heap_full(h))
		h->heap[h->end ++] = ptr;	
}

static __inline uint32_t left(uint32_t i)
{
	return (i << 1) + 1;
}

static __inline uint32_t right(uint32_t i)
{
	return (i << 1) + 2;
}

void _heap_print_tr(struct minheap *h, uint32_t i, uint32_t depth)
{
	uint32_t j;
	for (j = 0; j < depth; j++)
		printf("  ");
	printf("%u:%u\n", i, h->val(h->heap[i]));

	j = right(i);
	if (j < h->end)
		_heap_print_tr(h, j, depth + 1);

	j = left(i);
	if (j < h->end)
		_heap_print_tr(h, j, depth + 1);
}

void heap_print_tr(struct minheap *h)
{
	_heap_print_tr(h, 0, 0);
}

void heap_print_non_leaves(struct minheap *h)
{
	uint32_t i = (h->end >> 1) - 1;
	while (1) {
		printf("%u ", h->val(h->heap[i]));
		if (i == 0)
			break;
		else
			i --;
	}
	printf("\n");
}

void heap_trace_all(struct minheap *h)
{
	uint32_t i;
	for (i = 0; i < h->size; i++) {
		if (i == h->end)
			printf("| ");

		if (NULL == h->heap[i])
			printf("nil ");
		else
			printf("%u ", h->val(h->heap[i]));
	}
	printf("\n");
}

static __inline 
uint32_t swap(struct minheap *h, uint32_t i, uint32_t j)
{
	void *tmp = h->heap[i];
	h->heap[i] = h->heap[j];
	h->heap[j] = tmp;
}

void heapify(struct minheap *h, uint32_t i)
{
	HEAP_VAL son;
	uint32_t j, min = i;

	j = right(i);
	if (j < h->end) {
		son = h->val(h->heap[j]);
		if (h->val(h->heap[i]) > son)
			min = j;
	}
	
	j = left(i); 
	if (j < h->end) {
		son = h->val(h->heap[j]);
		if (h->val(h->heap[min]) > son)
			min = j;
	}

	if (min != i) {
		swap(h, min, i);
		heapify(h, min);
	}
}

void heap_build(struct minheap *h)
{
	uint32_t i = (h->end >> 1) - 1;

	while (1) {
		heapify(h, i);

		if (i == 0)
			break;
		else
			i --;
	}
}

static __inline 
void shift_down(struct minheap *h, uint32_t i)
{
	HEAP_VAL son;
	uint32_t j, min = i;

	while (1) {
		j = right(i);
		if (j < h->end) {
			son = h->val(h->heap[j]);
			if (h->val(h->heap[i]) > son)
				min = j;
		}

		j = left(i);
		if (j < h->end) {
			son = h->val(h->heap[j]);
			if (h->val(h->heap[min]) > son)
				min = j;
		}

		if (min != i) {
			swap(h, min, i);
			i = min;
		} else {
			break;
		}
	}
}

void heap_dele(struct minheap *h, uint32_t i)
{
	if (i < h->end) {
		h->end --;
		swap(h, i, h->end);
		shift_down(h, i);
	}
}

void heap_sort(struct minheap *h)
{
	heap_build(h);

	while (h->end)
		heap_dele(h, 0);
}

void heap_insert(struct minheap *h, void *ptr)
{
	uint32_t j, i = h->end;
	h->heap[i] = ptr;
	h->end ++;
	
	while (i) {
		j = (i - 1) >> 1;
		if (h->val(h->heap[j]) > h->val(h->heap[i]))
			swap(h, i, j);
		i = j;
	}
}

/* test */
HEAP_VAL my_heap_value(void *ptr)
{
	return *((uint32_t*)ptr);
}

int main()
{
	uint32_t i, data[] = {14, 2, 22, 13, 23, 10, 90, 36, 108, 12,
	                      9, 91, 1, 51, 11, 3, 15, 80, 3, 78, 53,
	                      5, 12, 21, 65, 70, 4};
	const uint32_t data_len = sizeof(data)/sizeof(uint32_t);
	struct minheap heap = heap_create(32, &my_heap_value);

	for (i = 0; i < data_len; i++)
		heap_push(&heap, &data[i]);

	heap_sort(&heap);
	heap_trace_all(&heap);
	
	for (i = 0; i < data_len; i++)
		heap_insert(&heap, &data[i]);

	heap_print_tr(&heap);
	heap_destory(&heap);
	return 0;
}
