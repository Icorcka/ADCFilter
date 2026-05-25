#ifndef FILTER_H
#define FILTER_H

typedef struct FilterState FilterState;

/*
    * This function uses floating-point math, so is not compatible with
    * most embedded platforms. This filter requires alpha to be in [0, 1], so int
    * will not work.
*/
FilterState* filter_create(float alpha);
void filter_destroy(FilterState* state);
float filter_process_sample(FilterState* state, float in_sample);

#endif
