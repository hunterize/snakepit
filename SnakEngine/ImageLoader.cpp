#include "ImageLoader.h"

namespace SnakEngine
{
	GLTexture ImageLoader::LoadPNG(const std::string& filePath)
	{
		GLTexture texture = {};

		std::vector<unsigned char> in;
		std::vector<unsigned char> out;

		unsigned long height, width;

		if (IOManager::ReadFiletoBuffer(filePath, in) == false)
		{
			fatalError("Failed to load PNG file to buffer");
		}

		int errorcode = decodePNG(out, width, height, &in[0], in.size());
		if (errorcode != 0)
		{
			fatalError("decodePNG failed with error: " + std::to_string(errorcode));
		}

		glGenTextures(1, &texture.ID);
		glBindTexture(GL_TEXTURE_2D, texture.ID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &out[0]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);

		texture.width = width;
		texture.height = height;

		return texture;

	}
}