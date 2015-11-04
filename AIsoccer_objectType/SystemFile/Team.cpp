/*--------------------------------------------------------------------------------------------------------------
*	Name:		Team.cpp
*	Author:		R.Imai
*	Created:	2015 / 09 / 27
*	Last Date:	2015 / 09 / 27
*	Note:
*
*--------------------------------------------------------------------------------------------------------------*/
#include"AIsoccer.h"


Team::Team(int side)
{
	this->side = side;
	for (int i = 1; i <= PLAYER; i++){
		this->player[i].init(side);
	}
}

Team::~Team()
{
}

void Team::stop(){
	for (int i = 1; i <= PLAYER; i++){
		this->player[i].stop();
	}
}

void Team::update(){
	for (int i = 1; i <= PLAYER; i++){
		this->player[i].update();
	}
}

int Team::teamState(int state){
	return this->side*state;
}

