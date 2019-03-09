#ifndef MANA3D_TIME_H
#define MANA3D_TIME_H

#include <windows.h>

namespace MANA3D
{
	class Win32App;
	class Time
	{
		friend Win32App;

	private:
		static float deltaTime;

	private:
		Time();
		~Time();

		static bool NextFrame( float );
		static void CalculateFrameRate( HWND );

	public:
		static float GetDeltaTime();

	};
	
}

#endif
