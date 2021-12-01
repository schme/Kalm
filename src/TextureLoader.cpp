#include "TextureLoader.h"
#include "ResourceBank.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace ks {


Texture* TextureLoader::load(const std::string &filepath, bool absolutePath)
{
	auto& bank = ResourceBank<Texture>::get();
	Texture *res = bank.find({filepath});
	if (res)
		return res;

	std::string fullpath = absolutePath ? filepath : projectRoot + filepath;
	int x,y,n,ok;
	ok = stbi_info(fullpath.c_str(), &x, &y, &n);

	if (!ok) {
		log_error("Failed to load texture: %s\n", filepath.c_str());
		return nullptr;
	}

	stbi_set_flip_vertically_on_load(false);
	u8* data = stbi_load(fullpath.c_str(), &x, &y, &n, 0);

	Texture txtr;
	txtr.data = data;
	txtr.channels = n;
	txtr.width = x;
	txtr.height = y;

	std::string name = filepath;
	removePath(name);

	res = bank.add(name, std::move(txtr));
	return res;
}

Texture* find(const ResourceId &id)
{
	auto bank = ResourceBank<Texture>::get();
	Texture* res = bank.find(id);
	return res;
}

}
