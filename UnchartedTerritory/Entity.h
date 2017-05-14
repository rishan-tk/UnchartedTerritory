#pragma once

#include <glm/glm.hpp>

#include <GameEngine2D/SpriteBatch.h>
#include <GameEngine2D/Errors.h>
#include <GameEngine2D/ResourceManager.h>
#include <GameEngine2D/SpriteSheet.h>

#include "Tile.h"

 enum EntityState {
	ENTITY,
	BULLET
};


class Entity
{
public:
	Entity();
	virtual ~Entity();

	virtual void draw(GameEngine2D::SpriteBatch& spriteBatch) = 0;

	glm::vec2 getPosition() const { return _position; }

protected:
	bool checkIfColliding(const glm::vec2& position1, const glm::vec2& position2, const float DIMENSION);

	int checkCollidableTiles(const std::vector<Tile>& tiles,
						   std::vector<Tile>& collidableTiles,
						   glm::vec2 position,
						   const float DIMENSION);

	bool collideWithLevel(const std::vector<Tile>& tiles, const glm::vec2& position, EntityState state);

	void collideWithTile(Tile tile);

	int _lifeTime;
	int _aliveFor;

	int _textureIndex;

	glm::vec2 _position;

	GameEngine2D::SpriteSheet _texture;

	EntityState _currentState;

};

