#ifndef CSV_PARSER_H
#define CSV_PARSER_H

#ifdef DESKTOP_BUILD

#include "filter.h"

int csv_process_stream(const char* in_path,
                       const char* out_path,
                       FilterState* filter);

#endif

#endif
