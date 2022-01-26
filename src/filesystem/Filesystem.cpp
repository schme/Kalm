#include "Filesystem.h"

#include "include/log.h"

#ifdef __linux__

#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define CHECK_IF_RETURN(x, logging)\
	{ if (x < 0) {\
		(logging);\
		perror(nullptr);\
		return -1;\
	}}

#define CHECK_IF_CLOSE_RETURN(x, logging)\
	{ if (x < 0) {\
		(logging);\
		perror(nullptr);\
		close(x);\
		return -1;\
	}}


namespace ks {

static int writeToFile_Linux(const char* content, size_t cSize, const char* path)
{
	int fd;

	// 0664
	constexpr int createMode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
	fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, createMode);
	CHECK_IF_RETURN(fd, (log_error("Couldn't open file %s:", path)));

	// write() does not want the null terminator
	int res = write(fd, content, cSize-1);
	CHECK_IF_CLOSE_RETURN(res, (log_error("Couldn't write to file %s:", path)));

	close(fd);
	return 0;
}


static int readFromFile_Linux(const char* path, char* buf, size_t n)
{
	int fd;

	fd = open(path, O_RDONLY);
	CHECK_IF_RETURN(fd,	log_error("Couldn't open file: %s.", path));

	// write() does not want the null terminator
	int res = read(fd, buf, n);
	CHECK_IF_CLOSE_RETURN(res, log_error("Couldn't write to file: %s.", path));

	close(fd);
	return 0;
}

#else

namespace ks {

#endif


/** API FUNCTIONS */

int writeToFile(const char* content, size_t contentSize, const char* path)
{
#ifdef __linux__
	return writeToFile_Linux(content, contentSize, path);
#else
#error "Not implemented!"
#endif
}

int readFromFile(const char* path, char* buf, size_t n)
{
#ifdef __linux__
	return readFromFile_Linux(path, buf, n);
#else
#error "Not implemented!"
#endif
}
}
