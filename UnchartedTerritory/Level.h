#pragma once

#include <string>
#include <vector>
#include <algorithm>

#include <GameEngine2D/SpriteBatch.h>

#include "Tile.h"

static struct SortTiles {
	bool operator()(Tile& a, Tile& b) {
		if (a.getPosition().y == b.getPosition().y)
			return (a.getPosition().x < b.getPosition().x);
		else
			return (a.getPosition().y < b.getPosition().y);
	}
}sortTiles;


class Level
{
public:
	//Constructor used to load the level
	Level(const std::string& fileName);
	~Level();

	void draw();

	glm::vec2 getPlayerStartPosition() const { return _playerStartPosition; }
	const std::vector<Tile>& getTiles() { return _tiles; }

private:
	void drawTiles();

	std::vector<std::string> _levelData;

	int _numPlayers;
	int _numHunters;

	glm::vec2 _playerStartPosition;
	std::vector<glm::vec2> _hunterStartPostion;

	std::vector<Tile> _tiles;

	GameEngine2D::SpriteBatch _spriteBatch;

};


