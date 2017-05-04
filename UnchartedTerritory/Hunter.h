#pragma once

#include "Enemy.h"

class Hunter : public Enemy
{
public:
	Hunter();
	~Hunter();

	void initialise(glm::vec2 position, glm::vec2 direction, float speed, float lifeTime);
};

