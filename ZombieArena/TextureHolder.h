#ifndef TEXTUREHOLDER_H
#define TEXTUREHOLDER_H

#include "stdafx.h"
#include <SFML\Graphics.hpp>
#include <map>

using namespace sf;

class TextureHolder{
private:
	//map container, holds string and texture
	std::map<std::string, Texture> _textures;
	
	static TextureHolder* _instance;
public:
	TextureHolder();
	static Texture& getTexture(std::string const&);
};

#endif