#include "Time.h"
using namespace MANA3D;

#include <cwchar>


float Time::deltaTime = 0.0f;

bool Time::NextFrame( float frameRate )
{
	// Define static values for the elapsed time and the last time.
	static float lastTime = GetTickCount() * 0.001f;
	static float elapsedTime = 0.0f;

	// Get current time in seconds.
	float currentTime = GetTickCount() * 0.001f;
	// Calcualte delta time.
	deltaTime = currentTime - lastTime;
	// Calcuate desired FPS.
	float desiredFPS = 1.0f / frameRate;

	// Add deltaTime to the elapsedTime.
	elapsedTime += deltaTime;
	// Set the lastTime to the currentTime (Update it for the next frame).
	lastTime = currentTime;

	// Check if the time (elapsedTime) since we last checked is greater than our desiredFPS.
	if ( elapsedTime > desiredFPS )
	{
		// Set the delta time
		deltaTime = desiredFPS;

		// Reset the elapsedTime.
		elapsedTime -= desiredFPS;

		// Proceed the next frame.
		return true;
	}

	// Don't proceed the next frame.
	return false;
}

void Time::CalculateFrameRate( HWND hWnd )
{
	static int framesPerSecond = 0;				// This will store our fps
	static float fpsTime = 0.0f;				// Amount of elapsed time until we update the FPS count
	//char strFrameRate[50] = { 0 };				// We will store the string here for the window title

	// Increase the fps elapsed time
	// deltaTime is the delta time, that is, the time between the last frame and the current frame
	fpsTime += deltaTime;

	// Check to see if our time is greater than 1 second, if it is, then
	// record the frames per second.
	if ( fpsTime > 1.0f )
	{
		// Reset the fpsTime
		fpsTime = 0.0f;
		
		// Copy the frames per second into a string to display in the window title bar
		//sprintf_s( strFrameRate, "Current Frames Per Second: %i", framesPerSecond );

		wchar_t buffer[256];
		std::swprintf( buffer, sizeof(buffer) / sizeof(*buffer), L"FPS: %i", framesPerSecond );
		LPCWSTR pStr = buffer;

		// Set the window title bar to our string
		SetWindowText( hWnd, pStr );

		// Reset the frames per second
		framesPerSecond = 0;
	}
	else
	{
		// Increase the frame counter
		++framesPerSecond;
	}
}


float Time::GetDeltaTime()
{
	return deltaTime;
}