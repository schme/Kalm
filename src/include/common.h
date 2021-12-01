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

static inline void removePath(std::string &filename)
{
	size_t pos = filename.find_last_of("/");
	if (pos == std::string::npos)
		return;

	filename = filename.substr(pos + 1);
}

static inline void removeExtension(std::string &filename)
{
	filename = filename.substr(0, filename.find_last_of("."));
}

