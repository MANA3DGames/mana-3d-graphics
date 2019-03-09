#define _CRT_SECURE_NO_WARNINGS
#include "MString.h"
#include "Vector3.h"
using namespace MANA3D;


MString::MString()
{

}

MString::MString( string gStr )
{
	str = gStr;
}

MString::MString( char *gStr )
{
	str.append( gStr );
}

MString::MString( const char *gStr )
{
	str.append( gStr );
}

MString::MString( const wchar_t *gStr )
{
	_bstr_t newStr( gStr );
	str.append( (char*)newStr );
}


MString::~MString()
{

}


MString MString::operator+( const char *cChr ) const
{
	MString temp( str );
	temp.Append( cChr );
	return MString( temp );
}

MString MString::operator+=( const char *cChr )
{
	str.append( cChr );
	return *this;
}

MString MString::operator+( string sStr ) const
{
	MString temp( str );
	temp.Append( sStr );
	return MString( temp );
}

MString MString::operator+=( string sStr )
{
	str.append( sStr );
	return *this;
}

MString MString::operator+( MString sStr ) const
{
	MString temp( str );
	temp.Append( sStr );
	return MString( temp );
}

MString MString::operator+=( MString sStr )
{
	str.append( sStr );
	return *this;
}

MString MString::operator+( int value ) const
{
	MString temp( str );
	char buffer[256];
	sprintf_s( buffer, sizeof(buffer), "%i", value );
	temp.Append( buffer );
	return temp;
}

MString MString::operator+=( int value )
{
	char buffer[256];
	sprintf_s( buffer, sizeof(buffer), "%i", value );
	str.append( buffer );
	return *this;
}

MString MString::operator+( float value ) const
{
	MString temp( str );
	char buffer[256];
	sprintf_s( buffer, sizeof(buffer), "%f", value );
	temp.Append( buffer );
	return temp;
}

MString MString::operator+=( float value )
{
	char buffer[256];
	sprintf_s( buffer, sizeof(buffer), "%f", value );
	str.append( buffer );
	return *this;
}

MString MString::operator+( Vector3 vec ) const
{
	MString temp( str );
	char buffer[256];
	sprintf_s( buffer, sizeof(buffer), "(%f, %f, %f)", vec.x, vec.y, vec.z );
	temp.Append( buffer );
	return temp;
}

MString MString::operator+=( Vector3 vec )
{
	char buffer[256];
	sprintf_s( buffer, sizeof(buffer), "(%f, %f, %f)", vec.x, vec.y, vec.z );
	str.append( buffer );
	return *this;
}


MString::operator const char*() const
{
	return str.c_str();
}

MString::operator LPWSTR() const
{
	return ToLPWSTR( str.c_str() );
}

MString::operator const wchar_t*() const
{
	return ToWChar_T( str.c_str() );
}


const char* MString::GetCStr()
{
	return str.c_str();
}

char* MString::ToLower()
{
	return ToLower( str.c_str() );
}

bool MString::IsEmpty()
{
	return str.empty();
}

void MString::Clear()
{
	str.clear();
}



LPWSTR MString::ToLPWSTR( const char* gStr )
{
	size_t size = strlen( gStr ) + 1;
	wchar_t* wChar = new wchar_t[size];
	size_t outSize;
	mbstowcs_s( &outSize, wChar, size, gStr, size - 1 );
	LPWSTR ptr = wChar;
	return ptr;
}

const wchar_t* MString::ToWChar_T( const char *cStr )
{
	const size_t cSize = strlen( cStr ) + 1;
	wchar_t wc[256];
	mbstowcs( wc, cStr, cSize );
	return wc;
}

char* MString::ToCharPtr( const wchar_t* wChar )
{
	_bstr_t newStr( wChar );
	return (char*)newStr;
}

char* MString::ToLower( const char* cStr )
{
	char* temp = _strdup(cStr);
	for (int i = 0; temp[i] != '\0'; i++)
		temp[i] = tolower(temp[i]);
	return temp;
}