#pragma once

#include "include/common.h"

// getting rid of this dependancy would be cool
#include <string>

namespace ks {

/**
 * Will call callback from a separate observer thread.
 * Linux version uses inotify, so things it can't do won't be done. Things it can do
 * aren't probably done.
 *
 * Pathname can be a file or a folder.
 */
struct FileWatcher {

	enum Event : u32 {
		Created = 1u << 0,
		Modified = 1u << 1,
		Deleted = 1u << 2,
	};

	struct EventParams {
		u32 eventMask = 0;
		std::string filename;
	};

	static void init();
	static void destroy();
	static void addWatcher(const char* pathname, void (*callback)(FileWatcher::EventParams));
	static void removeWatchers(const char* pathname);

};

}
