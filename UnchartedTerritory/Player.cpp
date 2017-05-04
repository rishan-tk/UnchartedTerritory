#include "Player.h"

#include <SDL/SDL.h>

#include <iostream>


Player::Player(glm::vec2 position){
	_position = position + glm::vec2(ENTITY_DIMENSION / 2, ENTITY_DIMENSION / 2); ///< Set the players position to center coordinate
}


Player::~Player()
{
}

void Player::initialise(float speed, GameEngine2D::InputManager* inputManager){
	_speed = speed;
	_inputManager = inputManager;
}

void Player::draw(GameEngine2D::SpriteBatch& spriteBatch){
	glm::vec4 posAndSize(_position-glm::vec2(ENTITY_DIMENSION/2, ENTITY_DIMENSION/2),
											 ENTITY_DIMENSION, ENTITY_DIMENSION);

	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);

	GLuint textureID = GameEngine2D::ResourceManager::getTexture("Textures/SpriteSheet/Player.png").id;

	GameEngine2D::ColourRGBA8 colour(255, 255, 255, 255);

	spriteBatch.draw(posAndSize, uv, textureID, 0.0f, colour);
}

void Player::update(std::vector<Tile> tiles){
	if (_inputManager->isKeyPressed(SDLK_w))
		_position += glm::vec2(0.0f, _speed);
	if (_inputManager->isKeyPressed(SDLK_s))
		_position += glm::vec2(0.0f, -_speed);

	if (_inputManager->isKeyPressed(SDLK_a))
		_position += glm::vec2(-_speed, 0.0f);
	if (_inputManager->isKeyPressed(SDLK_d))
		_position += glm::vec2(_speed, 0.0f);

	std::cout << "X: " << _position.x << " Y: " << _position.y << std::endl;

	collideWithLevel(tiles);
}
