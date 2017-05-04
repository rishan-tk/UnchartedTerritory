#pragma once

#include <GameEngine2D/InputManager.h>

#include "Tile.h"
#include "DynamicEntity.h"

class Player : public DynamicEntity
{
public:
	Player(glm::vec2 position);
	~Player();

	void initialise(float speed, GameEngine2D::InputManager* inputManager);

	void draw(GameEngine2D::SpriteBatch& spriteBatch);

	void update(std::vector<Tile> tiles);

private:
	GameEngine2D::InputManager* _inputManager;
};

