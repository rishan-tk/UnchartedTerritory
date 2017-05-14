#include "Bullet.h"

#include <GameEngine2D/ResourceManager.h>


Bullet::Bullet(glm::vec2 position, glm::vec2 velocity, float speed, int lifeTime){
	_position = position;
	_gravity = 0.5f;;
	_velocity = velocity;
	_speed = speed;
	_lifeTime = lifeTime;
	_currentState = BULLET;
}

Bullet::~Bullet()
{
}

void Bullet::draw(GameEngine2D::SpriteBatch& spriteBatch){
	glm::vec4 posAndSize(_position, BULLET_DIMENSION, BULLET_DIMENSION);

	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);

	GLuint textureID = GameEngine2D::ResourceManager::getTexture("Textures/Bullet/bullet.png").id;

	GameEngine2D::ColourRGBA8 colour(255, 255, 255, 255);

	spriteBatch.draw(posAndSize, uv, textureID, 0.0f, colour);

}

bool Bullet::update(const std::vector<Tile>& tiles) {

	if (!collideWithLevel(tiles, _position, _currentState)){
		_position += (_velocity * _speed);
		_lifeTime--;
		return false;
	}else {
		_lifeTime = 0;
		return true;
	}

}
