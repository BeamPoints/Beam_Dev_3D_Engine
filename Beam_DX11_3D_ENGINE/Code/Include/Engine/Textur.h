#pragma once
#include <vector>

class CTextur
{

public:
	struct Texture2DDescriptor
	{
		std::vector<std::vector<uint8_t>> inData;
		unsigned int                      inByteSize;
		unsigned int                      width;
		unsigned int                      height;
		unsigned int                      depth;
		unsigned int                      channels;
		unsigned int                      mipLevels;		  
	};

	static bool LoadTextureArrayFromFiles(const std::vector<std::string> &filenames, Texture2DDescriptor &outImage);

	static bool LoadTextureFromFile(const char* filename, Texture2DDescriptor& outImage);

};

