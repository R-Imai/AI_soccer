/*--------------------------------------------------------------------------------------------------------------
*	Name:		SampleB.h
*	Author:		R.Imai
*	Created:	2015 / 09 / 27
*	Last Date:	2015 / 09 / 27
*	Note:
*
*--------------------------------------------------------------------------------------------------------------*/

#include"AIsoccerSystem.h"

class SampleB :public Team
{
public:
	SampleB(int);
	void strategy();
	void defence_init();
	void offence_init();
	void lineover_init();

	void keeper();
private:

};

extern SampleB B;
