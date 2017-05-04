#include "Camera2D.h"

//Include all the files in 2DGameEngine namespace
namespace GameEngine2D {

	Camera2D::Camera2D() : 
		_screenDimensions(500.0f, 500.0f),
		_updateMatrix(true),
		_scale(1.0f),
		_position(0.0f, 0.0f),
		_cameraMatrix(1.0f),
		_orthoMatrix(1.0f)
	{
	}


	Camera2D::~Camera2D()
	{
	}

	void Camera2D::initialise(glm::vec2 screenDimensions){
		_screenDimensions = screenDimensions;
		_orthoMatrix = glm::ortho(0.0f, _screenDimensions.x, 0.0f, _screenDimensions.y);
	}

	void Camera2D::update(){

		if (_updateMatrix) {
			//Camera Translation 
			glm::vec3 translate(-_position.x + _screenDimensions.x/2.0f, -_position.y + _screenDimensions.y/2.0f, 0.0f);
			//Translate the camera matrix by our position
			_cameraMatrix = glm::translate(_orthoMatrix, translate);

			//Camera Scale
			glm::vec3 scale(_scale, _scale, 0.0f);
			//Scale the camera matrix
			_cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * _cameraMatrix;

			_updateMatrix = false;
		}

	}

	glm::vec2 Camera2D::convertScreentoWorldCoords(glm::vec2 screenCoords){
		//Invert y direction
		screenCoords.y = _screenDimensions.y - screenCoords.y;
		//Set it so (0,0) is the center
		screenCoords -= glm::vec2(_screenDimensions.x / 2, _screenDimensions.y / 2);
		//Scale the coordinates
		screenCoords /= _scale;
		//Translate with camera position
		screenCoords += _position;

		return screenCoords;
	}

}