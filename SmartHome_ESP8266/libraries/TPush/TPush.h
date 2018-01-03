/* Rev 1.5 25/11/2015
 * Created by TheTrigger
 * overpowered.it */


/* * * CHANGE LOG * * * * * * * * * *
 * Improved DoubleClick Func		*
 * Added auto-pullup support		*
 * Improved ::Keep					*
 * Reduced memory usage				*
 * Bug Fixes						*
 * Reworked Keep function			*
 * Removed KeepOn function			*
 		(Keep replaces it)
 *	   * * * * * * * * * * * *		*/

#ifndef TPush_H
#define TPush_H

#include "Arduino.h"
#define iSize 2
#define bDebug false
#define AutoPullUp true


class TPush {

	unsigned long iWait[iSize];
	unsigned long iClick[iSize];
	unsigned long iDouble[iSize];
	unsigned long DoubleState[iSize];
	unsigned long iPressed[iSize];
	unsigned long PressedTime[iSize];

	#if bDebug == true
		unsigned long iKeepON[iSize];
	#endif

	bool bClickState[iSize];
	bool bPressed[iSize];
	bool ActiveState = LOW;

	int BPIN = 0;

	public:
		TPush();
		~TPush();

		void setUp(int i, bool s);

		bool Wait(int ms, int i=0);

		bool Click(int ms = 10, int i=0);

		bool DoubleClick(int first=100, int second=100, int i=0, int sep=300);

		int Keep(int minTime=50, int i=0, bool b = false);
		int Keep(int minTime, bool b);

	#if bDebug == true
		bool KeepON(int time=50, int i=0);
	#endif
};

#endif