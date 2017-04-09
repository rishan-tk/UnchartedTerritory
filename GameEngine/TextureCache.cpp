#include "TextureCache.h"
#include "ImageLoader.h"
#include <iostream>


TextureCache::TextureCache(){

}


TextureCache::~TextureCache(){

}

GLTexture TextureCache::getTexture(std::string filePath){

	//Search the map using binary search for the key with the same path
	auto textureIterator = _textureCache.find(filePath);

	//Check if texture was not in the map
	if (textureIterator == _textureCache.end()){
		//If no texture then load it in
		GLTexture tempTexture = ImageLoader::loadPNG(filePath);

		//Insert the texture into the map
		_textureCache.emplace(filePath, tempTexture); 

		std::cout << "Loaded texture into cache\n";

		//Return the texture
		return tempTexture;
	}

	std::cout << "Used cached texture\n";

	//Texture is contained in the map
	return textureIterator->second; //Iterator contains a pair, return the value
}
