/*--------------------------------------------------------------------------------------------------------------
*	Name:		AIsoccer.h																						
*	Author:		R.Imai																							
*	Created:	2015 / 09 / 21																					
*	Last Date:	2015 / 11 / 04																					
*	Note:																										
*																												
*--------------------------------------------------------------------------------------------------------------*/
#pragma once
#include<iostream>
#include<math.h>
#include<stdlib.h>
#include <GL/glut.h>
#define P 3.141592
#define PLAYER 4	/*プレーヤー数*/

#define g_R 0.4		/*************/
#define g_G 0.8		/*グランドRGB*/
#define g_B 0.4		/*************/

#define l_g -160	/************/
#define r_g 160		/*ゴール	*/
#define A_g 905		/*		位置*/		
#define B_g -905	/************/

#define RANGE 300

#define X_MIN -625	/************/
#define Y_MIN -905	/*グランド	*/
#define X_MAX 625	/*		端点*/
#define Y_MAX 905	/************/

using namespace std;

extern int score_A, score_B;
//extern int goalflag;
//extern int col;


class Player
{
public:
	int have;
	double x, y, ang;
	double v,vx, vy, vang;
	int mode, cnd, re;
	int side;
	Player();
	~Player();

	void init(int);
	void set(double, double, double);
	void update();

	void turn(double);
	void turn(Player);
	void turn_B();
	void turn_G();
	void go();
	void go_G(double);
	void move(double, double);
	void get();
	void stop();
	void pass(Player);
	void shoot();
	double calc_passang(Player);
private:
	

};

class Team
{
public:
	Team(int);
	~Team();
	virtual void strategy()=0;
	virtual void defence_init()=0;
	virtual void offence_init()=0;
	virtual void lineover_init()=0;

	Player player[5];
	int side;
	int teamStrategy;
	void update();
	void stop();
	int teamState(int);
private:
	

};

class Ball
{
public:
	Ball();
	~Ball();
	double x, y;
	double v, vx, vy;
	int have, state, judge;

	void update();
	void speed(double,double);
	void turn();
private:
	
	

};

class Ground
{
public:
	Ground();
	~Ground();

	
	int goalflag;
	int col;

	void wall();
	void goal();
	void crash();
	void player_wall();

private:
	bool hitCheck(Player);
	bool haveCheck(Player);

};

class Draw
{
public:
	Draw();
	~Draw();

	void circle(double, double, double);
	void player(Player,double,double,double);
	void Ball();

private:

};



double dist(double x1, double y1, double x2, double y2);





extern Ball ball;
