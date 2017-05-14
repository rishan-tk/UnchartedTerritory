#pragma once

#include "Entity.h"

class Coin : public Entity
{
public:
	Coin(glm::vec2 position);
	~Coin();

	void draw(GameEngine2D::SpriteBatch& spriteBatch);

	bool update(const glm::vec2& position);
};

