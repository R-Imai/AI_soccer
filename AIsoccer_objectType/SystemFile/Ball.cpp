/*--------------------------------------------------------------------------------------------------------------
*	Name:		Ball.cpp
*	Author:		R.Imai
*	Created:	2015 / 09 / 22
*	Last Date:	2015 / 09 / 22
*	Note:		
*
*--------------------------------------------------------------------------------------------------------------*/
#include"AIsoccer.h"

Ball::Ball()
{
}

Ball::~Ball()
{
}

void Ball::update(){
	ball.x = ball.x + ball.vx;
	ball.y = ball.y + ball.vy;
}