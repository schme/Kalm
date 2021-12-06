#include "SceneBank.h"

#include "ResourceBank.h"

namespace ks {

Scene* SceneBank::load(const std::string &filepath, bool absolutePath)
{
	Scene scene;
	scene.name = filepath;

	auto& storage = ResourceStorage<Scene>::get();
	return storage.add(filepath, std::move(scene));
}

}
