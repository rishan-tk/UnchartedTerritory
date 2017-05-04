#pragma once

#include "Entity.h"

class DynamicEntity : public Entity
{
public:
	DynamicEntity();
	virtual ~DynamicEntity();

	virtual void draw(GameEngine2D::SpriteBatch& spriteBatch);

protected:
	int _lifeTime;
	float _speed;
	glm::vec2 _direction;
};

