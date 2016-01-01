#include"AIsoccerSystem.h"

class SampleB :public Team
{
public:
	SampleB(int);
	void strategy();
	void defence_init();
	void offence_init();
	void lineover_init();
	void click_init();

	void keeper();
private:

};

extern SampleB B;
