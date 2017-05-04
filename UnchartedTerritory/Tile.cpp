#include "Tile.h"

Tile::Tile(glm::vec2 position, TileType tile) : _isColliable(false){

	_position = position;//+ glm::vec2(ENTITY_DIMENSION/2, ENTITY_DIMENSION/2);
	_tile = tile;

}


Tile::~Tile()
{
}



void Tile::draw(GameEngine2D::SpriteBatch & spriteBatch){
	glm::vec4 posAndSize(_position, ENTITY_DIMENSION, ENTITY_DIMENSION);

	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);

	GLuint textureID;

	switch (_tile) {
		case EARTH:
			textureID = GameEngine2D::ResourceManager::getTexture("Textures/Tiles/Earth.png").id;
			break;
		case ROAD:
			textureID = GameEngine2D::ResourceManager::getTexture("Textures/Tiles/Road.png").id;
			_isColliable = true;
			break;
		case GRASS:
			textureID = GameEngine2D::ResourceManager::getTexture("Textures/Tiles/Grass.png").id;
			_isColliable = true;
			break;
	}

	GameEngine2D::Colour colour;
	colour.r = 255;
	colour.g = 255;
	colour.b = 255;
	colour.a = 255;

	spriteBatch.draw(posAndSize, uv, textureID, 0.0f, colour);
}
