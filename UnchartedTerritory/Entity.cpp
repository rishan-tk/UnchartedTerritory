#include "Entity.h"

#include <algorithm>
#include <iostream>


Entity::Entity() : _currentState(ENTITY){
}


Entity::~Entity()
{
}

void Entity::draw(GameEngine2D::SpriteBatch & spriteBatch){
}

//Bounding Box Collision Detection
bool Entity::checkIfColliding(const glm::vec2& position1, const glm::vec2& position2, const float DIMENSION){
	//If on of the conditions are true, they are not colliding (tweaking numbers for more precise collisions)
	if (((position1.x + DIMENSION) < position2.x + 10
		|| (position2.x + TILE_DIMENSION) - 10 < position1.x
		|| (position1.y + DIMENSION) < position2.y + 8
		|| (position2.y + TILE_DIMENSION) - 10 < position1.y))
		return false;
	else
		return true;
}

int Entity::checkCollidableTiles(const std::vector<Tile>& tiles, std::vector<Tile>& collidableTiles, glm::vec2 position, const float DIMENSION) {

	//Bottom Left Tile 
	Tile bottomLeft = tiles[(int)(floor(position.x / TILE_DIMENSION) +
		(floor(position.y / TILE_DIMENSION) * MAP_DIMENSION))];

	//Bottom Right Tile
	Tile bottomRight = tiles[(int)(floor((position.x + DIMENSION) / TILE_DIMENSION) +
		(floor(position.y / TILE_DIMENSION) * MAP_DIMENSION))];

	//Top Left Tile
	Tile topLeft = tiles[(int)(floor(position.x / TILE_DIMENSION) +
		(floor((position.y + DIMENSION) / TILE_DIMENSION) * MAP_DIMENSION))];

	//Top Right Tile
	Tile topRight = tiles[(int)(floor((position.x + DIMENSION) / TILE_DIMENSION) +
		(floor((position.y + DIMENSION) / TILE_DIMENSION) * MAP_DIMENSION))];

	//Check if Bottom Left Tile is collidable
	if (bottomLeft.isCollidable()) {
		//Check if it is colliding
		if (checkIfColliding(position, bottomLeft.getPosition(), DIMENSION))
			collidableTiles.push_back(bottomLeft);
	}

	//Check if Bottom Right Tile is collidable
	if (bottomRight.isCollidable()) {
		//Check if it is colliding
		if (checkIfColliding(position, bottomRight.getPosition(), DIMENSION))
			collidableTiles.push_back(bottomRight);
	}

	//Check if Top Left Tile is collidable
	if (topLeft.isCollidable()) {
		//Check if it is colliding
		if (checkIfColliding(position, topLeft.getPosition(), DIMENSION))
			collidableTiles.push_back(topLeft);
	}

	//Check if Top Right Tile is collidable
	if (topRight.isCollidable()) {
		//Check if it is colliding
		if (checkIfColliding(position, topRight.getPosition(), DIMENSION))
			collidableTiles.push_back(topRight);
	}

	if (collidableTiles.size() == 0)
		return 30; ///< Error number

	//Check for closest tile
	glm::vec2 closestPos(128.0f, 128.0f);
	int collidableTileIndex = 0;

	for (int i = 0; i < (int)collidableTiles.size(); i++) {
		if (glm::length(collidableTiles[i].getPosition() - _position) < glm::length(closestPos)) {
			closestPos = collidableTiles[i].getPosition() - _position;
			collidableTileIndex = i;
		}
	}

	return collidableTileIndex;
}

bool Entity::collideWithLevel(const std::vector<Tile>& tiles, const glm::vec2& position, EntityState state) {

	std::vector<Tile> collidableTiles;
	bool collision;

	int collidableTileIndex;

	if (state == ENTITY) {
		if (position.x > 0 && position.y > 0) {
			collidableTileIndex = checkCollidableTiles(tiles, collidableTiles, position, ENTITY_DIMENSION);

			if(collidableTileIndex == 30)
				collision = false;
			else 
				collision = true;
			
		}else {
			collision = false;
			std::cout << "Collision check out of bounds" << std::endl;
		}
	}else if (state == BULLET) {
		if (position.x > 0 && position.y > 0){
			collidableTileIndex = checkCollidableTiles(tiles, collidableTiles, position, BULLET_DIMENSION);
			if (collidableTileIndex == 30)
				collision = false;
			else
				collision = true;
		}else {
			collision = false;
			std::cout << "Collision check out of bounds" << std::endl;
		}
	}


	if (collision) {
 		collideWithTile(collidableTiles[collidableTileIndex]); ///<Handle the collision
		return collision;
	}else
		return collision;
	
}

void Entity::collideWithTile(Tile tile){

	//Distance of entity from tile
	glm::vec2 distVec = (_position + glm::vec2(ENTITY_DIMENSION/2, ENTITY_DIMENSION/2)) - (tile.getPosition() + glm::vec2(ENTITY_DIMENSION/2, ENTITY_DIMENSION/2));

	float xDepth = ENTITY_DIMENSION - abs(distVec.x);
	float yDepth = ENTITY_DIMENSION - abs(distVec.y);

	//Collision Response
	if (yDepth > 0 || xDepth > 0) {
		if (std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f)) {
			if (distVec.x < 0)
				_position.x -= xDepth;
			else
				_position.x += xDepth;
		}
		else {
			if (distVec.y < 0)
				_position.y -= yDepth;
			else
				_position += yDepth;
		}
	}

}
