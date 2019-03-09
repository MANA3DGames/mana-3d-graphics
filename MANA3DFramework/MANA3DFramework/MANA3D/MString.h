#ifndef MANA3D_MSTRING_H
#define MANA3D_MSTRING_H

#include <windows.h>
#include <cwchar>
#include <math.h>
#include <comdef.h>
#include <string>
using std::string;


namespace MANA3D
{
	class Vector3;

	class MString
	{
	private:
		string str;

	public:
		MString();
		MString( string str );
		MString( char *str );
		MString( const char *str );
		MString( const wchar_t* );

		~MString();

		MString operator+( const char *str ) const;
		MString operator+=( const char *str );
		MString operator+( string str ) const;
		MString operator+=( string str );
		MString operator+( MString str ) const;
		MString operator+=( MString str );
		MString operator+( int value ) const;
		MString operator+=( int value );
		MString operator+( float value ) const;
		MString operator+=( float value );
		MString operator+( Vector3 vec ) const;
		MString operator+=( Vector3 vec );

		/*template<typename T>
		MString operator+( T t ) const
		{
			MString temp;
			return temp + t;
		}*/

		operator const char*() const;
		operator LPWSTR() const;
		operator const wchar_t*() const;


		const char* GetCStr();

		char* ToLower();

		bool IsEmpty();

		void Clear();


		template <typename T>
		void Append( T t )
		{
			/*char buffer[256];
			if ( std::is_same<T, float>::value )
			{
				sprintf_s( buffer, "%s%f", (float)t );
			}
			else if ( std::is_same<T, int>::value )
			{
				sprintf_s( buffer, "%s%i", str, (int)t );
			}
			else
			{
				str.append( t );
				return;
			}

			str.append( buffer );*/
			str.append( t );
		}


	public:
		static LPWSTR ToLPWSTR( const char* str );
		static const wchar_t* ToWChar_T( const char *str );
		static char* ToCharPtr( const wchar_t* wChar );
		static char* ToLower( const char* str );
	};
}

#endif
