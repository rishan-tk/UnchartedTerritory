#include "InputManager.h"

namespace GameEngine2D {

	InputManager::InputManager() : _mouseCoordinates(0)
	{
	}


	InputManager::~InputManager()
	{
	}

	void GameEngine2D::InputManager::pressKey(unsigned int keyID) {
		_keyMap[keyID] = true;
	}

	void GameEngine2D::InputManager::releaseKey(unsigned int keyID) {
		_keyMap[keyID] = false;
	}

	//Keeps track of mouse coordinates
	void InputManager::setMouseCoords(float x, float y){
		_mouseCoordinates.x = x;
		_mouseCoordinates.y = y;
	}

	bool GameEngine2D::InputManager::isKeyPressed(unsigned int keyID) {
		auto it = _keyMap.find(keyID);
		if (it != _keyMap.end())
			return it->second;
		else
			return false;
	}

}