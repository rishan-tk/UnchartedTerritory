#include "InputManager.h"

namespace GameEngine2D {

	InputManager::InputManager()
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

	bool GameEngine2D::InputManager::isKeyPressed(unsigned int keyID) {
		auto it = _keyMap.find(keyID);
		if (it != _keyMap.end())
			return it->second;
		else
			return false;
	}

}