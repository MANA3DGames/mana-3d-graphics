#ifndef MANA3D_TEXTUREIMPORTER_H
#define MANA3D_TEXTUREIMPORTER_H

#include "Texture.h"


namespace MANA3D
{
#define TGA_RGB		 2		// This tells us it's a normal RGB (really BGR) file
#define TGA_A		 3		// This tells us it's a ALPHA file
#define TGA_RLE		10		// This tells us that the targa is Run-Length Encoded (RLE)

	extern bool ImportBitmap( const char*, Texture* );
	extern bool ImportTGA( const char*, Texture* );
	extern bool ImportJPG( const char*, Texture* );
	
	
	extern void InitDevIL();
	extern bool ImportTexture( const char *theFileName, Texture *texture );
}

#endif
