#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> //Used to build orthographic projection matrix

//Include all the files in 2DGameEngine namespace
namespace GameEngine2D {

	class Camera2D
	{
	public:
		Camera2D();
		~Camera2D();

		void init(int screenWidth, int screenHeight);

		void update();

		//Mutators(Setters)
		void setPosition(glm::vec2& newPosition) { _position = newPosition; _updateMatrix = true; }
		void setScale(float newScale) { _scale = newScale; _updateMatrix = true; }

		//Accessors(Getters)
		glm::vec2 getPosition() { return _position; }
		float getScale() { return _scale; }
		glm::mat4 getCameraMatrix() { return _cameraMatrix; }

	private:
		int _screenWidth, _screenHeight;
		bool _updateMatrix;
		float _scale;

		glm::vec2 _position;
		glm::mat4 _cameraMatrix;
		glm::mat4 _orthoMatrix;

	};

}