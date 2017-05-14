#pragma once

#include <GameEngine2D/SpriteBatch.h>

#include "DynamicEntity.h"

class Bullet : public DynamicEntity
{
public:
	Bullet(glm::vec2 position, glm::vec2 velocity, float speed, int lifeTime);
	~Bullet();

	void draw(GameEngine2D::SpriteBatch& spriteBatch);

	//Returns true when _lifeTime ends
	bool update(const std::vector<Tile>& tiles);

private:

};

