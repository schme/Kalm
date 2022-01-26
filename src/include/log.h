#pragma once

#include <cstdio>

namespace ks {

//TODO: implement actual logging

#define log_trace(format, ...) (fprintf(stderr, format, __VA_ARGS__))
#define log_debug(format, ...) (fprintf(stderr, format, __VA_ARGS__))
#define log_info(format, ...) (fprintf(stderr, format, __VA_ARGS__))
#define log_warning(format, ...) (fprintf(stderr, format, __VA_ARGS__))
#define log_error(format, ...) (fprintf(stderr, format, __VA_ARGS__))
#define log_fatal(format, ...) (fprintf(stderr, format, __VA_ARGS__))

}
