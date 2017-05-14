#include "Player.h"

#include <SDL/SDL.h>

#include <iostream>


Player::Player(glm::vec2 position){
	_position = position; 
	_jumpSpeed = 0.0f;
	_direction = 1;
	_groundHeight = 64.0f;
	_gravity = 0.0f;
	_velocity = glm::vec2(0);
	_velocityGoal = glm::vec2(0);
	_speed = 0.0f;
	_lifeTime = 100; ///< Characters health points
	_textureIndex = 8;
	_aliveFor = 0;
	_currentState = ENTITY;
	_movementState = STANDING;
}


Player::~Player(){

}

void Player::initialise(float speed, float gravity, GameEngine2D::InputManager* inputManager){
	_speed = speed;
	_inputManager = inputManager;
	_jumpSpeed = 64*2;
	_gravity = -gravity;
}

void Player::draw(GameEngine2D::SpriteBatch& spriteBatch){
	glm::vec4 posAndSize(_position, ENTITY_DIMENSION, ENTITY_DIMENSION);

	GameEngine2D::ColourRGBA8 colour(255, 255, 255, 255);

	_texture.initialise(GameEngine2D::ResourceManager::getTexture("Textures/SpriteSheet/BearSpriteSheet.png"), glm::ivec2(4, 3));

	if (_movementState == RUNNING) {
		if ((_textureIndex == 8 || _textureIndex == 9) && _aliveFor > 10) {
			_aliveFor = 0;
			_textureIndex = 4;
		}
		else if ((_textureIndex > 3 && _textureIndex < 8) && _aliveFor > 10) { ///< Cycle through running animation
			_aliveFor = 0;
			_textureIndex++;
		}
	}
	else if (_movementState == STANDING) {
		//Switch between 2 textures every 30 frames
		if (_textureIndex != 9 && _textureIndex != 8) { //If state has just changed set the texture straight away else alternate over 8 and 9
			_textureIndex = 9;
		}
		else if (_textureIndex == 8 && _aliveFor > 30) {
			_aliveFor = 0;
			_textureIndex = 9;
		}
		else if (_textureIndex == 9 && _aliveFor > 30) {
			_aliveFor = 0;
			_textureIndex = 8;
		}
	}
	else if (_movementState == JUMPING) {
		_aliveFor = 0;
		_textureIndex = 1;
	}
	else if (_movementState == FALLING) {
		_aliveFor = 0;
		_textureIndex = 2;
	}

	_aliveFor++;

	//Flip the image if needed
	glm::vec4 uv = _texture.getUV(_textureIndex);
	if (_direction == -1) {
		//Grab the next texture
		uv.x += 1.0f / _texture.dimensions.x;
		//Reverse the way it is grabbed
		uv.z *= -1;
	}

	//Draw the sprite
	spriteBatch.draw(posAndSize, uv, _texture.texture.id, 0.0f, colour);
}

void Player::update(const std::vector<Tile>& tiles, float deltaTime) {
	//If collision detected don't update the position

	//Moving left
	if (_inputManager->isKeyDown(SDLK_a) && _movementState != RUNNING) {
		_velocityGoal.x = -_speed;
		if (!collideWithLevel(tiles, _position + _velocityGoal, _currentState) && _movementState == STANDING) {
			move(deltaTime);
			_movementState = RUNNING;
			_direction = -1;
		}
		else if (!collideWithLevel(tiles, _position + _velocityGoal, _currentState)) {
			move(deltaTime);
		}
		/*else {
			_velocityGoal.x = 0;
			_movementState = FALLING;
		}*/
		else if (!collideWithLevel(tiles, _position + _velocityGoal, _currentState) && _movementState == FALLING) {
			move(deltaTime);
			_movementState = FALLING;
		}
		else
			_movementState = FALLING;

	}
	else if (_inputManager->isKeyDown(SDLK_a) && _movementState == JUMPING) {
		
		
	
	}
	else if (_inputManager->isKeyReleased(SDLK_a) && _movementState == RUNNING) {
		_movementState = STANDING;
	}

	//Moving right
	if (_inputManager->isKeyDown(SDLK_d) && _movementState != RUNNING) {
		_velocityGoal.x = _speed, 0.0f;
		if (!collideWithLevel(tiles, _position + _velocityGoal, _currentState) && _movementState == STANDING) {
			move(deltaTime);
			_movementState = RUNNING;
			_direction = 1;
		}
		else if (!collideWithLevel(tiles, _position + _velocityGoal, _currentState) && _movementState == JUMPING) {
			move(deltaTime);
		}
		else if (!collideWithLevel(tiles, _position + _velocityGoal, _currentState) && _movementState == FALLING) {
			move(deltaTime);
			_movementState = FALLING;
		}
		else
			_movementState = FALLING;
		
	}
	else if (_inputManager->isKeyDown(SDLK_d) && _movementState == JUMPING) {



	}
	
	else if (_inputManager->isKeyReleased(SDLK_d) && _movementState == RUNNING) {
		_movementState = STANDING;
	}

	//Jumping
	static int numJumps = 0;

	if (_inputManager->isKeyPressed(SDLK_SPACE) && _movementState != JUMPING) {
		_velocityGoal.y = _jumpSpeed * deltaTime;
		if (!collideWithLevel(tiles, _position + _velocityGoal, _currentState)) {
			move(deltaTime);
			_movementState = JUMPING;
		}
		else 
			_movementState = STANDING;
		
	}else if (_inputManager->isKeyReleased(SDLK_SPACE)) {
		_movementState = FALLING;
		numJumps = 0;
	}


	switch (_movementState) {
		case STANDING: 
			//Transition players velocity back to a halt
			_velocityGoal = glm::vec2(0);
			move(deltaTime);
			//If player can fall then change state to falling else keep same state
			if (!collideWithLevel(tiles, _position + _velocity + glm::vec2(0.0f, _gravity), _currentState))
				_movementState = FALLING;

			break;

		case RUNNING:
			//Update the position
			_position += _velocity;
			move(deltaTime);

			//If player can fall then change state to falling
			if (!collideWithLevel(tiles, _position + _velocity + glm::vec2(0.0f, _gravity), _currentState))
				_movementState = FALLING;
			else if (!collideWithLevel(tiles, _position + _velocity, _currentState)) ///< Else check if the player can continue moving in the same direction
				move(deltaTime);
			else ///< Else set state to standing
				_movementState = STANDING;

			break;

		case JUMPING:
			//Update the position
			_position += _velocity;

			if (!collideWithLevel(tiles, _position + _velocity, _currentState)) {
				if (_velocityGoal.y > _velocity.y && numJumps < 2) { ///< If player is going towards jump location and player can still jump
					move(deltaTime);
				}
				else if (_velocityGoal.y == _velocity.y && numJumps < 2 && _inputManager->isKeyPressed(SDLK_SPACE)) { ///< Double jump
					_velocity = glm::vec2(0); //If we want to double jump set velocity to 0 and keep jumping until velocity reaches the goal
					numJumps++;
				}
				else {
					_movementState = FALLING;
				}
			}
			else ///< Else if we are colliding set state to falling
				_movementState = FALLING;
			
			break;

		case FALLING:
			//Check if we are above ground level
			if (_position.y > _groundHeight) {
				_velocityGoal.y = _gravity;
				move(deltaTime);

				//Check if we can fall
				if (!collideWithLevel(tiles, _position + _velocity, _currentState)) {
					_position += _velocity;
					move(deltaTime);
				}
				else {
					_movementState = STANDING;
				}
			}
			else
				_movementState = STANDING;

			break;
	}

}

void Player::move(float deltaTime){
	if (_velocityGoal.x != _velocity.x)
		_velocity.x = approach(_velocityGoal.x, _velocity.x, deltaTime * 15); ///< deltaTime * 20 to improve responsiveness
	else
		_velocity.x = _velocityGoal.x;

	if (_velocityGoal.y != _velocity.y)
		_velocity.y = approach(_velocityGoal.y, _velocity.y, deltaTime * 15);
	else
		_velocity.y = _velocityGoal.y;
}

float Player::approach(float goalPosition, float currentPosition, float deltaTime)
{
	float difference = goalPosition - currentPosition;

	if (difference > deltaTime)
		return currentPosition + deltaTime;
	else if (difference < -deltaTime)
		return currentPosition - deltaTime;
	else
		return goalPosition;
}
