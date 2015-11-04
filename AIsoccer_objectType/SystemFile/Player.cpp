/*--------------------------------------------------------------------------------------------------------------
*	Name:		Player.cpp
*	Author:		R.Imai
*	Created:	2015 / 09 / 21
*	Last Date:	2015 / 11 / 04
*	Note:		side‚ÍU‚ß‚é•û‚ÌŠp“x
*
*--------------------------------------------------------------------------------------------------------------*/

#include"AIsoccer.h"


Player::Player(){
}

Player::~Player(){

}

void Player::init(int side){
	this->side = side;
}

void Player:: set(double X, double Y, double ANG){
	this->x = X;
	this->y = Y;
	this->ang = ANG;
}

void Player::update(){
	this->ang = this->ang + this->vang;
	if (this->ang >= 180){
		this->ang = this->ang - 360;
	}
	else if (this->ang < -180){
		this->ang = this->ang + 360;
	}

	this->vx = this->v*cos(this->ang*(P / 180));
	this->vy = this->v*sin(this->ang*(P / 180));

	this->x = this->x + this->vx;
	this->y = this->y + this->vy;
}


void Player::turn(double ang){
	double ang_path = this->ang - ang;
	if (ang_path > 180){			//Šp“x‚Ì’²®
		ang_path = ang_path - 360;
	}
	else if (ang_path < -180){
		ang_path = ang_path + 360;
	}

	if (fabs(ang_path) >= 0.2){//‚Ç‚Á‚¿ü‚è‚ª‹ß‚¢‚©ŒvZ(‚±‚Ì•Ó‚ğí—ª‚Å•¡G‚É‚·‚é‚È‚çV‚µ‚­ŠÖ”ì‚é)
		this->v = 0;
		if (ang_path<0){		//”½Œv‰ñ‚è

			this->ang = this->ang + 0.1;
			if (this->have == 1){
				ball.x = this->x + 55.5 * cos((this->ang + 0.1)*(P / 180));
				ball.y = this->y + 55.5 * sin((this->ang + 0.1)*(P / 180));
			}
		}
		else{		//Œv‰ñ‚è
			this->ang = this->ang - 0.1;
			if (this->have == 1){
				ball.x = this->x + 55.5 * cos((this->ang - 0.1)*(P / 180));
				ball.y = this->y + 55.5 * sin((this->ang - 0.1)*(P / 180));
			}
		}
	}


	else{
		this->re = 1;
	}
}

void Player::turn(Player waiter){
	 this->turn(atan2(waiter.y - this->y, waiter.x - this->x)*(180 / P));
}

void Player::turn_B(){
	double ang = atan2(ball.y - this->y, ball.x - this->x) * 180 / P;
	if (ang > 180){
		ang = ang - 360;
	}
	else if (ang <= -180){
		ang = ang + 360;
	}
	 this->turn(ang);
}

void Player::turn_G(){
	this->turn(atan2(-this->y + (-this->side) * 905, -this->x)*(180 / P));
}

void Player::go(){
	this->v = 0.1;
	if (this->have == 1){
		ball.x = ball.x + this->v*cos(this->ang*(P / 180));
		ball.y = ball.y + this->v*sin(this->ang*(P / 180));
	}
}

void Player::go_G(double meter){
	switch (this->re){
	case 0:
		this->turn_G();
		if (this->re == 1){
			this->re = 2;
		}
		break;
	case 2:
		if (dist(this->x, this->y, 0, side * 905) > meter){
			this->go();
		}
		else{
			this->re = 1;
		}
		break;
	}
}

void Player::move(double xx, double yy){
	double ang;
	ang = atan2(yy - this->y, xx - this->x)*(180 / P);
	this->turn(ang);
	if (dist(this->x, this->y, xx, yy)>2 && this->re == 1){
		this->go();
		this->re = 0;
	}
	else if (dist(this->x, this->y, xx, yy)<2){
		this->re = 1;
	}
}

void Player::get(){
	if (this->have == 0){
		this->move(ball.x, ball.y);
	}
	else if (this->have == 1){
		this->re = 1;
	}
}

void Player::stop(){
	this->v = 0;
}

void Player::pass(Player getter){
	double pass_ang, ang;
	ang = atan2(getter.y - this->y, getter.x - this->x)*(180 / P);
	pass_ang = ang - this->ang;
	if (this->have == 1){
		if (fabs(pass_ang) < 30){
			ball.x += 0.5*cos(ang*P / 180)*0.5;
			ball.y += 0.5*sin(ang*P / 180)*0.5;
			ball.vx = 0.5*cos(ang*P / 180);
			ball.vy = 0.5*sin(ang*P / 180);
			this->have = 0;
			this->re = 1;
		}
		else{
			this->turn(getter);
		}
	}
}

void Player::shoot(){
	if (this->have = 1){
		this->v = 0;
		this->vang = 0;
		ball.vx = cos(this->ang*P / 180);
		ball.vy = sin(this->ang*P / 180);
		this->re = 1;
		//cout << "shoot()";
	}
}

double Player::calc_passang(Player getter){
	return atan2(getter.y - this->y, getter.x - this->x)*(180 / P);
}