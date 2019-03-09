#ifndef MANA3D_TEXTURE_H
#define MANA3D_TEXTURE_H

#include <Windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>

#include <vector>
using std::vector;


namespace MANA3D
{
	class Texture
	{
	private:
		Texture();
		Texture( const char* );
		~Texture();


	public:
		int width;						// The width of the image in pixels
		int height;						// The height of the image in pixels

		char *fileName;					// Image path.
		const char *name;

		int channels;					// The channels in the image (3 = RGB : 4 = RGBA)

		GLuint ID;						// Texture ID

		bool Import( const char* );
		

	private:
		static vector<Texture*> allTextures;

	public:
		static Texture* Create();
		static void DestroyAll();

		static Texture* GetWithName( const char* name );

	};

}

#endif
