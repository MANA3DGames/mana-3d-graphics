#include "Texture.h"
#include "MString.h"
#include "TextureImporter.h"
using namespace MANA3D;


Texture::Texture()
{
	fileName = NULL;
	width = 0;
	height = 0;
	channels = 0;
	ID = (int)allTextures.size();
}


Texture::~Texture()
{

}

bool Texture::Import( const char *fileName )
{
	bool success;
	int fileName_len = strlen( fileName );
	if ( fileName_len >= 4 ) 
	{
		const char* extensionStr = fileName + fileName_len - 4;
		char* temp = MString::ToLower( extensionStr );
		if ( strcmp( temp, ".jpg" ) == 0 ||
			 strcmp( temp, "jpeg" ) == 0 )
			success = ImportJPG( fileName, this );
		else if ( strcmp( temp, ".bmp") == 0 )
			success = ImportBitmap( fileName, this );
		else if ( strcmp( temp, ".tga") == 0 )
			success = ImportTGA( fileName, this );
	}
	
	return success;
}



vector<Texture*> Texture::allTextures;

Texture* Texture::Create()
{
	Texture *tex = new Texture();
	allTextures.push_back( tex );
	return tex;
}

void Texture::DestroyAll()
{
	while ( !allTextures.empty() )
	{
		Texture* temp = *allTextures.begin();
		allTextures.erase( allTextures.begin() );
		delete temp;
	}

	allTextures.clear();
}

Texture* Texture::GetWithName( const char* name )
{
	for ( std::vector<Texture*>::iterator it = allTextures.begin(); it != allTextures.end(); it++ )
	{
		if ( name == (*it)->name )
			return *it;
	}

	return NULL;
}