#pragma once

#include <glm/glm.hpp>

#include <GameEngine2D/SpriteBatch.h>
#include <GameEngine2D/Errors.h>
#include <GameEngine2D/ResourceManager.h>

#include "Tile.h"




class Entity
{
public:
	Entity();
	virtual ~Entity();

	virtual void draw(GameEngine2D::SpriteBatch& spriteBatch) = 0;

	glm::vec2 getPosition() const { return _position; }

protected:
	bool checkIfColliding(glm::vec2& position, Tile& tile);

	void checkCollidableTiles(const std::vector<Tile>& tiles,
						   std::vector<Tile>& collidableTiles,
						   glm::vec2 position);

	void collideWithLevel(const std::vector<Tile>& tiles);

	void collideWithTile(Tile tile);

	glm::vec2 _position;

};

