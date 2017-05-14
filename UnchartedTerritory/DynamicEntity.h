#pragma once

#include "Entity.h"

class DynamicEntity : public Entity
{
public:
	DynamicEntity();
	virtual ~DynamicEntity();

	virtual void draw(GameEngine2D::SpriteBatch& spriteBatch);

protected:
	float _speed;
	glm::vec2 _velocity;
	glm::vec2 _velocityGoal;
	float _gravity;
};

