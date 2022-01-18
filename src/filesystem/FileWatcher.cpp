#include "FileWatcher.h"

#include "include/log.h"

#ifdef __linux__

/**
 * Based on inotify, most referenced from:
 * https://www.linuxjournal.com/article/8478
 */

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

		{
			std::scoped_lock lock(watchesMutex);

			for (const Watch& watch : watches) {
				inotify_rm_watch(desc, watch.desc);
			}
			watches.clear();
		}

		close(desc);
		desc = -1;

		observerThread.join();
	}

	void addWatcher(const char* pathname, void (*callback)(FileWatcher::EventParams))
	{
		log_debug("Adding watcher for: %s\n", pathname);

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

		constexpr size_t eventSize = sizeof(inotify_event);
		constexpr size_t bufLen = 1024 * (eventSize + 16);

		char buf[bufLen];
		int len, i=0;

		while((len = read(desc, buf, bufLen)) > 0) {
			while (i < len) {
				inotify_event *event = nullptr;
				event = (inotify_event *) &buf[i];

				FileWatcher::EventParams params = {};

				params.eventMask |= (event->mask & IN_CREATE) > 0 ? FileWatcher::Event::Created : 0;
				params.eventMask |= (event->mask & IN_MODIFY) > 0 ? FileWatcher::Event::Modified : 0;
				params.eventMask |= (event->mask & IN_DELETE) > 0 ? FileWatcher::Event::Deleted : 0;

				if (event->len > 0) {
					params.filename = std::string(event->name);
				}

				void (*callback) (FileWatcher::EventParams) = nullptr;

				{
					std::scoped_lock lock(watchesMutex);
					for (const Watch& watch : watches) {
						if (watch.desc == event->wd) {
							callback = watch.callback;
							break;
						}
					}
				}
				if (callback)
					callback(params);

				i += eventSize + event->len;
			}
			i = 0;
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

	int desc = 0;
	std::thread observerThread;
	std::mutex watchesMutex;
	std::vector<Watch> watches;
};



#elif _WIN32
namespace ks {
	static_assert(false, "not implemented");
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
