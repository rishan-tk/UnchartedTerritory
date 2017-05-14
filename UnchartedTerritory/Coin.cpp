#include "Coin.h"



Coin::Coin(glm::vec2 position){
	_position = position;
	_currentState = ENTITY;
	_lifeTime = 1;
	_aliveFor = 0;
	_textureIndex = 0;
}


Coin::~Coin()
{
}

void Coin::draw(GameEngine2D::SpriteBatch& spriteBatch){
	glm::vec4 posAndSize(_position, ENTITY_DIMENSION, ENTITY_DIMENSION);

	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);

	GameEngine2D::ColourRGBA8 colour(255, 255, 255, 255);

	_texture.initialise(GameEngine2D::ResourceManager::getTexture("Textures/SpriteSheet/coin.png"), glm::ivec2(1, 4));

	spriteBatch.draw(posAndSize, _texture.getUV(_textureIndex), _texture.texture.id, 0.0f, colour);
}

//Check if coin is colliding with player 
bool Coin::update(const glm::vec2& position){
	if (checkIfColliding(_position, position, ENTITY_DIMENSION)) {
		_lifeTime--;
		return true;
	}
	else {
		_aliveFor++;
		if (_aliveFor > 10) { ///< Rotate coin every 10 frames
			_aliveFor = 0;
			if (_textureIndex == 0)
				_textureIndex = 1;
			else if (_textureIndex == 1)
				_textureIndex = 2;
			else if (_textureIndex == 2)
				_textureIndex = 3;
			else
				_textureIndex = 0;
		}

		return false;
	}
}
