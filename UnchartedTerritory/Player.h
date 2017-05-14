#pragma once

#include <GameEngine2D/InputManager.h>

#include "Tile.h"
#include "DynamicEntity.h"

enum MovementState {
	STANDING,
	RUNNING,
	JUMPING,
	FALLING
};

class Player : public DynamicEntity
{
public:
	Player(glm::vec2 position);
	~Player();

	void initialise(float speed, float gravity, GameEngine2D::InputManager* inputManager);

	void draw(GameEngine2D::SpriteBatch& spriteBatch);

	void update(const std::vector<Tile>& tiles, float deltaTime);

private:
	void move(float deltaTime);

	float approach(float goalPosition, float currentPosition, float deltaTime);

	GameEngine2D::InputManager* _inputManager;
	float _jumpSpeed;
	float _groundHeight;
	bool _onGround;

	int _direction; // -1 = left, 1 = right

	MovementState _movementState;

};

