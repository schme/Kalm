#include "FileWatcher.h"

#include "include/log.h"

#ifdef __linux__

#include <cstdio>
#include <cstring>
#include <mutex>
#include <string>
#include <sys/inotify.h>
#include <thread>
#include <unistd.h>
#include <vector>

namespace ks {

struct Impl {

	struct Watch {
		std::string pathname;
		int desc = -1;
		void (*callback)(FileWatcher::EventParams);
	};

	void destroy() {

		std::scoped_lock lock(watchesMutex);
		observerThread.join();

		for (const Watch& watch : watches) {
			inotify_rm_watch(desc, watch.desc);
		}
		close(desc);
		watches.clear();
	}

	void addWatcher(const char* pathname, void (*callback)(FileWatcher::EventParams))
	{
		log_debug("%s: %s\n", "Adding watcher for", pathname);

		std::scoped_lock lock(watchesMutex);
		Watch watch;
		watch.pathname = pathname;
		watch.callback = callback;
		watch.desc = inotify_add_watch(desc, pathname, IN_MODIFY | IN_CREATE | IN_DELETE);

		if (watch.desc < 0) {
			perror("add watch");
		}

		watches.push_back(std::move(watch));
	}

	void removeWatchers(const char* pathname)
	{
		std::scoped_lock lock(watchesMutex);
		int found = -1;
		for (unsigned i=0; i < watches.size(); ++i) {
			Watch &watch = watches[i];
			if (std::strcmp(watch.pathname.c_str(), pathname) == 0) {
				found = i;
				break;
			}
		}

		if (found == -1)
			return;

		{
			Watch &watch = watches[found];
			inotify_rm_watch(desc, watch.desc);
		}
		watches.erase(watches.begin() + found);
	}

	void observe() {
		while(true) {
			log_info("%s\n", "tick");
			sleep(1000);
		}
	}

	void init() {
		desc = inotify_init();
		if (desc < 0) {
			perror("inotify_init");
			return;
		}

		observerThread = std::thread(&Impl::observe, this);
	}

	static Impl& get()
	{
		static Impl impl;
		return impl;
	}


	std::thread observerThread;
	std::mutex watchesMutex;

	int desc = 0;
	std::vector<Watch> watches;
};



#elif _WIN32

namespace ks {

	static_assert(false, "not implemented");
	// TODO: implement

#else

namespace ks {

	static_assert(false, "platform not supported");

#endif


void FileWatcher::init()
{
	Impl::get().init();
}

void FileWatcher::destroy()
{
	Impl::get().destroy();
}

void FileWatcher::addWatcher(const char* pathname, void (*callback)(FileWatcher::EventParams))
{
	Impl::get().addWatcher(pathname, callback);
}

void FileWatcher::removeWatchers(const char* pathname)
{
	Impl::get().removeWatchers(pathname);
}

}
