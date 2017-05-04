#include "Bullet.h"

#include <GameEngine2D/ResourceManager.h>


Bullet::Bullet(glm::vec2 position, glm::vec2 direction, float speed, int lifeTime){
	_lifeTime = lifeTime;
	_position = position;
	_direction = direction;
	_speed = speed;
}

Bullet::~Bullet()
{
}

void Bullet::draw(GameEngine2D::SpriteBatch& spriteBatch){
	glm::vec4 posAndSize(_position, 10.0f, 10.0f);

	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);

	GLuint textureID = GameEngine2D::ResourceManager::getTexture("Textures/Bullet/bullet.png").id;

	GameEngine2D::Colour colour;
	colour.r = 255;
	colour.g = 255;
	colour.b = 255;
	colour.a = 255;


	spriteBatch.draw(posAndSize, uv, textureID, 0.0f, colour);

}

bool Bullet::update(){
	_position += _direction * _speed;
	_lifeTime--;

	if (!_lifeTime)
		return true;
	else
		return false;
}
