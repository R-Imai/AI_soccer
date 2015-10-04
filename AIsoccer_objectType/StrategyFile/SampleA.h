/*--------------------------------------------------------------------------------------------------------------
*	Name:		SampleA.h
*	Author:		R.Imai
*	Created:	2015 / 09 / 27
*	Last Date:	2015 / 09 / 27
*	Note:
*
*--------------------------------------------------------------------------------------------------------------*/
#pragma once
#include"AIsoccerSystem.h"
#define AteamEnemy B

class SampleA::public Team
{
public:
	int sw = 1;

	SampleA(int);
	~SampleA();

	void strategy();
	void defence_init();
	void offence_init();
	void lineover_init();
	void keeper();
	int minplayer();
	void divide();
	int getPos(Player,char,double,double);
	int enemyCount(char, double, double);
int div;
private:
	int teamMode[5];
	
};

extern SampleA A;

