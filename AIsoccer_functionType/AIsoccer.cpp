/*--------------------------------------------------------------------------------------------------------------
*	Name:		AIsoccer.cpp
*	Author:		R.Imai
*	Created:	2015 / 02 / 01
*	Last Date:	2015 / 09 / 27
*	Note:
*
*--------------------------------------------------------------------------------------------------------------*/



#include<stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#define P 3.141592
#define PLAYER 4
#define g_R 0.4		//
#define g_G 0.8		//
#define g_B 0.4		//グランドRGB
#define l_g -160	//
#define r_g 160		//
#define A_g 905		//
#define B_g -905	//ゴール位置
#define RANGE 300
#define X_MIN -625
#define Y_MIN -905
#define X_MAX 625
#define Y_MAX 905

#define PLAY_MODE 0	//ここが、0:Bチームはsampleモード,1:BチームはBモード


struct P_data{			//プレイヤーデータ構造体
	double X, Y, ang, vx, vy, vang, v;
	int mode, cnd, have, re;
	char team;
	int debug;
};

struct B_data{			//ボールデータ構造体
	double X, Y, vx, vy;
	int have, state;//haveが+→A,-→B
};

double  x, y;
int Atean_mode[11];
int Ateam_strategy, Bteam_strategy;
int n, A_div = 0;
int col = 0;
int game = 0;
int goalflag;
int score_A = 0, score_B = 0;
int sw = 1;

P_data A[5], B[5];
B_data ball;

void circle(double x, double y, double r)//円を描く関数(x,y:中心座標,r:半径)
{
	double s = P / 18;
	int i;
	i = 0;
	glBegin(GL_POLYGON);
	while (i < 35){
		glVertex2d(x + r*cos(s*i), y + r*sin(s*i));
		i = i + 1;
	}
	glEnd();
}

void player(void)//人を描く関数
{
	double s = P / 18;
	int i;
	i = 0;
	glBegin(GL_POLYGON);
	while (i < 31){
		glVertex2d(40 * cos(s*(i + 3)), 40 * sin(s*(i + 3)));
		i = i + 1;
	}
	glEnd();
}

void player_set(void){
	for (int n = 0; n < PLAYER; n++){
		A[n].team = 'A';
		B[n].team = 'B';
	}
}



double dist(double x1, double y1, double x2, double y2){//(x1,y1)と(x2,y2)二点間の距離を測る関数
	double re;
	re = (x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2);
	re = pow(re, 0.5);
	return re;
}

void player_wall(void){//人にボールが当たったかの関数
	double range, ang, theta;
	int judge, cnt = 0, have = 0;//judge判定	cnt内側に入った数
	n = 1;
	while (n <= PLAYER){
		range = dist(A[n].X, A[n].Y, ball.X, ball.Y);
		ang = A[n].ang - (atan2(-A[n].Y + ball.Y, -A[n].X + ball.X) * 180 / P);
		if (ang > 180){ //角度を-180〜180にする
			ang = ang - 360;
		}
		else if (ang < -180){
			ang = ang + 360;
		}
		if (range < 56 && fabs(ang)>30){//ボールの内側に入った時にボールを持つかはじくかの判定
			judge = 1;
			cnt = cnt + 1;
			theta = atan2(A[n].Y - ball.Y, A[n].X - ball.X);
			A[n].X = (A[n].X + ball.X) / 2 + 28 * cos(theta);
			A[n].Y = (A[n].Y + ball.Y) / 2 + 28 * sin(theta);
			ball.X = (A[n].X + ball.X) / 2 - 28 * cos(theta);
			ball.Y = (A[n].Y + ball.Y) / 2 - 28 * sin(theta);
		}
		else if (range < 56 && fabs(ang) < 30){
			have = n;
		}

		range = dist(B[n].X, B[n].Y, ball.X, ball.Y);//Bチームも同様							
		ang = B[n].ang - (atan2(-B[n].Y + ball.Y, -B[n].X + ball.X) * 180 / P);
		if (ang > 180){
			ang = ang - 360;
		}
		else if (ang < -180){
			ang = ang + 360;
		}
		if (range < 56 && fabs(ang)>30){
			judge = 1;
			cnt = cnt + 1;
			theta = atan2(B[n].Y - ball.Y, B[n].X - ball.X);
			B[n].X = (B[n].X + ball.X) / 2 + 28 * cos(theta);
			B[n].Y = (B[n].Y + ball.Y) / 2 + 28 * sin(theta);
			ball.X = (B[n].X + ball.X) / 2 - 28 * cos(theta);
			ball.Y = (B[n].Y + ball.Y) / 2 - 28 * sin(theta);
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
			A[n].have = 0;
			B[n].have = 0;
			n++;
		}
		if (have > 0){//だれが持ってるかの確定(haveが+ならA)
			A[have].have = 1;
			ball.have = 1;
		}
		else if (have < 0){//(haveが-ならB)
			B[-have].have = 1;
			ball.have = -1;
		}

	}


}



void player_clash(void){
	double theta;
	for (int n = 1; n < PLAYER; n++){
		if (dist(A[n].X, A[n].Y, B[n].X, B[n].Y) < 80){
			theta = atan2(A[n].Y - B[n].Y, A[n].X - B[n].X);
			A[n].X = (A[n].X + B[n].X) / 2 + 40 * cos(theta);
			A[n].Y = (A[n].Y + B[n].Y) / 2 + 40 * sin(theta);
			B[n].X = (A[n].X + B[n].X) / 2 - 40 * cos(theta);
			B[n].Y = (A[n].Y + B[n].Y) / 2 - 40 * sin(theta);
		}
		for (int i = n + 1; i < PLAYER + 1; i++){
			if (dist(A[n].X, A[n].Y, A[i].X, A[i].Y) < 80){
				theta = atan2(A[n].Y - A[i].Y, A[n].X - A[i].X);
				A[n].X = (A[n].X + A[i].X) / 2 + 40 * cos(theta);
				A[n].Y = (A[n].Y + A[i].Y) / 2 + 40 * sin(theta);
				A[i].X = (A[n].X + A[i].X) / 2 - 40 * cos(theta);
				A[i].Y = (A[n].Y + A[i].Y) / 2 - 40 * sin(theta);
			}
			if (dist(A[n].X, A[n].Y, B[i].X, B[i].Y) < 80){
				theta = atan2(A[n].Y - B[i].Y, A[n].X - B[i].X);
				A[n].X = (A[n].X + B[i].X) / 2 + 40 * cos(theta);
				A[n].Y = (A[n].Y + B[i].Y) / 2 + 40 * sin(theta);
				B[i].X = (A[n].X + B[i].X) / 2 - 40 * cos(theta);
				B[i].Y = (A[n].Y + B[i].Y) / 2 - 40 * sin(theta);
			}
			if (dist(A[i].X, A[i].Y, B[n].X, B[n].Y) < 80){
				theta = atan2(A[i].Y - B[n].Y, A[i].X - B[n].X);
				A[i].X = (A[i].X + B[n].X) / 2 + 40 * cos(theta);
				A[i].Y = (A[i].Y + B[n].Y) / 2 + 40 * sin(theta);
				B[n].X = (A[i].X + B[n].X) / 2 - 40 * cos(theta);
				B[n].Y = (A[i].Y + B[n].Y) / 2 - 40 * sin(theta);
			}
			if (dist(B[n].X, B[n].Y, B[i].X, B[i].Y) < 80){
				theta = atan2(B[n].Y - B[i].Y, B[n].X - B[i].X);
				B[n].X = (B[n].X + B[i].X) / 2 + 40 * cos(theta);
				B[n].Y = (B[n].Y + B[i].Y) / 2 + 40 * sin(theta);
				B[i].X = (B[n].X + B[i].X) / 2 - 40 * cos(theta);
				B[i].Y = (B[n].Y + B[i].Y) / 2 - 40 * sin(theta);
			}
		}
	}
	if (dist(A[PLAYER].X, A[PLAYER].Y, B[PLAYER].X, B[PLAYER].Y) < 80){
		theta = atan2(A[PLAYER].Y - B[PLAYER].Y, A[PLAYER].X - B[PLAYER].X);
		A[PLAYER].X = (A[PLAYER].X + B[PLAYER].X) / 2 + 40 * cos(theta);
		A[PLAYER].Y = (A[PLAYER].Y + B[PLAYER].Y) / 2 + 40 * sin(theta);
		B[PLAYER].X = (A[PLAYER].X + B[PLAYER].X) / 2 - 40 * cos(theta);
		B[PLAYER].Y = (A[PLAYER].Y + B[PLAYER].Y) / 2 - 40 * sin(theta);
	}
	printf("");
}

void debuger(int com){//デバッグ用のプリント関数　1:位置	2:mode	3:ang	4:cnd
	switch (com){
	case 1:
		printf("A[2]:%3.0f,%3.0f	A[4]:%3.0f,%3.0f	A[3]:%3.0f,%3.0f	A[1]:%3.0f,%3.0f\n", A[2].X, A[2].Y, A[4].X, A[4].Y, A[3].X, A[3].Y, A[1].X, A[1].Y);
		break;
	case 2:
		printf("A[2]:%d	A[4]:%d	A[3]:%d	A[1]:%d\n", A[2].mode, A[4].mode, A[3].mode, A[1].mode);
		break;
	case 3:
		printf("A[2]:%3.1f	A[4]:%3.1f	A[3]:%3.1f	A[1]:%3.1f\n", A[2].ang, A[4].ang, A[3].ang, A[1].ang);
		break;
	case 4:
		printf("A[2]:%d	A[4]:%d	A[3]:%d	A[1]:%d\n", A[2].cnd, A[4].cnd, A[3].cnd, A[1].cnd);
		break;

	}

}


P_data turn(P_data play, double ang){//回転関数(引数はdeg)(完了時pray.reが1になる)
	double ang_path = play.ang - ang;
	if (ang_path > 180){			//角度の調整
		ang_path = ang_path - 360;
	}
	else if (ang_path < -180){
		ang_path = ang_path + 360;
	}

	if (fabs(ang_path) >= 0.2){//どっち周りが近いか計算(この辺を戦略で複雑にするなら新しく関数作る)
		play.v = 0;
		if (ang_path<0){		//反時計回り

			play.ang = play.ang + 0.1;
			if (play.have == 1){
				ball.X = play.X + 56 * cos((play.ang + 0.1)*(P / 180));
				ball.Y = play.Y + 56 * sin((play.ang + 0.1)*(P / 180));
			}
		}
		else{		//時計回り
			play.ang = play.ang - 0.1;
			if (play.have == 1){
				ball.X = play.X + 56 * cos((play.ang - 0.1)*(P / 180));
				ball.Y = play.Y + 56 * sin((play.ang - 0.1)*(P / 180));
			}
		}
	}


	else{
		play.re = 1;
	}

	return play;
}

struct P_data turn_G(P_data play){//ゴールを向く関数(完了時pray.reが1になる)
	int side;
	double ang;
	if (play.team == 'A'){
		side = -1;
	}
	else if (play.team == 'B'){
		side = 1;
	}
	play = turn(play, atan2(-play.Y + side * 905, -play.X)*(180 / P));
	return play;
}

struct P_data G_go(struct P_data play, int meter){		//ゴールへ進む関数(完了時pray.reが1になる)　meter:ゴールとの距離
	int side;
	if (play.team == 'A'){
		side = -1;
	}
	else if (play.team == 'B'){
		side = 1;
	}
	switch (play.re){
	case 0:
		play = turn_G(play);
		if (play.re == 1){
			play.re = 2;
		}
		break;
	case 2:
		if (dist(play.X, play.Y, 0, side * 905) > meter){
			play.v = 0.1;
			if (play.have == 1){
				ball.X = ball.X + play.v*cos(play.ang*(P / 180));
				ball.Y = ball.Y + play.v*sin(play.ang*(P / 180));
			}
		}
		else{
			play.re = 1;
		}
		break;
	}

	return play;
}



struct P_data go(struct P_data play){//ただ進む関数(指定点はmove(play,xx,yy)を使用)
	play.v = 0.1;
	if (play.have == 1){
		ball.X = ball.X + play.v*cos(play.ang*(P / 180));
		ball.Y = ball.Y + play.v*sin(play.ang*(P / 180));
	}
	return play;
}

struct P_data turn_P(struct P_data turner, struct P_data waiter){//turnerがwaiterを向く関数(完了時pray.reが1になる)

	turner = turn(turner, atan2(waiter.Y - turner.Y, waiter.X - turner.X)*(180 / P));

	return turner;
}

struct P_data move(struct P_data play, double xx, double yy){//playを(xx,yy)へ移動させる関数

	double ang;
	ang = atan2(yy - play.Y, xx - play.X)*(180 / P);
	play = turn(play, ang);
	if (dist(play.X, play.Y, xx, yy)>2 && play.re == 1){
		play = go(play);
		play.re = 0;
	}
	else if (dist(play.X, play.Y, xx, yy)<2){
		play.re = 1;
	}
	return play;
}



struct P_data find(struct P_data play){	//ボール発見関数(不要?)

	double ang;

	ang = atan2(ball.Y - play.Y, ball.X - play.X)*(180 / P) - play.ang;
	if (ang >= 180)
		ang = ang - 360;
	else if (ang<-180)
		ang = ang + 360;
	if (play.ang >= 180)
		play.ang = play.ang - 360;
	else if (play.ang<-180)
		play.ang = play.ang + 360;


	if ((ang>0.1 || ang<-0.1)){
		play.v = 0;
		if (ang>0)
		{
			play.vang = 0.02;
		}
		else if (ang <= 0)
		{
			play.vang = -0.02;
		}
	}
	else{
		play.vang = 0.0;
		play.re = 1;
	}
	return play;

}

int A_check_pass(P_data play, double ang){
	int direc = 0, re = 1;
	double TAN = tan(ang*(P / 180));
	double sec = -TAN*play.X + play.Y;
	double a = 1 + (TAN*TAN), b = -1, c = -play.X*TAN + play.Y, d;
	double range = 80;
	if (fabs(ang) < 90){
		direc = 1;
	}
	else{
		direc = -1;
	}

	for (int i = 1; i <= PLAYER; i++){
		d = fabs(TAN*B[i].X + b*B[i].Y + c) / sqrt(TAN*TAN + b*b);
		//printf("%f\n", d);
		/*b = -2 * B[i].X + 2 * sec*TAN - 2 * B[i].Y*TAN;
		c = B[i].X*B[i].X + sec*sec - 2 * B[i].Y*sec + B[i].Y*B[i].Y - range*range;
		d = b*b - (4 * a*c);
		printf("%f	%f	%f	%f\n",TAN,b,c, d);*/
		if (d <= range && play.X<direc*B[i].X){
			//printf("%f	%f	%f	%f	%f\n",play.X,play.Y,ang,direc*B[i].X,B[i].Y);
			re = 0;
		}
	}
	//printf("\n");
	return re;
}


struct P_data pass(struct P_data passer, struct P_data getter){//passerがgetterへボールをける関数(完了時pray.reが1になる)
	double pass_ang, ang;
	ang = atan2(getter.Y - passer.Y, getter.X - passer.X)*(180 / P);
	pass_ang = ang - passer.ang;
	if (passer.have == 1){
		if (fabs(pass_ang) < 30){
			//printf("%d\n",check_pass(passer,ang));
			ball.vx = 0.5*cos(ang*P / 180);
			ball.vy = 0.5*sin(ang*P / 180);
			passer.have = 0;
			passer.re = 1;
		}
		else{
			passer = turn_P(passer, getter);
		}
	}

	return passer;
}


struct P_data turn_B(struct P_data play){//ボールに向く関数
	double ang = atan2(ball.Y - play.Y, ball.X - play.X) * 180 / P;
	play.debug = 1;
	if (ang > 180){
		ang = ang - 360;
	}
	else if (ang <= -180){
		ang = ang + 360;
	}
	play = turn(play, ang);
	return play;
}


struct P_data get(struct P_data play){//ボールを取りに行く関数(完了時pray.reが1になる)

	if (play.have == 0){
		play = move(play, ball.X, ball.Y);
	}
	else if (play.have == 1){
		play.re = 1;
	}
	return play;
}




struct P_data shoot(struct P_data play){//シュートする関数(ただ思いっきりけるだけ)(完了時pray.reが1になる)
	if (play.have = 1){
		play.v = 0;
		play.vang = 0;
		ball.vx = cos(play.ang*P / 180);
		ball.vy = sin(play.ang*P / 180);
		play.have = 0;
		play.re = 1;
	}
	return play;
}







struct P_data oneman(struct P_data play){//ワンマンプレー用の関数

	if (play.cnd == 0){
		play = find(play);
		if (play.re == 1){
			play.cnd = 1;
			play.re = 0;
		}
	}
	if (play.cnd == 1){
		play = get(play);
		if (play.have == 1){
			play.cnd = 2;
			play.re = 0;
		}
	}
	if (play.cnd == 2){
		play = turn_G(play);
		if (play.re == 1){
			play.cnd = 3;
			play.re = 0;
		}
	}
	if (play.cnd == 3){
		play = G_go(play, 400);
		if (play.re == 1){
			play.cnd = 4;
			play.re = 0;
		}
	}
	if (play.cnd == 4){
		play = shoot(play);
		if (play.re == 1){
			play.cnd = 0;
			play.re = 0;
		}
	}

	return play;
}

void stop(char side){//チーム全員をストップ
	int n = 1;
	if (side == 'A'){
		while (n <= 4){
			A[n].v = 0.0;
			n++;
		}
	}
	else if (side == 'B'){
		while (n <= 4){
			B[n].v = 0.0;
			n++;
		}
	}
}

P_data stop(struct P_data play){//一人をストップ
	play.v = 0.0;
	return play;
}


///////////////
int A_minplayer(void){
	int re;
	double B_dist[PLAYER];
	n = 1;
	while (n <= PLAYER){
		B_dist[n] = dist(A[n].X, A[n].Y, ball.X, ball.Y);	//ボールとの距離を測る
		n++;
	}
	n = 1;
	while (n <= PLAYER){
		if (fmin(fmin(fmin(B_dist[1], B_dist[2]), B_dist[3]), B_dist[4]) == B_dist[n]){	//人数が増えるときに書き直すところ
			A[n].mode = 1;		//ボールに一番近い人にmode1を振り分ける。
			re = n;
		}
		n++;
	}
	return re;
}

void A_divide(void){
	double A_watch[4];//比較対象
	double B_dist[11];
	int k;

	n = 1;
	while (n < PLAYER){
		B_dist[n] = dist(A[n].X, A[n].Y, ball.X, ball.Y);	//ボールとの距離を測る
		n++;
	}
	n = 1;
	while (n < PLAYER && A_div == 0){
		if (fmin(fmin(B_dist[1], B_dist[2]), B_dist[3]) == B_dist[n]){	//人数が増えるときに書き直すところ
			A[n].mode = 1;		//ボールに一番近い人にmode1を振り分ける。
			Atean_mode[1] = n;
		}
		n++;
	}
	n = 1;
	k = 1;


	while (n < PLAYER && A_div == 0){
		if (n != Atean_mode[1]){
			A_watch[k] = dist(A[n].X, A[n].Y, -500, -500);
			k++;
		}
		n++;
	}
	n = 1;
	while (n < PLAYER && A_div == 0){
		if (n != Atean_mode[1]){
			if (fmin(A_watch[1], A_watch[2]) == dist(A[n].X, A[n].Y, -500, -500)){
				A[n].mode = 3;
				Atean_mode[3] = n;
			}
		}
		n++;
	}
	n = 1;
	k = 1;


	while (n < PLAYER && A_div == 0){
		if (n != Atean_mode[1] && n != Atean_mode[3]){
			A_watch[k] = dist(A[n].X, A[n].Y, 500, 100);
			k++;
		}
		n++;
	}
	n = 1;
	while (n < PLAYER && A_div == 0){
		if (n != Atean_mode[1] && n != Atean_mode[3]){
			if (fmin(A_watch[1], A_watch[2]) == dist(A[n].X, A[n].Y, 500, 100)){
				A[n].mode = 2;
				Atean_mode[2] = n;
			}
		}
		n++;
	}
	n = 1;
	/*while (n < PLAYER && A_div == 0){
	if ((n != Atean_mode[1] && n != Atean_mode[2]) && n != Atean_mode[3]){
	A[n].mode = 4;
	Atean_mode[4] = n;
	}
	n++;
	}*/
	A_div = 1;

}


int A_disideRoot(char axis, double start, double end){
	int cnt = 0;
	switch (axis)
	{
	case 'x':
		for (int i = 1; i <= PLAYER; i++){
			if (start < B[i].X&&B[i].X < end){
				cnt++;
			}
		}
		break;
	case 'y':
		for (int i = 1; i <= PLAYER; i++){
			if (start < B[i].Y&&B[i].Y < end){
				cnt++;
			}
		}
		break;
	}
	return cnt;
}

int A_getpos(P_data play){
	if (play.X < -216){
		return -1;
	}
	else if (-216 < play.X&&play.X < 216){
		return 0;
	}
	else if (216 < play.X){
		return 1;
	}
}

double calc_passang(P_data passer, P_data getter){
	return atan2(getter.Y - passer.Y, getter.X - passer.X)*(180 / P);
}


void A_Keeper(void){
	double A4_X = pow(10000 / (1 + pow((ball.Y - 905) / ball.X, 2)), 0.5);
	if (ball.X < 0 && A4_X>0){
		A4_X = -A4_X;
	}
	if (A[4].cnd == 0){
		A[4] = move(A[4], A4_X, ((ball.Y - 905) / ball.X)*A4_X + 905);
		if (A[4].re == 1){
			A[4].cnd = 1;
			A[4].re = 0;
		}
	}
	if (A[4].cnd == 1){
		A[4] = turn_B(A[4]);
	}
	if (A[4].have == 1){
		A[4].cnd = 2;
	}
	if (A[4].cnd == 2){
		A[4] = pass(A[4], A[Atean_mode[1]]);
	}
	if (dist(A[4].X, A[4].Y, A4_X, ((ball.Y - 905) / ball.X)*A4_X + 905) > 10 && A[4].have == 0 && A[4].cnd != 0){
		A[4].cnd = 0;
	}
}
/////////////////

/////////////////

void sampleB_Keeper(void){
	double B4_X = pow(10000 / (1 + pow((ball.Y + 905) / ball.X, 2)), 0.5);
	if (ball.X < 0 && B4_X>0){
		B4_X = -B4_X;
	}
	if (B[4].cnd == 0){
		B[4] = move(B[4], B4_X, ((ball.Y + 905) / ball.X)*B4_X - 905);
		if (B[4].re == 1){
			B[4].cnd = 1;
			B[4].re = 0;
		}
	}
	if (B[4].cnd == 1){
		B[4] = turn_B(B[4]);
	}
	if (B[4].have == 1){
		B[4].cnd = 2;
	}
	if (B[4].cnd == 2){
		B[4] = pass(B[4], B[1]);
	}
	if (dist(B[4].X, B[4].Y, B4_X, ((ball.Y + 905) / ball.X)*B4_X - 905) > 10 && B[4].have == 0 && B[4].cnd != 0){
		B[4].cnd = 0;
	}
}


////////////////



/*-----------------------------------------------Aチームのやつです。参考にしてください-------------------------------------------------------*/

void A_init(void){//Aチームの初期位置関数
	int n = 1;
	A[1].X = 500;
	A[1].Y = 200;
	A[1].ang = -90;
	A[2].X = -500;
	A[2].Y = 200;
	A[2].ang = -90;
	A[3].X = 0;
	A[3].Y = 400;
	A[3].ang = -90;
	A[4].X = 0;
	A[4].Y = 800;
	A[4].ang = -90;
	while (n <= PLAYER){//諸コマンド初期化
		A[n].v = 0;
		A[n].vang = 0;
		A[n].cnd = 0;
		A[n].mode = 0;
		n = n + 1;
	}
	glutPostRedisplay();

}

void A_offence_init(void){//Aチームの攻撃初期位置関数
	A[1].X = -50;
	A[1].Y = 50;
	A[1].ang = -45;

	A[2].X = 100;
	A[2].Y = 100;
	A[2].ang = -135;

	A[3].X = -400;
	A[3].Y = 45;
	A[3].ang = -90;

	A[4].X = 0;
	A[4].Y = 400;
	A[4].ang = -90;
}


void A_strategy(void){	//Aチーム戦略関数(みんなにここにいろいろ書いてもらう)
	double B_dist[PLAYER + 1];
	double ang;
	int coner, coner_passer;
	int Ycnt[3] = { 0, 0, 0 };
	int root;
	int side;
	int List[PLAYER];
	double min_dist;
	double judge_max, judge_min;
	double A_pos[PLAYER + 1];
	if ((Ateam_strategy == 0 || Ateam_strategy == 1) && ball.have >= 0){
		Ateam_strategy = 0;
	}
	if (ball.have < 0){
		Ateam_strategy = 1;
	}
	//printf("%d\n", Ateam_strategy);
	n = 1;
	while (n <= PLAYER){
		B_dist[n] = dist(A[n].X, A[n].Y, ball.X, ball.Y);	//ボールとの距離を測る
		n++;
	}


	A_Keeper();
	//printf("%d\n",Ateam_strategy);

	switch (Ateam_strategy){
	case 0://攻撃
		if (sw != 0){
			A_divide();
			sw = 0;
		}
		Ycnt[0] = A_disideRoot('x', -650.0, -216.0);
		Ycnt[1] = A_disideRoot('x', -216.0, 216.0);
		Ycnt[2] = A_disideRoot('x', 216.0, 650.0);

		if (Ycnt[0] == fmin(fmin(Ycnt[0], double(Ycnt[1])), Ycnt[2])){
			root = 1;
		}
		else if (Ycnt[1] == fmin(fmin(double(Ycnt[0]), Ycnt[1]), Ycnt[2])){
			root = 2;
		}
		else if (Ycnt[2] == fmin(fmin(Ycnt[0], double(Ycnt[1])), Ycnt[2])){
			root = 3;
		}

		for (int n = 1; n < PLAYER; n++){
			switch (A[n].mode){
			case 1:
				//printf("%d\n", A[n].have);
				switch (A[n].cnd){
				case 0:

					A[n] = get(A[n]);
					if (A[n].re == 1){
						A[n].cnd = 1;
						A[n].re = 0;
					}
					break;
				case 1:
					min_dist = 10000.0;
					for (int i = 0; i < PLAYER; i++){
						min_dist = fmin(min_dist, dist(A[n].X, A[n].Y, B[i].X, B[i].Y));
					}
					if (min_dist>RANGE){
						if (A[n].X != 0){
							side = A[n].X / fabs(A[n].X);
						}
						A[n] = move(A[n], side * 400, -750);
						if (A[n].re == 1){
							A[n].cnd = 2;
							A[n].re = 0;
						}
					}
					else{
						A[Atean_mode[2]].cnd = 1;
						A[n] = pass(A[n], A[Atean_mode[2]]);
						if (A[n].re == 1){
							A[n].cnd = 4;
							A[n].re = 0;
						}
					}
					break;
				case 2:
					A[Atean_mode[3]].cnd = 2;
					A[n] = pass(A[n], A[Atean_mode[3]]);
					if (A[n].re == 1){
						A[n].cnd = 3;
						A[n].re = 0;
					}
					break;
				case 3:

					break;
				case 4:
					A[n] = move(A[n], -A_getpos(A[Atean_mode[2]]) * 434, A[Atean_mode[2]].Y - 300);
					if (A[n].re == 1){
						A[n].re = 0;
					}
					break;
				case 5:
					if (A[n].have == 1){
						A[n].cnd = 6;
					}
					A[n] = turn_B(A[n]);
					if (A[n].re == 1){
						A[n].re = 0;
					}

					break;
				case 6:
					if (A[n].Y > -600){
						min_dist = 10000.0;
						for (int i = 0; i < PLAYER; i++){
							min_dist = fmin(min_dist, dist(A[n].X, A[n].Y, B[i].X, B[i].Y));
						}
						if (min_dist>300){
							if (A[n].X != 0){
								side = A[n].X / fabs(A[n].X);
							}
							A[n] = move(A[n], side * 600, -700);
							if (A[n].re == 1){
								A[n].re = 0;
							}
						}
						else{
							A[Atean_mode[2]].cnd = 1;
							A[n] = pass(A[n], A[Atean_mode[2]]);
							if (A[n].re == 1){
								A[n].cnd = 4;
								A[n].re = 0;
							}
						}

					}
					else{
						/*A[n]=turn_P(A[n], A[Atean_mode[3]]);
						if (A[n].re == 1){
						A[n].re = 0;
						A[n].cnd = 3;
						}*/
						A[n] = turn_G(A[n]);
						if (A[n].re == 1){
							A[n].cnd = 2;
							A[n].re = 0;
						}
					}
					break;
				case 7:
					A[n] = shoot(A[n]);
					if (A[n].re == 1){
						A[n].re = 0;
						A[n].cnd = 9;
					}
					break;
				}
				break;

			case 2:
				//printf("	%d\n", A[n].cnd);
				switch (A[n].cnd){
				case 0:
					A[n] = move(A[n], -A_getpos(A[Atean_mode[1]]) * 434, A[Atean_mode[1]].Y - 300);
					if (A[n].re == 1){
						A[n].re = 0;
					}
					break;

				case 1:
					A[n] = turn_B(A[n]);
					if (A[n].re == 1){
						A[n].re = 0;
					}
					if (A[n].have == 1){
						A[n].cnd = 2;
					}
					break;

				case 2:

					if (A[n].Y > -600){

						min_dist = 10000.0;
						for (int i = 0; i < PLAYER; i++){
							min_dist = fmin(min_dist, dist(A[n].X, A[n].Y, B[i].X, B[i].Y));
						}
						if (min_dist>RANGE){
							if (A[n].X != 0){
								side = A[n].X / fabs(A[n].X);
							}
							A[n] = move(A[n], A_getpos(A[Atean_mode[2]]) * 600, -700);
							if (A[n].re == 1){
								A[n].cnd = 3;
								A[n].re = 0;
							}
						}
						else{
							A[Atean_mode[1]].cnd = 5;
							A[n] = pass(A[n], A[Atean_mode[1]]);
							if (A[n].re == 1){
								A[n].cnd = 0;
								A[n].re = 0;
							}
						}
					}
					else{
						/*A[n]=turn_P(A[n], A[Atean_mode[3]]);
						if (A[n].re == 1){
						A[n].re = 0;
						A[n].cnd = 3;
						}*/
						/*A[n] = turn_G(A[n]);
						if (A[n].re == 1){
						A[n].cnd = 3;
						A[n].re = 0;
						}*/
						A[Atean_mode[3]].cnd = 2;
						A[n] = pass(A[n], A[Atean_mode[3]]);
						if (A[n].re == 1){
							A[n].cnd = 4;
							A[n].re = 0;
						}
					}
					break;
				case 3:
					A[n] = shoot(A[n]);
					if (A[n].re == 1){
						A[n].re = 0;
						A[n].cnd = 4;
					}
					break;
				}
				break;

			case 3:
				switch (A[n].cnd){
				case 0:
					A[n] = move(A[n], -fabs(ball.X) / ball.X * r_g, -750);
					if (A[n].re == 1){
						A[n].re = 0;
						A[n].cnd = 1;
					}
					break;

				case 1:

					if (-fabs(A[n].X) / A[n].X == -fabs(ball.X) / ball.X){
						A[n].cnd = 0;
					}
				case 2:
					A[n] = turn_B(A[n]);
					if (A[n].re == 1){
						A[n].re = 0;
					}
					if (A[n].have == 1){
						A[n].cnd = 3;
					}
					break;

				case 3:

					/*if (A[n].Y > -600){
					A[n] = move(A[n], (root - 2) * 600, -700);
					if (A[n].re == 1){
					A[n].re = 0;
					}
					}
					else{*/
					/*A[n]=turn_P(A[n], A[Atean_mode[3]]);
					if (A[n].re == 1){
					A[n].re = 0;
					A[n].cnd = 3;
					}*/
					A[n] = turn_G(A[n]);
					if (A[n].re == 1){
						A[n].cnd = 4;
						A[n].re = 0;
						//}
					}
					break;
				case 4:
					A[n] = shoot(A[n]);
					if (A[n].re == 1){
						A[n].re = 0;
						A[n].cnd = 5;
					}
					break;
				}
				break;
			}

		}


		break;
	case 1://守備
		if (sw != 1){
			n = 1;
			while (n <= PLAYER){
				B_dist[n] = dist(A[n].X, A[n].Y, ball.X, ball.Y);	//ボールとの距離を測る
				n++;
			}
			n = 1;
			while (n <= PLAYER){
				if (fmin(fmin(fmin(B_dist[1], B_dist[2]), B_dist[3]), B_dist[4]) == B_dist[n]){	//人数が増えるときに書き直すところ
					A[n].mode = 1;		//ボールに一番近い人にmode1を振り分ける。
					Atean_mode[1] = n;
				}
				n++;
			}

			A[Atean_mode[1]].mode = 1;
			n = 1;
			for (int i = 1; i < PLAYER; i++){
				A[n].cnd = 0;
				if (i != Atean_mode[1]){
					List[n] = i;
					n++;
				}
			}
			if (A[List[1]].Y == fmax(A[List[1]].Y, A[List[2]].Y)){
				Atean_mode[2] = List[1];
				A[List[1]].mode = 2;
				Atean_mode[3] = List[2];
				A[Atean_mode[3]].mode = 3;
			}
			else{
				Atean_mode[2] = List[2];
				A[List[1]].mode = 2;
				Atean_mode[3] = List[1];
				A[Atean_mode[3]].mode = 3;
			}
			sw = 1;
		}
		n = 1;

		for (int n = 1; n <= PLAYER; n++){
			switch (A[n].mode){
			case 1:
				//printf("OK\n");
				A[n] = move(A[n], ball.X, ball.Y);
				if (A[n].re == 1){
					A[n].re = 0;
				}
				break;
			case 2:
				A[n] = move(A[n], ball.X, ball.Y);
				if (A[n].re == 1){
					A[n].re = 0;
				}
				break;
			case 3:
				A[n] = move(A[n], -fabs(ball.X) / ball.X * 300, -400);
				if (A[n].re == 1){
					A[n].re = 0;
				}
				break;
			}
		}


		break;


	case 2://スローイン

		if (A[1].cnd == 0){
			A[1] = pass(A[1], A[2]);
			if (A[1].re == 1){
				A[1].re = 0;
				A[1].cnd = 0;
			}
		}
		if (A[2].have == 1){
			switch (A[2].cnd){
			case 0:
				A[2] = turn_G(A[2]);
				if (A[2].re == 1){
					A[2].re = 0;
					A[2].cnd = 1;
				}
				break;
			case 1:
				A[2] = shoot(A[2]);
				if (A[2].re == 1){
					A[2].re = 0;
					A[2].cnd = 0;
				}
				break;
			}
		}
		/*while (n <= 4){
		judge_max = (A[n].Y + 905) / A[n].X - 905 + 30 * sin((A[n].ang + 90)*P / 180);
		judge_min = (A[n].Y + 905) / A[n].X - 905 - 30 * sin((A[n].ang + 90)*P / 180);

		k = 1;
		coner = 0;
		while (k <= 4){
		if ((judge_min < B[k].Y && B[k].Y < judge_max)){
		coner = coner + 1;
		}
		k++;
		}
		if (coner == 4){
		coner_passer = n;
		}
		n++;
		}
		if (A[1].have == 0){
		A[1].v = 0.1;
		}

		if (A[1].have == 1){
		A[1].v = 0;
		A[1] = pass(A[1], A[coner_passer]);
		}*/

		/*if (A[n].have == 1){
		A[n] = turn_G(A[n]);
		if (A[n].re == 1){
		A[n] = shoot(A[n]);
		}
		}*/
		break;
	case 3://コーナーキック
		if (A[1].cnd == 0){
			A[1] = pass(A[1], A[3]);
			if (A[1].re == 1){
				A[1].re = 0;
				A[1].cnd = 0;
			}
		}
		if (A[3].have == 1){
			switch (A[3].cnd){
			case 0:
				A[3] = turn_G(A[3]);
				if (A[3].re == 1){
					A[3].re = 0;
					A[3].cnd = 1;
				}
				break;
			case 1:
				A[3] = shoot(A[3]);
				if (A[3].re == 1){
					A[3].re = 0;
					A[3].cnd = 0;
				}
				break;
			}
		}
		break;
	case 4://ゴールキック
		if (A[4].cnd == 0){
			A[4] = pass(A[4], A[3]);
			if (A[4].re == 1){
				A[4].re = 0;
			}
		}
		if (A[3].have == 1){
			switch (A[3].cnd){
			case 0:
				A[3] = turn_P(A[3], A[2]);
				if (A[3].re == 1){
					A[3].re = 0;
					A[3].cnd = 1;
				}
				break;
			case 1:
				A[3] = pass(A[3], A[2]);
				if (A[3].re == 1){
					A[3].re = 0;
					A[3].cnd = 0;
				}
				break;
			}
		}
		A[2] = turn_B(A[2]);
		if (A[2].have == 1){
			switch (A[2].cnd){
			case 0:
				A[2] = turn_P(A[2], A[4]);
				if (A[2].re == 1){
					A[2].re = 0;
					A[2].cnd = 1;
				}
				break;
			case 1:
				A[2] = pass(A[2], A[4]);
				if (A[2].re == 1){
					A[2].re = 0;
					A[2].cnd = 0;
				}
				break;
			}
		}
		if (A[1].have == 1){
			switch (A[1].cnd){
			case 0:
				A[1] = turn_G(A[1]);
				if (A[1].re == 1){
					A[1].re = 0;
					A[1].cnd = 1;
				}
				break;
			case 1:
				A[1] = shoot(A[1]);
				if (A[1].re == 1){
					A[1].re = 0;
					A[1].cnd = 0;
				}
				break;
			}
		}
		break;
	case 5:
		break;
	case 6:
		break;
	}

}


void A_lineover_init(void){//線を割った時のAチームの関数
	int side;
	if (ball.X > 0){
		side = 1;
	}
	else{
		side = -1;
	}

	n = 1;
	stop('A');
	while (n <= 4){
		A[n].cnd = 0;
		A[n].mode = 0;
		n++;
	}

	switch (ball.state)
	{
	case -3:
		//相手チームのコーナーキック
		break;
	case -2:
		//相手チームのゴールキック
		break;
	case -1:
		//相手チームのスローイン
		break;
	case 1:
		//自分のチームのスローイン
		ball.X = side * 630;
		A[1].X = ball.X + side * 55;
		A[1].Y = ball.Y;
		A[1].ang = 90 + side * 90;
		if (ball.Y > 800){//自ゴールに近い
			A[2].X = side * 800;
			A[2].Y = ball.Y - 300;
			A[2].ang = -180 + atan2(A[2].Y - ball.Y, A[2].X - ball.X) * 180 / P;
			A[3].X = -side * 300;
			A[3].Y = ball.Y - 150;
			A[3].ang = -180 + atan2(A[3].Y - ball.Y, A[3].X - ball.X) * 180 / P;
			A[4].X = 0;
			A[4].Y = 800;
			A[4].ang = -180 + atan2(A[4].Y - ball.Y, A[4].X - ball.X) * 180 / P;

		}
		else if (ball.Y<-500){//敵ゴールに近い
			A[2].X = side * 200;
			A[2].Y = ball.Y + 200;
			A[2].ang = -180 + atan2(A[2].Y - ball.Y, A[2].X - ball.X) * 180 / P;
			A[3].X = -side * 300;
			A[3].Y = ball.Y + 50;
			A[3].ang = -180 + atan2(A[3].Y - ball.Y, A[3].X - ball.X) * 180 / P;
			A[4].X = 0;
			A[4].Y = 300;
			A[4].ang = -180 + atan2(A[4].Y - ball.Y, A[4].X - ball.X) * 180 / P;

		}
		else{//その他
			A[2].X = side * 400;
			A[2].Y = ball.Y + 100;
			A[2].ang = -180 + atan2(A[2].Y - ball.Y, A[2].X - ball.X) * 180 / P;
			A[3].X = side * 200;
			A[3].Y = ball.Y - 400;
			A[3].ang = -180 + atan2(A[3].Y - ball.Y, A[3].X - ball.X) * 180 / P;
			A[4].X = -side * 300;
			A[4].Y = ball.Y - 200;
			A[4].ang = -180 + atan2(A[4].Y - ball.Y, A[4].X - ball.X) * 180 / P;
		}
		Ateam_strategy = 2;
		break;
	case 2:
		//自分のチームのゴールーキック
		ball.X = 0;
		ball.Y = 800;
		A[4].X = 0;
		A[4].Y = 855;
		A[4].ang = -90;

		A[2].X = 300;
		A[2].Y = 600;
		A[2].ang = -180 + atan2(A[2].Y - ball.Y, A[2].X - ball.X) * 180 / P;
		A[3].X = -300;
		A[3].Y = 600;
		A[3].ang = -180 + atan2(A[3].Y - ball.Y, A[3].X - ball.X) * 180 / P;
		A[1].X = 0;
		A[1].Y = -400;
		A[1].ang = -180 + atan2(A[4].Y - ball.Y, A[4].X - ball.X) * 180 / P;
		A[1].cnd = 0;
		Ateam_strategy = 4;
		break;
	case 3:
		//自分のチームのコーナキック
		ball.X = side * 630;
		ball.Y = -905;
		A[1].X = side * 670;
		A[1].Y = -945;
		A[1].ang = 45;
		A[1].have = 1;

		A[2].X = side * 400;
		A[2].Y = -500;
		A[2].ang = -180 + atan2(A[2].Y - ball.Y, A[2].X - ball.X) * 180 / P;

		A[3].X = -side * 100;
		A[3].Y = -550;
		A[3].ang = -180 + atan2(A[3].Y - ball.Y, A[3].X - ball.X) * 180 / P;

		A[4].X = side*(-400);
		A[4].Y = -700;
		A[4].ang = -180 + atan2(A[4].Y - ball.Y, A[4].X - ball.X) * 180 / P;
		Ateam_strategy = 3;
		break;
	}
	glutIdleFunc(NULL);

}

/*---------------------------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------ここに書いてください-----------------------------------------------------------------*/

void B_init(void){//Bチームの初期位置関数
	int n = 1;
	//B[1].X = ;
	//B[1].Y = ;
	//B[1].ang = ;
	//B[2].X = ;
	//B[2].Y = ;
	//B[2].ang = ;
	//B[3].X = ;
	//B[3].Y = ;
	//B[3].ang = ;
	//B[4].X = ;
	//B[4].Y = ;
	//B[4].ang = ;


	/*while (n <= PLAYER){
	B[n].v = 0;
	B[n].vang = 0;
	B[n].cnd = 0;
	B[n].mode = 0;
	n = n + 1;
	}*/
	glutPostRedisplay();
}

void B_offence_init(void){//Bチームの攻撃初期位置関数
	//B[1].X = ;
	//B[1].Y = ;
	//B[1].ang = ;

	//B[2].X = ;
	//B[2].Y = ;
	//B[2].ang = ;

	//B[3].X = ;
	//B[3].Y = ;
	//B[3].ang = ;

	//B[4].X = ;
	//B[4].Y = ;
	//B[4].ang = ;
}

void B_strategy(void){

}


void B_lineover_init(void){//線を割った時のBチームの関数
	int side;
	if (ball.X > 0){
		side = 1;
	}
	else{
		side = -1;
	}

	n = 1;
	while (n <= 4){
		B[n].cnd = 0;
		B[n].mode = 0;
		n++;
	}

	switch (ball.state)
	{
	case -3:
		//自分のチームのコーナーキック
		ball.X = side * 630;
		ball.Y = 905;

		B[1].X = side * 670;//キッカー
		B[1].Y = 945;
		B[1].ang = -90 - side * 45;

		//B[2].X = ;
		//B[2].Y = ;
		//B[2].ang = ;

		//B[3].X = ;
		//B[3].Y = ;
		//B[3].ang = ;

		//B[4].X = ;
		//B[4].Y = ;
		//B[4].ang = ;
		break;
	case -2:
		//自分のチームのゴールキック
		ball.X = 0;
		ball.Y = -800;

		B[4].X = 0;//キッカー(キーパーポジションが4の場合)
		B[4].Y = -855;
		B[4].ang = 90;

		//B[2].X = ;
		//B[2].Y = ;
		//B[2].ang = ;

		//B[3].X = ;
		//B[3].Y = ;
		//B[3].ang = ;

		//B[4].X = ;
		//B[4].Y = ;
		//B[4].ang = ;
		break;
	case -1:
		//自分のチームのスローイン
		B[1].X = ball.X + side * 55;
		B[1].Y = ball.Y;
		B[1].ang = A[1].ang = 90 + side * 90;
		//B[2].X = ;
		//B[2].Y = ;
		//B[2].ang = ;

		//B[3].X = ;
		//B[3].Y = ;
		//B[3].ang = ;

		//B[4].X = ;
		//B[4].Y = ;
		//B[4].ang = ;

		break;
	case 1:
		//相手チームのスローイン

		//B[1].X = ;
		//B[1].Y = ;
		//B[1].ang = ;

		//B[2].X = ;
		//B[2].Y = ;
		//B[2].ang = ;

		//B[3].X = ;
		//B[3].Y = ;
		//B[3].ang = ;

		//B[4].X = ;
		//B[4].Y = ;
		//B[4].ang = ;
		break;
	case 2:
		//相手チームのゴールキック

		//B[1].X = ;
		//B[1].Y = ;
		//B[1].ang = ;

		//B[2].X = ;
		//B[2].Y = ;
		//B[2].ang = ;

		//B[3].X = ;
		//B[3].Y = ;
		//B[3].ang = ;

		//B[4].X = ;
		//B[4].Y = ;
		//B[4].ang = ;
		break;
	case 3:
		//相手チームのコーナーキック

		//B[1].X = ;
		//B[1].Y = ;
		//B[1].ang = ;

		//B[2].X = ;
		//B[2].Y = ;
		//B[2].ang = ;

		//B[3].X = ;
		//B[3].Y = ;
		//B[3].ang = ;

		//B[4].X = ;
		//B[4].Y = ;
		//B[4].ang = ;
		break;
	}


}

/*----------------------------------------------------------------------------------------------------------------------------------*/


/*-------------------------------------------------サンプルチームのやつです。参考にしてください-------------------------------------*/

void sample_init(void){//Bチームの初期位置関数
	int n = 1;
	B[1].X = -400;
	B[1].Y = -500;
	B[1].ang = 90;
	B[2].X = 350;
	B[2].Y = 100;
	B[2].ang = 90;
	B[3].X = -100;
	B[3].Y = -450;
	B[3].ang = 90;
	B[4].X = 0;
	B[4].Y = -800;
	B[4].ang = 90;


	while (n <= PLAYER){
		B[n].v = 0;
		B[n].vang = 0;
		B[n].cnd = 0;
		B[n].mode = 0;
		n = n + 1;
	}
	glutPostRedisplay();
}


void sample_offence_init(void){//sampleチームの攻撃初期位置関数
	B[1].X = -50;
	B[1].Y = -50;
	B[1].ang = 45;

	B[2].X = 100;
	B[2].Y = -100;
	B[2].ang = 135;

	B[3].X = -400;
	B[3].Y = -45;
	B[3].ang = 90;

	B[4].X = 0;
	B[4].Y = -400;
	B[4].ang = 90;
}

void sample_strategy(void){//sampleチームの戦略関数
	double B3_X = pow(240000 / (1 + pow((ball.Y + 905) / ball.X, 2)), 0.5);
	if (ball.X < 0 && B3_X>0){
		B3_X = -B3_X;
	}

	if (Bteam_strategy == 0 || Bteam_strategy == 1){
		if (ball.have >= 0){
			Bteam_strategy = 0;
		}
		else if (ball.have < 0){
			Bteam_strategy = 1;
		}
	}
	switch (Bteam_strategy)
	{
	case 0:
	case 1:
		/*if (B[2].have == 0){
		B[2] = move(B[2], ball.X, ball.Y);
		}
		if (B[2].have == 1){
		B[2].v = 0;
		B[2]=turn(B[2], (atan2(905 - B[2].Y, -B[2].X)*180/P));
		if (B[2].re == 1){
		B[2].cnd = 1;
		B[2].re = 0;
		}
		}
		if (B[2].cnd == 1){
		B[2] = shoot(B[2]);
		if (B[2].re == 1){
		B[2].cnd = 2;
		B[2].re = 0;
		}
		}*/

		sampleB_Keeper();

		if (B[1].have == 0){
			B[1] = move(B[1], ball.X, ball.Y);
		}
		if (B[1].have == 1){
			B[1].v = 0;
			//B[1] = B_turn_G(B[1]);
			B[1].cnd = 1;
			if (B[1].re == 1){
				B[1].cnd = 1;
				B[1].re = 0;
			}
		}
		if (B[1].cnd == 1){
			//B[1] = shoot(B[1]);
			B[1] = pass(B[1], B[2]);
		}
		if (B[1].re == 1){
			B[1].cnd = 0;
			B[1].re = 0;
		}

		if (B[2].cnd == 0){
			//B[2] = move(B[2], (A[Atean_mode[3]].X + A[Atean_mode[2]].X) / 2, (A[Atean_mode[3]].Y + A[Atean_mode[2]].Y) / 2);
			B[2] = move(B[2], 400, 600);
			if (B[2].re == 1){
				B[2].cnd = 1;
				B[2].re = 0;
			}
		}
		if (B[2].cnd == 1){
			B[2] = turn_B(B[2]);
			//B[2] = turn_P(B[2],B[1]);
			if (B[2].re == 1){
				B[2].re = 0;
				//B[2].cnd = 2;
			}
		}
		if (B[2].have == 1){
			//B[2] = pass(B[2], B[1]);
			B[2] = turn_G(B[2]);
			if (B[2].re == 1){
				B[2].cnd = 3;
				B[2].re = 0;
			}

			if (B[2].cnd == 3){
				B[2] = shoot(B[2]);
			}
		}
		if (B[2].have == 0 && B[2].cnd == 2){
			B[2].cnd = 0;
		}
		/*if (B[2].cnd == 2){
		B[2] = pass(B[2], B[1]);
		if (B[2].re == 1){
		B[2].cnd = 0;
		B[2].re = 0;
		}
		}
		if (dist(B[2].X, B[2].Y, (A[Atean_mode[3]].X + A[Atean_mode[2]].X) / 2, (A[Atean_mode[3]].Y + A[Atean_mode[2]].Y) / 2) > 10 && B[2].have == 0 && B[2].cnd != 0){
		B[2].cnd = 0;
		B[2].re = 0;
		}*/

		if (ball.Y >= 0){
			if (B[3].cnd == 0){
				B[3] = move(B[3], B3_X, ((ball.Y + 905) / ball.X)*B3_X - 905);
				if (B[3].re == 1){
					B[3].cnd = 1;
					B[3].re = 0;
				}
			}
			if (B[3].cnd == 1){
				B[3] = turn_B(B[3]);
			}
			if (B[3].have == 1){
				B[3].cnd = 2;
			}
			if (B[3].cnd == 2){
				B[3] = pass(B[3], B[1]);
			}
			if (dist(B[3].X, B[3].Y, B3_X, ((ball.Y + 905) / ball.X)*B3_X - 905) > 10 && B[3].have == 0 && B[3].cnd != 0){
				B[3].cnd = 0;
			}
		}
		else{
			if (B[3].have == 0){
				B[3] = move(B[3], ball.X, ball.Y);
			}
			if (B[3].have == 1){
				B[3].v = 0;
				B[3] = turn(B[3], (atan2(905 - B[3].Y, -B[3].X) * 180 / P));
				if (B[3].re == 1){
					B[3].cnd = 1;
					B[3].re = 0;
				}
			}
			if (B[3].cnd == 1){
				B[3] = pass(B[3], B[2]);
				if (B[3].re == 1){
					B[3].cnd = 2;
					B[3].re = 0;
				}
			}
		}
		break;
	}


}

void sample_lineover_init(void){//線を割った時のBチームの関数
	int side;
	if (ball.X > 0){
		side = 1;
	}
	else{
		side = -1;
	}

	n = 1;
	while (n <= 4){
		B[n].cnd = 0;
		B[n].mode = 0;
		n++;
	}

	switch (ball.state)
	{
	case -3:
		//自分のチームのコーナーキック
		ball.X = side * 630;
		ball.Y = 905;
		B[1].X = side * 670;
		B[1].Y = 945;
		B[1].ang = -90 - side * 45;

		B[2].X = side * 400;
		B[2].Y = 500;
		B[2].ang = -180 + atan2(A[2].Y - ball.Y, A[2].X - ball.X) * 180 / P;

		B[3].X = -side * 100;
		B[3].Y = 550;
		B[3].ang = -180 + atan2(A[3].Y - ball.Y, A[3].X - ball.X) * 180 / P;

		B[4].X = side*(-400);
		B[4].Y = 700;
		B[4].ang = -180 + atan2(A[4].Y - ball.Y, A[4].X - ball.X) * 180 / P;
		break;
	case -2:
		//自分のチームのゴールキック
		ball.X = 0;
		ball.Y = -800;
		B[1].X = 0;
		B[1].Y = -855;
		B[1].ang = 90;

		B[2].X = 300;
		B[2].Y = -600;
		B[2].ang = -180 + atan2(A[2].Y - ball.Y, A[2].X - ball.X) * 180 / P;
		B[3].X = -300;
		B[3].Y = -600;
		B[3].ang = -180 + atan2(A[3].Y - ball.Y, A[3].X - ball.X) * 180 / P;
		B[4].X = 0;
		B[4].Y = 400;
		B[4].ang = -180 + atan2(A[4].Y - ball.Y, A[4].X - ball.X) * 180 / P;
		break;
	case -1:
		//自分のチームのスローイン
		B[1].X = ball.X + side * 55;
		B[1].Y = ball.Y;
		B[1].ang = A[1].ang = 90 + side * 90;
		if (ball.Y < -800){//自ゴールに近い
			B[2].X = side * 800;
			B[2].Y = ball.Y + 300;
			B[2].ang = +180 + atan2(A[2].Y - ball.Y, A[2].X - ball.X) * 180 / P;
			B[3].X = -side * 300;
			B[3].Y = ball.Y + 150;
			B[3].ang = +180 + atan2(A[3].Y - ball.Y, A[3].X - ball.X) * 180 / P;
			B[4].X = 0;
			B[4].Y = -800;
			B[4].ang = +180 + atan2(A[4].Y - ball.Y, A[4].X - ball.X) * 180 / P;

		}
		else if (ball.Y > 500){//敵ゴールに近い
			B[2].X = side * 200;
			B[2].Y = ball.Y - 200;
			B[2].ang = 180 + atan2(A[2].Y - ball.Y, A[2].X - ball.X) * 180 / P;
			B[3].X = -side * 300;
			B[3].Y = ball.Y - 50;
			B[3].ang = 180 + atan2(A[3].Y - ball.Y, A[3].X - ball.X) * 180 / P;
			B[4].X = 0;
			B[4].Y = -300;
			B[4].ang = 180 + atan2(A[4].Y - ball.Y, A[4].X - ball.X) * 180 / P;

		}
		else{//その他
			B[2].X = side * 400;
			B[2].Y = ball.Y - 100;
			B[2].ang = 180 + atan2(A[2].Y - ball.Y, A[2].X - ball.X) * 180 / P;
			B[3].X = side * 200;
			B[3].Y = ball.Y + 400;
			B[3].ang = 180 + atan2(A[3].Y - ball.Y, A[3].X - ball.X) * 180 / P;
			B[4].X = -side * 300;
			B[4].Y = ball.Y + 200;
			B[4].ang = 180 + atan2(A[4].Y - ball.Y, A[4].X - ball.X) * 180 / P;
		}

		break;
	case 1:
		B[1].X = side * 200;
		B[1].Y = ball.Y - 200;
		B[1].ang = 180 + atan2(A[2].Y - ball.Y, A[2].X - ball.X) * 180 / P;
		B[2].X = side * 200;
		B[2].Y = ball.Y - 200;
		B[2].ang = 180 + atan2(A[2].Y - ball.Y, A[2].X - ball.X) * 180 / P;
		B[3].X = -side * 300;
		B[3].Y = ball.Y - 50;
		B[3].ang = 180 + atan2(A[3].Y - ball.Y, A[3].X - ball.X) * 180 / P;
		B[4].X = 0;
		B[4].Y = -300;
		B[4].ang = 180 + atan2(A[4].Y - ball.Y, A[4].X - ball.X) * 180 / P;
		//相手チームのスローイン
		break;
	case 2:
		//相手チームのゴールキック
		break;
	case 3:
		//相手チームのコーナーキック
		break;
	}


}


/*---------------------------------------------------------------------------------------------------------------------------*/







void goal(void){//ゴール判定関数
	if (l_g < ball.X && ball.X < r_g && ball.Y > A_g){
		A_init();
		B_init();
		//sample_init()
		goalflag = 1;
	}
	else if (l_g < ball.X&&ball.X < r_g&&ball.Y < B_g){
		A_init();
		if (PLAY_MODE){
			B_init();
		}
		else if (!PLAY_MODE){
			sample_init();
		}
		goalflag = -1;
	}
}

void wall(void){//壁判定の関数
	if (((ball.X>650 || ball.X<-650 || ball.Y>925 || ball.Y<-925)) && (ball.X<l_g || ball.X>r_g)){
		//printf("wall;\n");

		if ((ball.X>650 || ball.X < -650) && (ball.Y<925 && ball.Y>-925)){
			ball.state = ball.have*(-1);
		}
		else if (ball.Y < -925){
			if (ball.have>0){
				ball.state = -2;
			}
			else if (ball.have < 0){
				ball.state = 3;
			}
		}
		else if (ball.Y > 925){
			if (ball.have>0){
				ball.state = -3;
			}
			else if (ball.have < 0){
				ball.state = 2;
			}
		}

		ball.vx = 0;
		ball.vy = 0;
		game = 1;
		A_lineover_init();
		if (PLAY_MODE){
			B_lineover_init();
		}
		else if (!PLAY_MODE){
			sample_init();
		}

	}
	else if (goalflag == 1 || goalflag == -1){
		if ((ball.Y > 940 || ball.Y<-940) && (ball.X<r_g || ball.X > l_g)){
			//printf("wall;\n");
			if (goalflag == 1){
				score_B++;
			}
			else if (goalflag == -1){
				score_A++;
			}
			ball.vx = 0;
			ball.vy = 0;
			ball.X = 0;
			ball.Y = 0;
			goalflag = 0;
			printf("A:%d-%d:B\n", score_A, score_B);
			glutIdleFunc(NULL);
		}
	}
}



void ground(void){	//グランド描画関数
	glPushMatrix();
	glColor3f(1, 1, 1);
	circle(0, 0, 160);
	glColor3f(g_R, g_G, g_B);
	circle(0, 0, 150);
	glColor3f(1, 1, 1);
	glBegin(GL_POLYGON);
	glVertex2d(-625, 905);
	glVertex2d(625, 905);
	glVertex2d(625, 895);
	glVertex2d(-625, 895);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex2d(-625, -905);
	glVertex2d(625, -905);
	glVertex2d(625, -895);
	glVertex2d(-625, -895);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex2d(625, -905);
	glVertex2d(635, -905);
	glVertex2d(635, 905);
	glVertex2d(625, 905);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex2d(-625, -905);
	glVertex2d(-635, -905);
	glVertex2d(-635, 905);
	glVertex2d(-625, 905);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex2d(-635, 5);
	glVertex2d(-635, -5);
	glVertex2d(635, -5);
	glVertex2d(635, 5);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex2d(-160, 905);
	glVertex2d(-160, 745);
	glVertex2d(160, 745);
	glVertex2d(160, 905);
	glEnd();
	glColor3f(g_R, g_G, g_B);
	glBegin(GL_POLYGON);
	glVertex2d(-150, 895);
	glVertex2d(-150, 755);
	glVertex2d(150, 755);
	glVertex2d(150, 895);
	glEnd();
	glColor3f(1, 1, 1);
	glBegin(GL_POLYGON);
	glVertex2d(-160, -905);
	glVertex2d(-160, -745);
	glVertex2d(160, -745);
	glVertex2d(160, -905);
	glEnd();
	glColor3f(g_R, g_G, g_B);
	glBegin(GL_POLYGON);
	glVertex2d(-150, -895);
	glVertex2d(-150, -755);
	glVertex2d(150, -755);
	glVertex2d(150, -895);
	glEnd();
	glPopMatrix();

}

void debug_disp(){
	double s = P / 18;
	int i;
	i = 0;
	glColor3f(1.0, 0, 0);
	glPushMatrix();
	glTranslatef(A[1].X, A[1].Y, 0);
	glBegin(GL_LINES);
	while (i < 35){
		glVertex2d(x + RANGE * cos(s*i), y + RANGE * sin(s*i));
		i = i + 1;
	}
	glEnd();
	glPopMatrix();

	i = 0;
	glColor3f(1.0, 0.3, 0);
	glPushMatrix();
	glTranslatef(A[2].X, A[2].Y, 0);
	glBegin(GL_LINES);
	while (i < 35){
		glVertex2d(x + RANGE * cos(s*i), y + RANGE * sin(s*i));
		i = i + 1;
	}
	glEnd();
	glPopMatrix();

	i = 0;
	glColor3f(1.0, 0.6, 0);
	glPushMatrix();
	glTranslatef(A[3].X, A[3].Y, 0);
	glBegin(GL_LINES);
	while (i < 35){
		glVertex2d(x + RANGE * cos(s*i), y + RANGE * sin(s*i));
		i = i + 1;
	}
	glEnd();
	glPopMatrix();

	i = 0;
	glColor3f(1.0, 0.9, 0);
	glPushMatrix();
	glTranslatef(A[4].X, A[4].Y, 0);
	glBegin(GL_LINES);
	while (i < 35){
		glVertex2d(x + RANGE * cos(s*i), y + RANGE * sin(s*i));
		i = i + 1;
	}
	glEnd();
	glPopMatrix();

	i = 0;
	glColor3f(0, 0, 1.0);
	glPushMatrix();
	glTranslatef(B[1].X, B[1].Y, 0);
	glBegin(GL_LINES);
	while (i < 35){
		glVertex2d(x + 80 * cos(s*i), y + 80 * sin(s*i));
		i = i + 1;
	}
	glEnd();
	glPopMatrix();

	i = 0;
	glColor3f(0, 0.3, 1.0);
	glPushMatrix();
	glTranslatef(B[2].X, B[2].Y, 0);
	glBegin(GL_LINES);
	while (i < 35){
		glVertex2d(x + 80 * cos(s*i), y + 80 * sin(s*i));
		i = i + 1;
	}
	glEnd();
	glPopMatrix();

	i = 0;
	glColor3f(0, 0.6, 1.0);
	glPushMatrix();
	glTranslatef(B[3].X, B[3].Y, 0);
	glBegin(GL_LINES);
	while (i < 35){
		glVertex2d(x + 80 * cos(s*i), y + 80 * sin(s*i));
		i = i + 1;
	}
	glEnd();
	glPopMatrix();

	i = 0;
	glColor3f(0, 0.9, 1.0);
	glPushMatrix();
	glTranslatef(B[4].X, B[4].Y, 0);
	glBegin(GL_LINES);
	while (i < 35){
		glVertex2d(x + 80 * cos(s*i), y + 80 * sin(s*i));
		i = i + 1;
	}
	glEnd();
	glPopMatrix();
}


void display(void){
	glClear(GL_COLOR_BUFFER_BIT);

	ground();

	debug_disp();

	glColor3f(1.0, 0, 0);		//A[1]
	glPushMatrix();
	glTranslatef(A[1].X, A[1].Y, 0);
	glRotatef(A[1].ang, 0, 0, 1.0);
	player();
	glPopMatrix();

	glColor3f(1.0, 0.3, 0);		//A[2]
	glPushMatrix();
	glTranslatef(A[2].X, A[2].Y, 0);
	glRotatef(A[2].ang, 0, 0, 1.0);
	player();
	glPopMatrix();

	glColor3f(1.0, 0.6, 0);		//A[3]
	glPushMatrix();
	glTranslatef(A[3].X, A[3].Y, 0);
	glRotatef(A[3].ang, 0, 0, 1.0);
	player();
	glPopMatrix();

	glColor3f(1.0, 0.9, 0);		//A[4]
	glPushMatrix();
	glTranslatef(A[4].X, A[4].Y, 0);
	glRotatef(A[4].ang, 0, 0, 1.0);
	player();
	glPopMatrix();

	glColor3f(0, 0, 1.0);		//B[1]
	glPushMatrix();
	glTranslatef(B[1].X, B[1].Y, 0);
	glRotatef(B[1].ang, 0, 0, 1.0);
	player();
	glPopMatrix();

	glColor3f(0, 0.3, 1.0);		//B[2]
	glPushMatrix();
	glTranslatef(B[2].X, B[2].Y, 0);
	glRotatef(B[2].ang, 0, 0, 1.0);
	player();
	glPopMatrix();

	glColor3f(0, 0.6, 1.0);		//B[3]
	glPushMatrix();
	glTranslatef(B[3].X, B[3].Y, 0);
	glRotatef(B[3].ang, 0, 0, 1.0);
	player();
	glPopMatrix();

	glColor3f(0, 0.9, 1.0);		//B[4]
	glPushMatrix();
	glTranslatef(B[4].X, B[4].Y, 0);
	glRotatef(B[4].ang, 0, 0, 1.0);
	player();
	glPopMatrix();


	glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslatef(ball.X, ball.Y, 0);
	circle(0.0, 0.0, 20);
	glPopMatrix();

	glColor3f(0.5, 0.5, 1.0);
	glPushMatrix();
	circle(A[1].X + 40 * cos((P / 18)*(3) + (A[1].ang*(P / 180))), A[1].Y + 40 * sin((P / 18)*(3) + (A[1].ang*(P / 180))), 5.0);
	glPopMatrix();


	glColor3f(1.0, 0.5, 0.5);
	glPushMatrix();
	circle(A[1].X + 40 * cos((P / 18)*(34) + (A[1].ang*(P / 180))), A[1].Y + 40 * sin((P / 18)*(34) + (A[1].ang*(P / 180))), 5.0);
	glPopMatrix();

	glColor3f(0.5, 1.0, 0.5);
	glPushMatrix();
	circle(x, y, 5.0);
	glPopMatrix();

	glColor3f(0.2, 1.0, 0.7);
	glPushMatrix();
	circle(A[1].X, A[1].Y, 5.0);
	glPopMatrix();



	glutSwapBuffers();
}




void simu(void)
{



	n = 1;
	while (n <= PLAYER){

		A[n].ang = A[n].ang + A[n].vang;
		if (A[n].ang >= 180)
			A[n].ang = A[n].ang - 360;
		else if (A[n].ang < -180)
			A[n].ang = A[n].ang + 360;

		B[n].ang = B[n].ang + B[n].vang;
		if (B[n].ang >= 180)
			B[n].ang = B[n].ang - 360;
		else if (B[n].ang < -180)
			B[n].ang = B[n].ang + 360;


		A[n].vx = A[n].v*cos(A[n].ang*(P / 180));
		A[n].vy = A[n].v*sin(A[n].ang*(P / 180));
		B[n].vx = B[n].v*cos(B[n].ang*(P / 180));
		B[n].vy = B[n].v*sin(B[n].ang*(P / 180));

		A[n].X = A[n].X + A[n].vx;
		A[n].Y = A[n].Y + A[n].vy;
		B[n].X = B[n].X + B[n].vx;
		B[n].Y = B[n].Y + B[n].vy;
		n++;
	}

	ball.X = ball.X + ball.vx;
	ball.Y = ball.Y + ball.vy;


	wall();
	player_wall();
	player_clash();
	goal();

	n = 1;

	A_strategy();
	if (PLAY_MODE){
		B_strategy();
	}
	else if (!PLAY_MODE){
		sample_strategy();
	}
	//A[1] = oneman(A[1]);
	glutPostRedisplay();//再描画

}



void init(void)
{
	glClearColor(g_R, g_G, g_B, 0.0);			//ウィンドウ全体の色
	glShadeModel(GL_FLAT);					//濃淡
	A_init();
	if (PLAY_MODE){
		B_init();
	}
	else if (!PLAY_MODE){
		sample_init();
	}
}



void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-720, 720, -960, 960, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void mouse(int button, int state, int x, int y){		//(ボタン,押したか離したか,マウスポインタのx座標,マウスポインタのy座標)
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN){
			glutIdleFunc(simu);
			ball.X = (x - 300)*2.4;
			ball.Y = -(y - 400)*2.4;
			ball.vx = 0;
			ball.vy = 0;
			game = 1;
			n = 1;
			A_div = 0;
			Ateam_strategy = 0;
			ball.have = 0;
			sw = 1;
			while (n <= PLAYER){
				A[n].cnd = 0;
				A[n].mode = 0;
				A[n].re = 0;
				A[n].vang = 0;
				A[n].have = 0;
				A[n].vx = 0;
				A[n].vy = 0;

				B[n].cnd = 0;
				B[n].mode = 0;
				B[n].re = 0;
				B[n].vang = 0;
				B[n].have = 0;
				B[n].vx = 0;
				B[n].vy = 0;
				n++;
			}

		}

		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
			glutIdleFunc(NULL);
		break;
	default:
		break;
	}
}

void keyboard(unsigned char key, int x, int y)			//(押されたボタン,マウスポインタのx座標,マウスポインタのy座標)
{
	if (key == '\x1b') exit(0);

	if (key == 's'){
		glutIdleFunc(simu);
		game = 1;
	}
	if (key == 'a') glutIdleFunc(NULL);
}

int main(int argc, char *argv[])
{
	ball.X = 0;
	ball.Y = 0;
	player_set();

	glutInit(&argc, argv);							//初期化
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600, 800);					//ウィンドウの大きさ
	glutInitWindowPosition(700, 0);					//ウィンドウの位置
	glutCreateWindow(argv[0]);						//生成
	init();
	glutReshapeFunc(reshape);							//サイズ変更時の呼び出す関数
	glutKeyboardFunc(keyboard);						//キーボード関数の参照
	glutMouseFunc(mouse);								//マウス関数の参照
	glutDisplayFunc(display);							//ディスプレイ関数の参照
	glutMainLoop();									//無限ループ
	return 0;
}