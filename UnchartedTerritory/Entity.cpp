#include "Entity.h"

#include <algorithm>


Entity::Entity(){
}


Entity::~Entity()
{
}

void Entity::draw(GameEngine2D::SpriteBatch & spriteBatch){
}

bool Entity::checkIfColliding(glm::vec2 & position, Tile & tile){
	//If on of the conditions are true, they are not colliding
	if (((position.x + ENTITY_DIMENSION) < tile.getPosition().x
		|| (tile.getPosition().x + ENTITY_DIMENSION) < position.x
		|| (position.y + ENTITY_DIMENSION) < tile.getPosition().y
		|| (tile.getPosition().y + ENTITY_DIMENSION) < position.y))
		return false;
	else
		return true;
}

void Entity::checkCollidableTiles(const std::vector<Tile>& tiles, std::vector<Tile>& collidableTiles, glm::vec2 position){

	//Bottom Left Tile 
	Tile bottomLeft = tiles[(int)(floor(position.x / (float)ENTITY_DIMENSION) +
		(floor(position.y / (float)ENTITY_DIMENSION) * MAP_DIMENSION))];

	//Bottom Right Tile
	Tile bottomRight = tiles[(int)(floor((position.x + ENTITY_DIMENSION) / (float)ENTITY_DIMENSION) +
		(floor(position.y / (float)ENTITY_DIMENSION) * MAP_DIMENSION))];

	//Top Left Tile
	Tile topLeft = tiles[(int)(floor(position.x / (float)ENTITY_DIMENSION) +
		(floor((position.y + ENTITY_DIMENSION) / (float)ENTITY_DIMENSION) * MAP_DIMENSION))];

	//Top Right Tile
	Tile topRight = tiles[(int)(floor((position.x + ENTITY_DIMENSION) / (float)ENTITY_DIMENSION) +
		(floor((position.y + ENTITY_DIMENSION) / (float)ENTITY_DIMENSION) * MAP_DIMENSION))];

	//Check if Bottom Left Tile is collidable
	if (bottomLeft.isCollidable()) {
		//Check if it is colliding
		if(checkIfColliding(position, bottomLeft))
			collidableTiles.push_back(bottomLeft);	
	}

	//Check if Bottom Right Tile is collidable
	if (bottomRight.isCollidable()) {
		//Check if it is colliding
		if (checkIfColliding(position, bottomRight))
			collidableTiles.push_back(bottomRight);
	}

	//Check if Top Left Tile is collidable
	if (topLeft.isCollidable()) {
		//Check if it is colliding
		if (checkIfColliding(position, topLeft))
			collidableTiles.push_back(topLeft);
	}

	//Check if Top Right Tile is collidable
	if (topRight.isCollidable()) {
		//Check if it is colliding
		if (checkIfColliding(position, topRight))
			collidableTiles.push_back(topRight);
	}
}

void Entity::collideWithLevel(const std::vector<Tile>& tiles){

	std::vector<Tile> collidableTiles;

	//Check if any of the 4 Tiles around are collidable
	checkCollidableTiles(tiles, collidableTiles, glm::vec2(_position.x, _position.y));

	//Handle the collision
	for (int i = 0; i < (int)collidableTiles.size(); i++)
		collideWithTile(collidableTiles[i]);
	
}

void Entity::collideWithTile(Tile tile){


	//Distance of entity from tile
	glm::vec2 distVec = _position - tile.getPosition() - glm::vec2(ENTITY_DIMENSION/2, ENTITY_DIMENSION/2);

	float xDepth = ENTITY_DIMENSION - abs(distVec.x);
	float yDepth = ENTITY_DIMENSION - abs(distVec.y);


	if (xDepth > 0 || yDepth > 0) {
		if (std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f)) {
			if (distVec.x < 0)
				_position.x -= xDepth;
			else
				_position.x += xDepth;
		}else{
			if (distVec.y < 0)
				_position.y -= yDepth;
			else
				_position.y += yDepth;
		}

	}

}
