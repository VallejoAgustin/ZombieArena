#include "stdafx.h"
#include "TextureHolder.h"
#include <assert.h>

TextureHolder* TextureHolder::_instance = nullptr;

TextureHolder::TextureHolder() {
	assert(_instance == nullptr);
	_instance = this;
}

Texture& TextureHolder::getTexture(std::string const& filename) {
	//get ref to _textures using _instance
	auto& m = _instance->_textures;

	//iterator to hold kvp (key value pair) and search map
	auto keyValuePair = m.find(filename);

	if (keyValuePair != m.end()) {
		return keyValuePair->second;
	}
	else {
		//filename not found
		auto& texture = m[filename];
		texture.loadFromFile(filename);

		return texture;
	}
}