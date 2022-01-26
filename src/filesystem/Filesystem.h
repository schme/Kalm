#pragma once

#include <stddef.h>

namespace ks {

/** Use log_x macros to do their own outputs */

/* contentSize expects null terminator
 * return -1 is error */
int writeToFile(const char* content, size_t contentSize, const char* path);

/* return bytes read, -1 on error */
int readFromFile(const char* path, char* buf, size_t n);

}
