#pragma once

namespace ks {

template <typename T>
struct StaticSingleton {
	static T& get() {
		static T* instance = new T;;
		return *instance;
	}
};

}
