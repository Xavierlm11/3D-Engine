#pragma once
#include "MathGeoLib/include/Algorithm/Random/LCG.h"
#include "SDL/include/SDL_stdinc.h"
//#include <string.h>
#include<string>

class Resource
{
public:
	enum class Types
	{
		NONE,
		MODEL,
		MESH,
		MATERIAL,
	};

	Resource(const char* path, Types type) {

		assetPath = path;

		std::string fileName_s = assetPath.substr(assetPath.find_last_of('\\') + 1);
		fileName_s = fileName_s.substr(fileName_s.find_last_of('/') + 1);
		fileName = fileName_s;

		std::string assetName_s = fileName.substr(0, fileName.find_last_of('.'));
		assetName = assetName_s;

		resourceType = type;

		assetID = random.Int();
	}	

	Resource(Types type) {
		resourceType = type;
		assetID = random.Int();
	}

	~Resource() {};
	Uint64 GetNewID() {
		assetID = random.Int();
		return assetID;
	}


public:
	std::string assetName;
	std::string fileName;
	std::string assetPath;
	Types resourceType;
	Uint64 assetID=0;
	LCG random;
};


