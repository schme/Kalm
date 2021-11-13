#pragma once

namespace ks {

enum LogLevel : int {
	Fatal,
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
}
