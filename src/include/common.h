#pragma once

#include <cstdint>
#include <string>

#include "log.h"

namespace ks {

enum LogLevel : int {
	Fatal = 0,
	Error,
	Warning,
	Info,
	Debug,
	Trace,

	Highest,
};

#define ks_unused(x) (void(x))

#define ks_likely(x) ([[likely]] x)
#define ks_unlikely(x) ([[unlikely]] x)

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using f32 = float;
using f64 = double;

}

static inline void removePath(std::string& filename)
{
	size_t pos = filename.find_last_of("/");
	if (pos == std::string::npos)
		return;

	filename = filename.substr(pos + 1);
}

/** returns extension */
static inline std::string removeExtension(std::string &filename)
{
	int dotPos = filename.find_last_of(".");
	std::string ext = filename.substr(dotPos, filename.size());
	filename = filename.substr(0, dotPos);
	return ext;
}

static inline std::string getExtension(const std::string &filename)
{
	return filename.substr(filename.find_last_of("."), filename.size());
}
