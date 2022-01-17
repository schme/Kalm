#pragma once

namespace ks {

/**
 * Will call callback from a separate observer thread.
 * Linux version uses inotify, limitations and capabilities apply
 *
 * Pathname can be a file or a folder.
 */
struct FileWatcher {

	enum Event : int {
		Created,
		Modified,
		Deleted
	};

	struct EventParams {
		FileWatcher::Event event;
	};

	static void init();
	static void destroy();
	static void addWatcher(const char* pathname, void (*callback)(FileWatcher::EventParams));
	static void removeWatchers(const char* pathname);

};

}
