#include "memcached.h"
#include "items.h"
#include "slabs.h"


item ** items[MAX_NUMBER_OF_SLAB_CLASSES];
rel_time_t * times[MAX_NUMBER_OF_SLAB_CLASSES];
size_t count[MAX_NUMBER_OF_SLAB_CLASSES];
size_t budget[MAX_NUMBER_OF_SLAB_CLASSES];

void add_item( item * it, int id ) {
    if (!budget[id]) {
        budget[id] = 1024;
        items[id] = malloc(sizeof(item *)*budget[id]);
        times[id] = malloc(sizeof(rel_time_t)*budget[id]);
    } else if (count == budget) {
        budget[id] *= 2;
        items[id] = realloc(items[id], sizeof(item *)*budget[id]);
        times[id] = realloc(times[id], sizeof(rel_time_t)*budget[id]);
    }
    items[count[id]] = it;
    times[count[id]] = it->time;
    count[id]++;
}

void merge_sort(int i, int j, int id, rel_time_t * aux1, item ** aux2) {
    if (j <= i) {
        return;     // the subsection is empty or a single element
    }
    int mid = (i + j) / 2;
    
    merge_sort(i, mid, id, aux1, aux2);     // sort the left sub-array recursively
    merge_sort(mid + 1, j, id, aux1, aux2);     // sort the right sub-array recursively

    int pointer_left = i;       // pointer_left points to the beginning of the left sub-array
    int pointer_right = mid + 1;        // pointer_right points to the beginning of the right sub-array
    int k;      // k is the loop counter

    // we loop from i to j to fill each element of the final merged array
    for (k = i; k <= j; k++) {
        if (pointer_left == mid + 1) {      // left pointer has reached the limit
            aux1[k] = times[id][pointer_right];
            aux2[k] = items[id][pointer_right];
            pointer_right++;
        } else if (pointer_right == j + 1) {        // right pointer has reached the limit
            aux1[k] = times[id][pointer_left];
            aux2[k] = items[id][pointer_left];
            pointer_left++;
        } else if (times[id][pointer_left] < times[id][pointer_right]) {        // pointer left points to smaller element
            aux1[k] = times[id][pointer_left];
            aux2[k] = items[id][pointer_left];
            pointer_left++;
        } else {        // pointer right points to smaller element
            aux1[k] = times[id][pointer_right];
            aux2[k] = items[id][pointer_right];
            pointer_right++;
        }
    }

    for (k = i; k <= j; k++) {      // copy the elements from aux[] to a[]
        times[id][k] = aux1[k];
        items[id][k] = aux2[k];
    }
}



void repair_lru(const unsigned int slabs_clsid) {
    void * ptr;
    item * it;
    for (int id=0; id<MAX_NUMBER_OF_SLAB_CLASSES; id++) {
        for (int i=0; i<slabclass[id].slabs; i++) {
            ptr = slabclass[id].slab_list[i];
            for (int j=0; j<slabclass[id].perslab; j++) {
                item * it = (item*) ptr;
                if ((it->it_flags & ITEM_SLABBED) == 0) {
                    add_item(it, id);
                    unlink_item_q(it);
                }
            }
        }
    }

    int max_count = count[0];
    for (int i=0; i<MAX_NUMBER_OF_SLAB_CLASSES; i++) {
        if (count[i] > max_count) max_count = count[i];
    }
    rel_time_t * aux1 = malloc(sizeof(rel_time_t)*max_count);
    item ** aux2 = malloc(sizeof(item *)*max_count);
    for (int id=0; id<MAX_NUMBER_OF_SLAB_CLASSES; id++) {
        merge_sort(0, count[id], id, aux1 ,aux2);
        for (int j=0; j<count[id]; j++) {
            item_link_q(items[j]);
        }
    }
}