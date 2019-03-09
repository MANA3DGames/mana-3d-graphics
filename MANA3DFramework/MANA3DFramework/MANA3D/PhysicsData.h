#ifndef MANA3D_PHYSICSDATA_H
#define MANA3D_PHYSICSDATA_H

namespace MANA3D
{
	class Vector3;

	/**
	Holds the value for energy under which a body will be put to sleep. This is a global value for the whole solution.  
	By default it is 0.1, which is fine for simulation when gravity is about 20 units per second squared, masses are about one, and
	other forces are around that of gravity. It may need tweaking if your simulation is drastically different to this.
	*/
	extern float sleepEpsilon;

	/**
	Sets the current sleep epsilon value: the kinetic energy under which a body may be put to sleep. Bodies are put to sleep if
	they appear to have a stable kinetic energy less than this value. For simulations that often have low values (such as slow
	moving, or light objects), this may need reducing. The value is global; all bodies will use it.
	*/
	extern void SetSleepEpsilon( float value );

	/**
	Gets the current value of the sleep epsilon parameter.
	*/
	extern float GetSleepEpsilon();


	extern const Vector3 GRAVITY;
	extern const Vector3 HIGH_GRAVITY;
}

#endif