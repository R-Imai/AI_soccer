/*--------------------------------------------------------------------------------------------------------------
*	Name:		Ground.cpp
*	Author:		R.Imai
*	Created:	2015 / 09 / 25
*	Last Date:	2015 / 09 / 27
*	Note:
*
*--------------------------------------------------------------------------------------------------------------*/
#include"AIsoccer.h"


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

void Ground::player_wall(){
	double range, ang, theta;
	int judge, cnt = 0, have = 0;//judge判定	cnt内側に入った数
	int n = 1;
	while (n <= PLAYER){
		range = dist(A.player[n].x, A.player[n].y, ball.x, ball.y);
		ang = A.player[n].ang - (atan2(-A.player[n].y + ball.y, -A.player[n].x + ball.x) * 180 / P);
		if (ang > 180){ //角度を-180～180にする
			ang = ang - 360;
		}
		else if (ang < -180){
			ang = ang + 360;
		}
		if (range < 56 && fabs(ang)>30){//ボールの内側に入った時にボールを持つかはじくかの判定
			judge = 1;
			cnt = cnt + 1;
			theta = atan2(A.player[n].y - ball.y, A.player[n].x - ball.x);
			A.player[n].x = (A.player[n].x + ball.x) / 2 + 28 * cos(theta);
			A.player[n].y = (A.player[n].y + ball.y) / 2 + 28 * sin(theta);
			ball.x = (A.player[n].x + ball.x) / 2 - 28 * cos(theta);
			ball.y = (A.player[n].y + ball.y) / 2 - 28 * sin(theta);
		}
		else if (range < 56 && fabs(ang) < 30){
			have = n;
		}

		range = dist(B.player[n].x, B.player[n].y, ball.x, ball.y);//Bチームも同様							
		ang = B.player[n].ang - (atan2(-B.player[n].y + ball.y, -B.player[n].x + ball.x) * 180 / P);
		if (ang > 180){
			ang = ang - 360;
		}
		else if (ang < -180){
			ang = ang + 360;
		}
		if (range < 56 && fabs(ang)>30){
			judge = 1;
			cnt = cnt + 1;
			theta = atan2(B.player[n].y - ball.y, B.player[n].x - ball.x);
			B.player[n].x = (B.player[n].x + ball.x) / 2 + 28 * cos(theta);
			B.player[n].y = (B.player[n].y + ball.y) / 2 + 28 * sin(theta);
			ball.x = (B.player[n].x + ball.x) / 2 - 28 * cos(theta);
			ball.y = (B.player[n].y + ball.y) / 2 - 28 * sin(theta);
		}
		else if (range < 56 && fabs(ang) < 30){
			have = -n;
		}
		n++;
	}


	if (cnt == 0){//だれもボールの内側にいないとき
		judge = 0;
	}
	else if (judge == 1 && col == 0){//初めて正面以外にボールが当たった時
		ball.vx = -0.3*ball.vx;
		ball.vy = -0.3*ball.vy;
		col = 1;
	}
	if (judge == 0){
		col = 0;
	}
	n = 1;
	if (have != 0){//誰かがボールを持ってる
		ball.vx = 0;
		ball.vy = 0;
		while (n <= PLAYER){
			A.player[n].have = 0;
			B.player[n].have = 0;
			n++;
		}
		if (have > 0){//だれが持ってるかの確定(haveが+ならA)
			A.player[have].have = 1;
			ball.have = 1;
		}
		else if (have < 0){//(haveが-ならB)
			B.player[-have].have = 1;
			ball.have = -1;
		}

	}

}

void Ground::wall(){
	if (((ball.x>650 || ball.x<-650 || ball.y>925 || ball.y<-925)) && (ball.x<l_g || ball.x>r_g)){

		if ((ball.x>650 || ball.x < -650) && (ball.y<925 && ball.y>-925)){
			ball.state = ball.have*(-1);
		}
		else if (ball.y < -925){
			if (ball.have>0){
				ball.state = -2;
			}
			else if (ball.have < 0){
				ball.state = 3;
			}
		}
		else if (ball.y > 925){
			if (ball.have>0){
				ball.state = -3;
			}
			else if (ball.have < 0){
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


