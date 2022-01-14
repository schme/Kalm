#include "TextureBank.h"

#include "main.h"
#include "render/Rendering.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace ks {

Texture* TextureBank::load(const std::string& filepath, bool absolutePath)
{
	auto& storage = ResourceStorage<Texture>::get();
	Texture* res = storage.find({filepath});
	if (res)
		return res;

	std::string fullpath = absolutePath ? filepath : getEditorState().projectRoot + getEditorState().assetPrefix + filepath;
	int x,y,n,ok;
	ok = stbi_info(fullpath.c_str(), &x, &y, &n);

	if (!ok) {
		log_error("Failed to load texture: %s\n", filepath.c_str());
		return nullptr;
	}

	stbi_set_flip_vertically_on_load(true);
	u8* data = stbi_load(fullpath.c_str(), &x, &y, &n, 0);

	Texture txtr;
	txtr.data = data;
	txtr.channels = n;
	txtr.width = x;
	txtr.height = y;
	txtr.id = render::generateTexture();
	render::loadTexture(txtr.id, txtr.width, txtr.height, txtr.channels, txtr.data);

	std::string name = filepath;
	removePath(name);

	res = storage.add(name, std::move(txtr));
	return res;
}

}
