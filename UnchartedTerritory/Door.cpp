#include "Door.h"

Door::Door(){
	_aliveFor = 0;
	_currentState = ENTITY;
	_textureIndex = 1;
}

Door::~Door()
{
}

void Door::initialise(glm::vec2 position, int lifeTime){
	_position = position;
	_lifeTime = lifeTime; //Once lifeTime = 0 open the door
	
}

void Door::draw(GameEngine2D::SpriteBatch & spriteBatch){
	glm::vec4 posAndSize(_position, ENTITY_DIMENSION, ENTITY_DIMENSION);

	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);

	GameEngine2D::ColourRGBA8 colour(255, 255, 255, 255);

	_texture.initialise(GameEngine2D::ResourceManager::getTexture("Textures/SpriteSheet/Door.png"), glm::ivec2(6, 1));

	spriteBatch.draw(posAndSize, _texture.getUV(_textureIndex), _texture.texture.id, 0.0f, colour);
}

void Door::update(int noOfCoins, glm::vec2 playerPosition){
	if(_lifeTime != noOfCoins)
		_lifeTime = noOfCoins;

	//Start opening the door
	if (_lifeTime == 0) {
		//Animate door opening every 30 frames
		if (_aliveFor > 30 && _textureIndex < 5) {
			_textureIndex++;
			_aliveFor = 0;
		}

		if (_textureIndex == 5) {
			if (checkIfColliding(_position + glm::vec2(ENTITY_DIMENSION/2, 0.0f), playerPosition, ENTITY_DIMENSION))
				_atExit = true;
		}
		
		_aliveFor++;
	}
}

bool Door::isColliding(){

	return _atExit;
}
