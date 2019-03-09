#ifndef MANA3D_DEBUG_H
#define MANA3D_DEBUG_H
		
#include "MString.h"

namespace MANA3D
{
	class Win32App;
	class Debug
	{
		friend Win32App;

	public:
		static void Enable();
		static void Disable();

		template <typename T>
		static void Log( T t )
		{
			// First make sure debugging is enabled.
			if ( !IsEnabled ) return;

			// Start a new line if the string is not empty already.
			if ( !mStr.IsEmpty() )
				mStr.Append( "\n" );

			// Add new entry to the static mString.
			mStr += t;

			// Display debug window.
			ShowDebug();
		}

	private:
		Debug() {}											// Private constructor.
		~Debug() {}											// Private destructor.

		static void Init();
		static void Update();
		static void ShowDebug();
		static void HideDebug();


	private:
		static bool IsEnabled;								// Is debugging enabled
		static bool IsDisplayed;							// Is console (window) is shown.
		static MString mStr;								// Static string for all logs.
		static HWND hWndDebug;								// Handle for the console window. 		

		static bool keyIsNotUp;
	};
}

#endif