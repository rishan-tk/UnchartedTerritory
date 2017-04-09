#pragma once

#include <vector>
#include <string>

//Static class - only one is needed 
//Used for reading and writing files
class IOManager
{
public:
	//Load file
	static bool readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer); 
};

