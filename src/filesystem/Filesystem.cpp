#include "Filesystem.h"

#include "include/log.h"

#ifdef __linux__

#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

namespace ks {

static void writeToFile_Linux(const char* content, size_t cSize, const char* path)
{
	int fd;
	fd = open(path, O_WRONLY | O_CREAT);

	int res = write(fd, content, cSize);
	if (res < 0) {
		log_error("Could write to file: %s ", path);
		perror(strerror(errno));
	}
}

#else

namespace ks {

#endif


/** API FUNCTIONS */

void writeToFile(const char* content, size_t contentSize, const char* path)
{
#ifdef __linux__
	writeToFile_Linux(content, contentSize, path);
#else
#error "Not implemented!"
#endif
}

}
