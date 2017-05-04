#include "Level.h"

#include <fstream>

#include <GameEngine2D/Errors.h>
#include <GameEngine2D/ResourceManager.h>


Level::Level(const std::string & fileName)
{
	std::ifstream file(fileName);

	//Error check file
	if (file.fail())
		GameEngine2D::fatalError("Failed to open file: " + fileName);

	std::string tempString;
	//Throw away first word into a string and read in the number
	file >> tempString >> _numHunters;

	//Discard the rest of the line
	std::getline(file, tempString);

	//Read in the rest of the level into _levelData
	while (std::getline(file, tempString)) 
		_levelData.push_back(tempString);
	
	_spriteBatch.initialise();
	_spriteBatch.begin();

	//Render all the tiles and invert the y-axis 
	for (int row = 0; row < (int)_levelData.size(); row++) {
		for (int column = 0; column < (int)_levelData[row].size(); column++) {

			glm::vec2 position(column * ENTITY_DIMENSION, (MAP_DIMENSION - row - 1) * ENTITY_DIMENSION);
			glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
			GameEngine2D::ColourRGBA8 colour(255, 255, 255, 255);


			switch (_levelData[row][column]) {
				case 'P':
					_playerStartPosition = position;
					//_spriteBatch.draw(destRect, uvRect, GameEngine2D::ResourceManager::getTexture("Textures/SpriteSheet/Froglvl1.png").id, 0.0f, colour);
					_tiles.emplace_back(position, EARTH);
					break;
				case 'H':
					_hunterStartPostion.emplace_back(position);
					//_spriteBatch.draw(destRect, uvRect, GameEngine2D::ResourceManager::getTexture("Textures/SpriteSheet/Froglvl1.png").id, 0.0f, colour);
					_tiles.emplace_back(position, EARTH);
					break;
				case '.':
					_tiles.emplace_back(position, EARTH);
					break;
				case 'R':
					_tiles.emplace_back(position, ROAD);
					break;
				case 'G':
					_tiles.emplace_back(position, GRASS);
					break;
				default:
					GameEngine2D::fatalError("Unexpected symbol " + _levelData[row][column]);
			}
		}
	}

	std::sort(_tiles.begin(), _tiles.end(), sortTiles);

	drawTiles();

	_spriteBatch.end();

}

Level::~Level()
{
}

void Level::draw(){
	_spriteBatch.renderBatch();
}


void Level::drawTiles(){
	for (int i = 0; i < (int)_tiles.size(); i++)
		_tiles[i].draw(_spriteBatch);
}
