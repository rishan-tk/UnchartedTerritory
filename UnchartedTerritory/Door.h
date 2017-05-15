#pragma once

#include "Entity.h"

class Door : public Entity 
{
public:
	Door();
	~Door();

	void initialise(glm::vec2 position, int lifeTime);

	void draw(GameEngine2D::SpriteBatch& spriteBatch);

	//Return true if player is at opened door
	void update(int noOfCoins, glm::vec2 playerPosition);

	bool isColliding();

private:
	bool _atExit = false;

};
