/*--------------------------------------------------------------------------------------------------------------
*	Name:		R-Imai.h
*	Author:		R.Imai
*	Created:	2015 / 09 / 27
*	Last Date:	2015 / 09 / 27
*	Note:
*
*--------------------------------------------------------------------------------------------------------------*/
#pragma once
#include"AIsoccerSystem.h"
#define R_ImaiEnemy B

class R_Imai:public Team
{
public:

	R_Imai(int);
	~R_Imai();
	int sw;
	int initChecker;
	void strategy();
	void defence_init();
	void offence_init();
	void lineover_init();
	void click_init();
	void keeper();
	int minplayer();
	void divide(int);
	int getPos(Player,char,double,double);
	int enemyCount(char, double, double);
	bool checkPass(Player, Player);
int div;
private:
	int teamMode[5];
	
};

extern R_Imai A;

