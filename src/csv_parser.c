#include "csv_parser.h"

#ifdef DESKTOP_BUILD

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filter.h"

#define CSV_LINE_MAX (256)

int csv_process_stream(const char* in_path,
                       const char* out_path,
                       FilterState* filter) {
    FILE* in_file = fopen(in_path, "r");
    if (in_file == NULL) {
        return -1;
    }

    FILE* out_file = fopen(out_path, "w");
    if (out_file == NULL) {
        fclose(in_file);
        return -1;
    }

    char line[CSV_LINE_MAX];

    if (fgets(line, sizeof(line), in_file) == NULL) {
        fclose(in_file);
        fclose(out_file);
        return -1;
    }

    fprintf(out_file, "t,raw,filtered\n");

    while (fgets(line, sizeof(line), in_file) != NULL) {
        line[strcspn(line, "\r\n")] = '\0';

        char* comma = strchr(line, ',');
        if (comma == NULL) {
            continue;
        }
        *comma = '\0';

        char* t_str   = line;
        char* raw_str = comma + 1;

        float raw_value = strtof(raw_str, NULL);
        float filtered  = filter_process_sample(filter, raw_value);

        char filtered_str[32];
        snprintf(filtered_str, sizeof(filtered_str), "%.6f", filtered);

        fprintf(out_file, "%s,%s,%s\n", t_str, raw_str, filtered_str);
    }

    fclose(in_file);
    fclose(out_file);
    return 0;
}

#endif
