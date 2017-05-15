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
	glm::vec2 getDoorStartPosition() const { return _doorStartPosition; }
	const std::vector<Tile>& getTiles() { return _tiles; }
	int getNoCoins() const { return _numCoins; }
	glm::vec2 getCoinStartPosition(int index) const { return _coinStartPositions[index]; }

private:
	void drawTiles();

	std::vector<std::string> _levelData;

	int _numPlayers;
	int _numHunters;
	int _numCoins;

	glm::vec2 _playerStartPosition;
	glm::vec2 _doorStartPosition;
	std::vector<glm::vec2> _hunterStartPostion;
	std::vector<glm::vec2> _coinStartPositions;

	std::vector<Tile> _tiles;

	GameEngine2D::SpriteBatch _spriteBatch;

};


