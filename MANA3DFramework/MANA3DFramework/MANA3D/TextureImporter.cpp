#include "TextureImporter.h"
#include "MString.h"
using namespace MANA3D;

#include "jpeglib.h"
#include <stdio.h>
//#include <png.h>

#define ILUT_USE_OPENGL	
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

using namespace std;


bool MANA3D::ImportBitmap( const char *fileName, Texture *texture )
{
	//texture = (Texture*)malloc( sizeof(Texture) );
	/*texture->ID = allTextures();
	_textureCount++;*/
	
	// Create the texture.
	glGenTextures( 1, &texture->ID );

	HBITMAP hBMP = (HBITMAP)LoadImage( GetModuleHandle( NULL ), MString::ToLPWSTR( fileName ), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE );
	
	// Check for error (Does the Bitmap exist?).
	if ( !hBMP )
		return FALSE;

	// Bitmap structure.
	BITMAP bitmap;

	// Get the object.
	GetObject( hBMP, sizeof(BITMAP), &bitmap );
	
	// Pixel Storage Mode (Word Alignment / 4 Bytes).
	glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );



	// Typical Texture Generation Using Data From The Bitmap.

	// Bind To The Texture ID.
	glBindTexture( GL_TEXTURE_2D, texture->ID );
	// Linear Min Filter.
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	// Linear Mag Filter.
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	// MUST NOT BE INDEX BMP, BUT RGB.
	glTexImage2D( GL_TEXTURE_2D, 0, 3, bitmap.bmWidth, bitmap.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, bitmap.bmBits );

	texture->fileName = _strdup( fileName );
	texture->width = bitmap.bmWidth;
	texture->height = bitmap.bmHeight;


	// Delete The Object.
	DeleteObject( hBMP );

	return TRUE;
}

bool MANA3D::ImportTGA( const char *fileName, Texture *texture )
{
	if ( fileName == NULL ) return FALSE;

	unsigned char* data;

	WORD width = 0, height = 0;			// The dimensions of the image
	byte length = 0;					// The length in bytes to the pixels
	byte imageType = 0;					// The image type (RLE, RGB, Alpha...)
	byte bits = 0;						// The bits per pixel for the image (16, 24, 32)
	FILE* pFile = NULL;					// The file pointer
	int channels = 0;					// The channels of the image (3 = RGA : 4 = RGBA)
	int stride = 0;						// The stride (channels * width)
	int i = 0;							// A counter

	// This function loads in a TARGA (.TGA) file and returns its data to be
	// used as a texture or what have you.  This currently loads in a 16, 24
	// and 32-bit targa file, along with RLE compressed files.  Eventually you
	// will want to do more error checking to make it more robust.  This is
	// also a perfect start to go into a modular class for an engine.
	// Basically, how it works is, you read in the header information, then
	// move your file pointer to the pixel data.  Before reading in the pixel
	// data, we check to see the if it's an RLE compressed image.  This is because
	// we will handle it different.  If it isn't compressed, then we need another
	// check to see if we need to convert it from 16-bit to 24 bit.  24-bit and
	// 32-bit textures are very similar, so there's no need to do anything special.
	// We do, however, read in an extra bit for each color.


	fopen_s( &pFile, fileName, "rb" );
	// Open a file pointer to the targa file and check if it was found and opened 
	//if ((pFile = fopen(filename, "rb")) == NULL)
	if ( pFile == NULL )
	{
		// Display an error message saying the file was not found, then return NULL
		//MessageBox(g_hWnd, "Unable to load TGA File!", "Error", MB_OK);
		return FALSE;
	}

	// Read in the length in bytes from the header to the pixel data
	fread(&length, sizeof(byte), 1, pFile);

	// Jump over one byte
	fseek(pFile, 1, SEEK_CUR);

	// Read in the imageType (RLE, RGB, etc...)
	fread(&imageType, sizeof(byte), 1, pFile);

	// Skip past general information we don't care about
	fseek(pFile, 9, SEEK_CUR);

	// Read the width, height and bits per pixel (16, 24 or 32)
	fread(&width, sizeof(WORD), 1, pFile);
	fread(&height, sizeof(WORD), 1, pFile);
	fread(&bits, sizeof(byte), 1, pFile);

	// Now we move the file pointer to the pixel data
	fseek(pFile, length + 1, SEEK_CUR);

	// Check if the image is RLE compressed or not
	if (imageType != TGA_RLE)
	{
		// Check if the image is a 24 or 32-bit image
		if (bits == 24 || bits == 32)
		{
			// Calculate the channels (3 or 4) - (use bits >> 3 for more speed).
			// Next, we calculate the stride and allocate enough memory for the pixels.
			channels = bits / 8;
			stride = channels * width;
			data = new unsigned char[stride * height];

			// Load in all the pixel data line by line
			for (int y = 0; y < height; y++)
			{
				// Store a pointer to the current line of pixels
				unsigned char *pLine = &(data[stride * y]);

				// Read in the current line of pixels
				fread(pLine, stride, 1, pFile);

				// Go through all of the pixels and swap the B and R values since TGA
				// files are stored as BGR instead of RGB (or use GL_BGR_EXT verses GL_RGB)
				for (i = 0; i < stride; i += channels)
				{
					int temp = pLine[i];
					pLine[i] = pLine[i + 2];
					pLine[i + 2] = temp;
				}
			}
		}
		// Check if the image is a 16 bit image (RGB stored in 1 unsigned short)
		else if (bits == 16)
		{
			unsigned short pixels = 0;
			int r = 0, g = 0, b = 0;

			// Since we convert 16-bit images to 24 bit, we hardcode the channels to 3.
			// We then calculate the stride and allocate memory for the pixels.
			channels = 3;
			stride = channels * width;
			data = new unsigned char[stride * height];

			// Load in all the pixel data pixel by pixel
			for (int i = 0; i < width*height; i++)
			{
				// Read in the current pixel
				fread(&pixels, sizeof(unsigned short), 1, pFile);

				// To convert a 16-bit pixel into an R, G, B, we need to
				// do some masking and such to isolate each color value.
				// 0x1f = 11111 in binary, so since 5 bits are reserved in
				// each unsigned short for the R, G and B, we bit shift and mask
				// to find each value.  We then bit shift up by 3 to get the full color.
				b = (pixels & 0x1f) << 3;
				g = ((pixels >> 5) & 0x1f) << 3;
				r = ((pixels >> 10) & 0x1f) << 3;

				// This essentially assigns the color to our array and swaps the
				// B and R values at the same time.
				data[i * 3 + 0] = r;
				data[i * 3 + 1] = g;
				data[i * 3 + 2] = b;
			}
		}
		// Else return a NULL for a bad or unsupported pixel format
		else
			return FALSE;
	}
	// Else, it must be Run-Length Encoded (RLE)
	else
	{
		// First, let me explain real quickly what RLE is.  
		// For further information, check out Paul Bourke's intro article at: 
		// http://astronomy.swin.edu.au/~pbourke/dataformats/rle/
		// 
		// Anyway, we know that RLE is a basic type compression.  It takes
		// colors that are next to each other and then shrinks that info down
		// into the color and a integer that tells how much of that color is used.
		// For instance:
		// aaaaabbcccccccc would turn into a5b2c8
		// Well, that's fine and dandy and all, but how is it down with RGB colors?
		// Simple, you read in an color count (rleID), and if that number is less than 128,
		// it does NOT have any optimization for those colors, so we just read the next
		// pixels normally.  Say, the color count was 28, we read in 28 colors like normal.
		// If the color count is over 128, that means that the next color is optimized and
		// we want to read in the same pixel color for a count of (colorCount - 127).
		// It's 127 because we add 1 to the color count, as you'll notice in the code.

		// Create some variables to hold the rleID, current colors read, channels, & stride.
		byte rleID = 0;
		int colorsRead = 0;
		channels = bits / 8;
		stride = channels * width;

		// Next we want to allocate the memory for the pixels and create an array,
		// depending on the channel count, to read in for each pixel.
		data = new unsigned char[stride * height];
		byte *pColors = new byte[channels];

		// Load in all the pixel data
		while (i < width*height)
		{
			// Read in the current color count + 1
			fread(&rleID, sizeof(byte), 1, pFile);

			// Check if we don't have an encoded string of colors
			if (rleID < 128)
			{
				// Increase the count by 1
				rleID++;

				// Go through and read all the unique colors found
				while (rleID)
				{
					// Read in the current color
					fread(pColors, sizeof(byte)* channels, 1, pFile);

					// Store the current pixel in our image array
					data[colorsRead + 0] = pColors[2];
					data[colorsRead + 1] = pColors[1];
					data[colorsRead + 2] = pColors[0];

					// If we have a 4 channel 32-bit image, assign one more for the alpha
					if (bits == 32)
						data[colorsRead + 3] = pColors[3];

					// Increase the current pixels read, decrease the amount
					// of pixels left, and increase the starting index for the next pixel.
					i++;
					rleID--;
					colorsRead += channels;
				}
			}
			// Else, let's read in a string of the same character
			else
			{
				// Minus the 128 ID + 1 (127) to get the color count that needs to be read
				rleID -= 127;

				// Read in the current color, which is the same for a while
				fread(pColors, sizeof(byte)* channels, 1, pFile);

				// Go and read as many pixels as are the same
				while (rleID)
				{
					// Assign the current pixel to the current index in our pixel array
					data[colorsRead + 0] = pColors[2];
					data[colorsRead + 1] = pColors[1];
					data[colorsRead + 2] = pColors[0];

					// If we have a 4 channel 32-bit image, assign one more for the alpha
					if (bits == 32)
						data[colorsRead + 3] = pColors[3];

					// Increase the current pixels read, decrease the amount
					// of pixels left, and increase the starting index for the next pixel.
					i++;
					rleID--;
					colorsRead += channels;
				}

			}

		}

		// Free up pColors
		delete[] pColors;
	}

	// Close the file pointer that opened the file
	fclose(pFile);


	// Allocate the structure that will hold our eventual image data (must free it!)
	//texture = (Texture*)malloc( sizeof(Texture) );
	
	// Fill in our tImageTGA structure to pass back
	texture->fileName = _strdup( fileName );
	texture->channels = channels;
	texture->width = width;
	texture->height = height;
	//texture->ID = _textureCount;
	//_textureCount++;

	// Generate a texture with the associative texture ID stored in the array.
	glGenTextures( 1, &texture->ID );

	// Bind the texture to the texture arrays index and init the texture.
	glBindTexture( GL_TEXTURE_2D, texture->ID );


	// Assume that the texture is a 24 bit RGB texture (We convert 16-bit ones to 24-bit).
	int textureType = GL_RGB;

	// If the image is 32-bit (4 channels), then we need to specify GL_RGBA for an alpha.
	if ( texture->channels == 4 )
		textureType = GL_RGBA;

	// Build Mipmaps (builds different versions of the picture for distances - looks better).
	gluBuild2DMipmaps( GL_TEXTURE_2D, texture->channels, texture->width, texture->height, textureType, GL_UNSIGNED_BYTE, data );


	// Lastly, we need to tell OpenGL the quality of our texture map.  GL_LINEAR_MIPMAP_LINEAR
	// is the smoothest.  GL_LINEAR_MIPMAP_NEAREST is faster than GL_LINEAR_MIPMAP_LINEAR, 
	// but looks blochy and pixilated.  Good for slower computers though. 

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	// Now we need to free the image data that we loaded since OpenGL stored it as a texture.
	if ( texture )										// If we loaded the image.
	{
		if ( data )										// If there is texture data.
			delete[] data;								// Free the texture data, we don't need it anymore.

		//free( texture );								// Free the image structure.
	}

	return TRUE;
}

bool MANA3D::ImportJPG( const char *fileName, Texture *texture  )
{
	// Return from the function if no file name was passed in.
	if ( !fileName ) 
		return FALSE;

	// Create a file pointer.
	FILE *filePtr;

	fopen_s( &filePtr, fileName, "rb" );

	// Open a file pointer to the jpeg file and check if it was found and opened.
	//if ( ( filePtr = fopen( fileName, "rb" ) ) == NULL )
	if ( filePtr == NULL )
	{
		// Display an error message saying the file was not found, then return NULL
		//MessageBox(g_hWnd, "Unable to load JPG File!", "Error", MB_OK);
		return NULL;
	}

	struct jpeg_decompress_struct jpgDecompressInfo;

	// Create an error handler.
	jpeg_error_mgr jpgErr;

	// Have our compression info object point to the error handler address.
	jpgDecompressInfo.err = jpeg_std_error(&jpgErr);

	// Initialize the decompression object.
	jpeg_create_decompress( &jpgDecompressInfo );

	// Specify the data source (Our file pointer).
	jpeg_stdio_src(&jpgDecompressInfo, filePtr);

	// Allocate the structure that will hold our eventual jpeg data (must free it!).
	tImageJPG* imageData = (tImageJPG*)malloc(sizeof(tImageJPG));

	// Decode the jpeg file and fill in the image data structure to pass back.
	jpeg_decompress_struct* info = &jpgDecompressInfo;
	// Read in the header of the jpeg file.
	jpeg_read_header( info, TRUE );

	// Start to decompress the jpeg file with our compression info.
	jpeg_start_decompress( info );

	// Get the image dimensions and row span to read in the pixel data.
	imageData->rowSpan = info->image_width * info->num_components;
	imageData->sizeX = info->image_width;
	imageData->sizeY = info->image_height;

	// Allocate memory for the pixel buffer
	imageData->data = new unsigned char[imageData->rowSpan * imageData->sizeY];

	// Here we use the library's state variable info.output_scanline as the
	// loop counter, so that we don't have to keep track ourselves.

	// Create an array of row pointers
	unsigned char** rowPtr = new unsigned char*[imageData->sizeY];
	for (int i = 0; i < imageData->sizeY; i++)
		rowPtr[i] = &(imageData->data[i*imageData->rowSpan]);

	// Now comes the juice of our work, here we extract all the pixel data
	int rowsRead = 0;
	while (info->output_scanline < info->output_height)
	{
		// Read in the current row of pixels and increase the rowsRead count
		rowsRead += jpeg_read_scanlines(info, &rowPtr[rowsRead], info->output_height - rowsRead);
	}

	// Delete the temporary row pointers
	delete[] rowPtr;

	// Finish decompressing the data
	jpeg_finish_decompress(info);


	// This releases all the stored memory for reading and decoding the jpeg.
	jpeg_destroy_decompress(&jpgDecompressInfo);

	// Close the file pointer that opened the file.
	fclose( filePtr );



	// If we can't load the file, quit!
	//if ( image == NULL )
	//	exit(0);
	if ( imageData == NULL )
		return FALSE;


	//texture = (Texture*)malloc( sizeof(Texture) );
	texture->fileName = _strdup( fileName );
	texture->width = imageData->sizeX;
	texture->height = imageData->sizeY;
	//texture->ID = _textureCount;
	//_textureCount++;


	// Generate a texture with the associative texture ID stored in the array.
	glGenTextures( 1, &texture->ID );

	// Bind the texture to the texture arrays index and init the texture.
	glBindTexture( GL_TEXTURE_2D, texture->ID );

	// Build Mipmaps (builds different versions of the picture for distances - looks better)
	gluBuild2DMipmaps( GL_TEXTURE_2D, 3, imageData->sizeX, imageData->sizeY, GL_RGB, GL_UNSIGNED_BYTE, imageData->data );

	// Lastly, we need to tell OpenGL the quality of our texture map.  GL_LINEAR_MIPMAP_LINEAR
	// is the smoothest.  GL_LINEAR_MIPMAP_NEAREST is faster than GL_LINEAR_MIPMAP_LINEAR, 
	// but looks blochy and pixilated.  Good for slower computers though. 

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	// Now we need to free the image data that we loaded since OpenGL stored it as a texture

	if ( imageData )										// If we loaded the image
	{
		if ( imageData->data )								// If there is texture data
		{
			free( imageData->data );						// Free the texture data, we don't need it anymore
		}

		//free( imageData );									// Free the image structure
	}

	return TRUE;
}



void MANA3D::InitDevIL()
{
	// Initialise all DevIL functionality
	ilutRenderer( ILUT_OPENGL );

	ilInit();
	iluInit();
	ilutInit();

	// Tell DevIL that we're using OpenGL for our rendering
	ilutRenderer( ILUT_OPENGL );	
}

bool MANA3D::ImportTexture( const char *theFileName, Texture *texture )
{
	ILuint imageID;				// Create a image ID as a ULuint
	GLuint textureID;			// Create a texture ID as a GLuint
	ILboolean success;			// Create a flag to keep track of success/failure
	ILenum error;				// Create a flag to keep track of the IL error state

	ilGenImages( 1, &imageID ); 		// Generate the image ID

	ilBindImage( imageID ); 			// Bind the image


	//int BUFFER_SIZE = 100;
	//size_t  count;
	//char    *pMBBuffer = (char *)malloc(BUFFER_SIZE);
	//const wchar_t *pWCBuffer = ToWCharT( theFileName );// L"img.jpg";

	//count = wcstombs( pMBBuffer, pWCBuffer, BUFFER_SIZE ); // C4996
	//MessageBoxA( 0, pMBBuffer, 0, 0 );												 // Note: wcstombs is deprecated; consider using wcstombs_s instead
	//free(pMBBuffer);

	success = ilLoadImage( MString::ToWChar_T( theFileName ) ); 	// Load the image file


	// If we managed to load the image, then we can start to do things with it...
	if ( success )
	{
		// If the image is flipped (i.e. upside-down and mirrored, flip it the right way up!)
		ILinfo ImageInfo;
		iluGetImageInfo( &ImageInfo );
		if ( ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT )
		{
			iluFlipImage();
		}

		// ... then attempt to conver it.
		// NOTE: If your image contains alpha channel you can replace IL_RGB with IL_RGBA
		success = ilConvertImage( IL_RGB, IL_UNSIGNED_BYTE );

		// Quit out if we failed the conversion
		if ( !success )
		{
			error = ilGetError();
			//cout << "Image conversion failed - IL reports error: " << error << endl;
			MessageBoxA( NULL, "Quit out if we failed the conversion", NULL, NULL );
			exit( -1 );
		}

		// Generate a new texture
		glGenTextures( 1, &textureID );

		// Bind the texture to a name
		glBindTexture( GL_TEXTURE_2D, textureID );

		// Set texture clamping method
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

		// Set texture interpolation method to use linear interpolation (no MIPMAPS)
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

		// Specify the texture specification
		glTexImage2D( GL_TEXTURE_2D, 						// Type of texture
					  0,									// Pyramid level (for mip-mapping) - 0 is the top level
					  ilGetInteger( IL_IMAGE_BPP ),			// Image colour depth
					  ilGetInteger( IL_IMAGE_WIDTH ),		// Image width
					  ilGetInteger( IL_IMAGE_HEIGHT ),		// Image height
					  0,									// Border width in pixels (can either be 1 or 0)
					  ilGetInteger( IL_IMAGE_FORMAT ),		// Image format (i.e. RGB, RGBA, BGR etc.)
					  GL_UNSIGNED_BYTE,						// Image data type
					  ilGetData() );						// The actual image data itself
 	}
  	else // If we failed to open the image file in the first place...
  	{
		error = ilGetError();
		//cout << "Image load failed - IL reports error: " << error << endl;
		MessageBoxA( NULL, "We failed to open the image file in the first place", NULL, NULL );
		//exit( -1 );
		return false;
  	}

	strcpy_s( texture->fileName, sizeof( char* ), theFileName );
	texture->width = (int)( ilGetInteger( IL_IMAGE_WIDTH ) );
	texture->height = (int)( ilGetInteger( IL_IMAGE_HEIGHT ) );
	texture->ID = textureID;

 	ilDeleteImages( 1, &imageID ); // Because we have already copied image data into texture data we can release memory used by image.

	//cout << "Texture creation successful." << endl;
	MessageBoxA( NULL, "Texture creation successful.", NULL, NULL );

	return true;
}
