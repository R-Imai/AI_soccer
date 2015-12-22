/*--------------------------------------------------------------------------------------------------------------
*	Name:		Ground.cpp
*	Author:		R.Imai
*	Created:	2015 / 09 / 25
*	Last Date:	2015 / 11 / 04
*	Note:
*
*--------------------------------------------------------------------------------------------------------------*/
#include"AIsoccer.h"


bool inFrag = true;
Ground::Ground()
{
}

Ground::~Ground()
{
	goalflag = 0;
	col = 0;

}

void Ground::crash(){
	double theta;
	for (int n = 1; n < PLAYER; n++){
		if (dist(A.player[n].x, A.player[n].y, B.player[n].x, B.player[n].y) < 80){
			theta = atan2(A.player[n].y - B.player[n].y, A.player[n].x - B.player[n].x);
			A.player[n].x = (A.player[n].x + B.player[n].x) / 2 + 40 * cos(theta);
			A.player[n].y = (A.player[n].y + B.player[n].y) / 2 + 40 * sin(theta);
			B.player[n].x = (A.player[n].x + B.player[n].x) / 2 - 40 * cos(theta);
			B.player[n].y = (A.player[n].y + B.player[n].y) / 2 - 40 * sin(theta);
		}
		for (int i = n + 1; i < PLAYER + 1; i++){
			if (dist(A.player[n].x, A.player[n].y, A.player[i].x, A.player[i].y) < 80){
				theta = atan2(A.player[n].y - A.player[i].y, A.player[n].x - A.player[i].x);
				A.player[n].x = (A.player[n].x + A.player[i].x) / 2 + 40 * cos(theta);
				A.player[n].y = (A.player[n].y + A.player[i].y) / 2 + 40 * sin(theta);
				A.player[i].x = (A.player[n].x + A.player[i].x) / 2 - 40 * cos(theta);
				A.player[i].y = (A.player[n].y + A.player[i].y) / 2 - 40 * sin(theta);
			}
			if (dist(A.player[n].x, A.player[n].y, B.player[i].x, B.player[i].y) < 80){
				theta = atan2(A.player[n].y - B.player[i].y, A.player[n].x - B.player[i].x);
				A.player[n].x = (A.player[n].x + B.player[i].x) / 2 + 40 * cos(theta);
				A.player[n].y = (A.player[n].y + B.player[i].y) / 2 + 40 * sin(theta);
				B.player[i].x = (A.player[n].x + B.player[i].x) / 2 - 40 * cos(theta);
				B.player[i].y = (A.player[n].y + B.player[i].y) / 2 - 40 * sin(theta);
			}
			if (dist(A.player[i].x, A.player[i].y, B.player[n].x, B.player[n].y) < 80){
				theta = atan2(A.player[i].y - B.player[n].y, A.player[i].x - B.player[n].x);
				A.player[i].x = (A.player[i].x + B.player[n].x) / 2 + 40 * cos(theta);
				A.player[i].y = (A.player[i].y + B.player[n].y) / 2 + 40 * sin(theta);
				B.player[n].x = (A.player[i].x + B.player[n].x) / 2 - 40 * cos(theta);
				B.player[n].y = (A.player[i].y + B.player[n].y) / 2 - 40 * sin(theta);
			}
			if (dist(B.player[n].x, B.player[n].y, B.player[i].x, B.player[i].y) < 80){
				theta = atan2(B.player[n].y - B.player[i].y, B.player[n].x - B.player[i].x);
				B.player[n].x = (B.player[n].x + B.player[i].x) / 2 + 40 * cos(theta);
				B.player[n].y = (B.player[n].y + B.player[i].y) / 2 + 40 * sin(theta);
				B.player[i].x = (B.player[n].x + B.player[i].x) / 2 - 40 * cos(theta);
				B.player[i].y = (B.player[n].y + B.player[i].y) / 2 - 40 * sin(theta);
			}
		}
	}
	if (dist(A.player[PLAYER].x, A.player[PLAYER].y, B.player[PLAYER].x, B.player[PLAYER].y) < 80){
		theta = atan2(A.player[PLAYER].y - B.player[PLAYER].y, A.player[PLAYER].x - B.player[PLAYER].x);
		A.player[PLAYER].x = (A.player[PLAYER].x + B.player[PLAYER].x) / 2 + 40 * cos(theta);
		A.player[PLAYER].y = (A.player[PLAYER].y + B.player[PLAYER].y) / 2 + 40 * sin(theta);
		B.player[PLAYER].x = (A.player[PLAYER].x + B.player[PLAYER].x) / 2 - 40 * cos(theta);
		B.player[PLAYER].y = (A.player[PLAYER].y + B.player[PLAYER].y) / 2 - 40 * sin(theta);
	}
}

void Ground::goal(){
	if (l_g < ball.x && ball.x < r_g && ball.y > A_g){
		A.offence_init();
		B.defence_init();
		goalflag = 1;
		score_B++;
	}
	else if (l_g < ball.x&&ball.x < r_g&&ball.y < B_g){
		A.defence_init();
		B.offence_init();
		goalflag = -1;
		score_A++;
	}
	
	ball.vx = 0;
	ball.vy = 0;
	ball.x = 0;
	ball.y = 0;
	goalflag = 0;
	cout << "A:" << score_A << "-" << score_B << ":B\n";
	glutIdleFunc(NULL);
}

bool Ground::hitCheck(Player play){
	bool check = false;

	if (dist(play.x, play.y, ball.x, ball.y) <= 56){
		check = true;
	}
	return check;
}

bool Ground::haveCheck(Player play){
	bool check = false;
	double ang = play.ang - (atan2(-play.y + ball.y, -play.x + ball.x) * 180 / P);
	//cout << play.ang <<" : "<< atan2(-play.y + ball.y, -play.x + ball.x) * 180 / P << "\n";
	if (ang < -180){
		ang += 360;
	}
	else if (ang > 180){
		ang -= 360;
	}

	if (fabs(ang) < 30){
		check = true; 
	}
	return check;
}


void Ground::player_wall(){
	int inCount = 0;
	int haveCount = 0;
	int havePlayer[8],inPlayer[8];
	double minDist;
	double ballRange;
	int posHave;
	double PtoB, BtoS, StoP;
	double theta;
	double wasVx = ball.vx, wasVy = ball.vy;

	Player player;
	
	for (int i = 1; i <= PLAYER; i++){
		A.player[i].have = 0;
		B.player[i].have = 0;
	}
	for (int i = 1; i <= PLAYER; i++){
		if (hitCheck(A.player[i])){
			if (haveCheck(A.player[i])){
				havePlayer[haveCount] = i;
				haveCount++;
			}
			else{
				inPlayer[inCount] = i;
				inCount++;
			}
		}
		if (hitCheck(B.player[i])){
			if (haveCheck(B.player[i])){
				havePlayer[haveCount] = -i;
				haveCount++;
			}
			else{
				inPlayer[inCount] = -i;
				inCount++;
			}
		}
	}
	//cout << "inCount = " << inCount << "	haveCount = " << haveCount<<"\n";
	if (haveCount >= 1){
		if (haveCount == 1){
			if (havePlayer[0] > 0){
				A.player[havePlayer[0]].have = 1;
				ball.have = havePlayer[0];
				ball.judge = 1;
				ball.vx = 0;
				ball.vy = 0;
			}
			else{
				B.player[-havePlayer[0]].have = 1;
				ball.have = havePlayer[0];
				ball.judge = -1;
				ball.state = -1;
				ball.vx = 0;
				ball.vy = 0;
			}
		}
		else{
			if (havePlayer[0] > 0){
				player = A.player[havePlayer[0]];
			}
			else{
				player = B.player[-havePlayer[0]];
			}
			minDist = dist(player.x, player.y, ball.x, ball.y);
			posHave = 0;
			for (int i = 1; i <= haveCount; i++){
				if (havePlayer[i] > 0){
					player = A.player[havePlayer[i]];
				}
				else{
					player = B.player[-havePlayer[i]];
				}
				if (minDist > dist(player.x, player.y, ball.x, ball.y)){
					minDist = dist(player.x, player.y, ball.x, ball.y);
					posHave = i;
				}
			}
			if (havePlayer[posHave] > 0){
				A.player[havePlayer[posHave]].have = 1;
				ball.have = 1;
				ball.judge = 1;
				ball.vx = 0;
				ball.vy = 0;
			}
			else{
				B.player[-havePlayer[posHave]].have = 1;
				ball.have = -1;
				ball.judge = -1;
				ball.vx = 0;
				ball.vy = 0;
			}

		}
	}
	else if(inCount >= 1){
		if (inCount == 1 && inFrag){
			if (inPlayer[0] > 0){
				player = A.player[inPlayer[0]];
				ball.judge = 1;
				cout << "A touch \n";
			}
			else{
				player = B.player[-inPlayer[0]];
				ball.judge = -1;
				cout << "B touch \n";
			}
			PtoB = dist(player.x, player.y, ball.x, ball.y);
			StoP = dist(ball.x, ball.y, ball.x + 500 * ball.vx, ball.y + 500 * ball.vy);//sqrt(500*ball.vx*500*ball.vx + 500*ball.vy*500*ball.vy);
			BtoS = dist(ball.x + 500 * ball.vx, ball.y + 500 * ball.vy, player.x, player.y);
			theta = fabs(P - 2 * acos((BtoS*BtoS + PtoB*PtoB - StoP*StoP) / (2 * BtoS*PtoB)));
			if ((ball.y - player.y) / (ball.x - player.x) > ball.vy / ball.vx){
				theta = -theta;
			}
			
			ball.vx = 0.3*(wasVx*cos(theta)-wasVy*sin(theta));
			ball.vy = 0.3*(wasVx*sin(theta)+wasVy*cos(theta));
			inFrag = false;
		}
		//ball.vx = -ball.vx;
		//ball.vy = -ball.vy;
	}
	if (inCount == 0){
		inFrag = true;
	}

}

void Ground::wall(){
	if (((ball.x>650 || ball.x<-650 || ball.y>925 || ball.y<-925)) && (ball.x<l_g || ball.x>r_g)){

		if ((ball.x>650 || ball.x < -650) && (ball.y<925 && ball.y>-925)){
			ball.state = -ball.judge;
		}
		else if (ball.y < -925){
			if (ball.judge > 0){
				ball.state = -2;
			}
			else if (ball.judge < 0){
				ball.state = 3;
			}
		}
		else if (ball.y > 925){
			if (ball.judge > 0){
				ball.state = -3;
			}
			else if (ball.judge < 0){
				ball.state = 2;
			}
		}
		ball.vx = 0;
		ball.vy = 0;
		A.lineover_init();
		B.lineover_init();

	}
	
	if ((ball.y > 940 || ball.y < -940) && (ball.x<r_g || ball.x > l_g)){
		goal();
		/*if (goalflag == 1){
			score_B++;
		}
		else if (goalflag == -1){
			score_A++;
		}
		ball.vx = 0;
		ball.vy = 0;
		ball.x = 0;
		ball.y = 0;
		goalflag = 0;
		cout << "A:" << score_A << "-" << score_B << ":B\n";
		glutIdleFunc(NULL);*/
	}
	
}


