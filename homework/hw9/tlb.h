#include "config.h" /* see pagtable guidance on this file */
#include "mlpt.h"   /* see pagetable this file */

/** invalidate all cache lines in the TLB */
void tlb_clear();

/**
 * return 0 if this virtual address does not have a valid
 * mapping in the TLB. Otherwise, return its LRU status: 1
 * if it is the most-recently used, 2 if the next-to-most,
 * etc.
 */
int tlb_peek(size_t va);

/**
 * If this virtual address is in the TLB, return its
 * corresponding physical address. If not, use
 * `translate(va)` to find that address, store the result
 * in the TLB, and return it. In either case, make its
 * cache line the most-recently used in its set.
 *
 * As an exception, if translate(va) returns -1, do not
 * update the TLB: just return -1.
 */
size_t tlb_translate(size_t va);

typedef struct way {
    size_t tag;
    size_t pa;
    int id;
    struct way *next;

} way;

typedef struct {
    int id;
    way **ways; 
    way *rway;
} bset;

typedef struct {
    size_t tag;
    size_t id;
    size_t off;
} tva;

void init_tlb(bset ***cache);
void init_va(tva *v, size_t va);
bset* init_set(bset *s, int id);
int get_status(way *rway, size_t t);
way *get_before(way *rway, way *nway);
int find_slot(way **ways);
void update_recent(way *rway, way* before);
void print_lru(way *rway);
