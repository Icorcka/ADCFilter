#ifdef DESKTOP_BUILD

#include <stdio.h>
#include <stdlib.h>
#include "filter.h"
#include "csv_parser.h"

int main(int argc, char* argv[]) {
    if (argc != 4) {
        fprintf(stderr,
                "Usage: %s <input_file> <output_file> <alpha>\n"
                "Example: %s data/signal.csv data/output_signal.csv 0.12\n",
                argv[0], argv[0]);
        return 1;
    }

    const char* in_path  = argv[1];
    const char* out_path = argv[2];

    char* end = NULL;
    float alpha = strtof(argv[3], &end);
    if (end == argv[3] || *end != '\0') {
        fprintf(stderr, "Error: alpha must be a number, got '%s'\n", argv[3]);
        return 1;
    }
    if (alpha < 0.0f || alpha > 1.0f) {
        fprintf(stderr, "Error: alpha must be in [0, 1], got %f\n", (double)alpha);
        return 1;
    }

    FilterState* filter = filter_create(alpha);
    if (filter == NULL) {
        fprintf(stderr, "Error: failed to create filter\n");
        return 1;
    }

    int rc = csv_process_stream(in_path, out_path, filter);
    filter_destroy(filter);

    if (rc != 0) {
        fprintf(stderr, "Error: failed to process '%s' -> '%s'\n", in_path, out_path);
        return 1;
    }

    printf("Done. Wrote %s (alpha=%.4f)\n", out_path, (double)alpha);
    return 0;
}

#else

#include <stddef.h>
#include "filter.h"

void filter_array_in_place(float* buffer, size_t length, float alpha) {
    if (buffer == NULL || length == 0) {
        return;
    }
    FilterState* filter = filter_create(alpha);
    if (filter == NULL) {
        return;
    }
    for (size_t i = 0; i < length; ++i) {
        buffer[i] = filter_process_sample(filter, buffer[i]);
    }
    filter_destroy(filter);
}

#endif
