#include "task.h"
#include <stdio.h>
#include <stdlib.h>
#include "heap-inl.h"
#include <assert.h>

#define container_of(ptr, type, member) \
  ((type *) ((char *) (ptr) - offsetof(type, member)))

typedef struct {
        void* heap_node[3];
        uint64_t start_id;
}test_node_t;


typedef struct {
    struct heap  queue_heap;
    uint64_t counter;
}test_heap_t;

static int node_less_than(const struct heap_node* ha, const struct heap_node* hb)
{
    assert(ha != NULL);
    assert(hb != NULL);

    const test_node_t* lhs = container_of(ha, test_node_t, heap_node);
    const test_node_t* rhs = container_of(hb, test_node_t, heap_node);

    if (lhs->start_id < rhs->start_id)
        return 1;
    if (rhs->start_id < lhs->start_id)
        return 0;

    return 0;
}

TEST_IMPL(ux_heap_test)
{
    test_heap_t heap;
    test_node_t node, node1;

    node.start_id = heap.counter++;
    node1.start_id = heap.counter++;

    heap_init(&heap.queue_heap);

    heap_insert((struct heap*)&heap.queue_heap,
        (struct heap_node*)&node.heap_node,
        node_less_than);

    heap_insert((struct heap*)&heap.queue_heap,
        (struct heap_node*)&node1.heap_node,
        node_less_than);

    heap_remove((struct heap*)&heap.queue_heap,
        (struct heap_node*)&node.heap_node,
        node_less_than);

    node.start_id = heap.counter++;
    heap_remove((struct heap*)&heap.queue_heap,
        (struct heap_node*)&node.heap_node,
        node_less_than);

    return 0;
}
