#include "filter.h"
#include <stddef.h>
#include <stdint.h>

#ifdef DESKTOP_BUILD
#include <stdlib.h>
#endif

struct FilterState {
    float alpha;
    float prev_value;
    uint8_t is_first;
};

#ifdef DESKTOP_BUILD

FilterState* filter_create(float alpha) {
    FilterState* state = (FilterState*)malloc(sizeof(FilterState));
    if (state == NULL) {
        return NULL;
    }
    state->alpha = alpha;
    state->prev_value = 0.0f;
    state->is_first = 1;
    return state;
}

void filter_destroy(FilterState* state) {
    free(state);
}

#else 
#define FILTER_POOL_SIZE (4)

static FilterState filter_pool[FILTER_POOL_SIZE];
static int filter_pool_used = 0;

FilterState* filter_create(float alpha) {
    if (filter_pool_used >= FILTER_POOL_SIZE) {
        return NULL;
    }
    FilterState* state = &filter_pool[filter_pool_used++];
    state->alpha      = alpha;
    state->prev_value = 0.0f;
    state->is_first   = 1;
    return state;
}

void filter_destroy(FilterState* state) {
    (void)state;
}

#endif

float filter_process_sample(FilterState* state, float in_sample) {
    if (state->is_first) {
        state->prev_value = in_sample;
        state->is_first   = 0;
        return in_sample;
    }
    state->prev_value = state->alpha * in_sample
                      + (1.0f - state->alpha) * state->prev_value;
    return state->prev_value;
}
