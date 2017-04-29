#pragma once

#include <unordered_map>
#include <glm/glm.hpp>

namespace GameEngine2D {

	class InputManager
	{
	public:
		InputManager();
		~InputManager();

		void pressKey(unsigned int keyID);
		void releaseKey(unsigned int keyID);

		void setMouseCoords(float x, float y);

		bool isKeyPressed(unsigned int keyID);

		//Accessors
		glm::vec2 getMouseCoords() const;

	private:
		std::unordered_map<unsigned int, bool> _keyMap;

		glm::vec2 _mouseCoordinates;

	};

	inline glm::vec2 InputManager::getMouseCoords() const { return _mouseCoordinates; }

}

