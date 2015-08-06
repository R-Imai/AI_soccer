/*--------------------------------------------------------------------------------------------------------------*/
/*	Name:		ArtificialIntelligence_soccer.c																*/
/*	Author:		Ryosuke.IMAI																					*/
/*	Created:	2015 / 02 / 01																					*/
/*	Last Date:	2015 / 08 / 06																					*/
/*	Note:		ボールがラインを割った時の試合の流れとプレイヤー同士が重ならないような関数の作成が第一優先課題	*/
/*				列挙型		Doxygen																				*/
/*				どのタイミングでオブジェクト指向を入れるか悩み中←一回試合ができてから?							*/
/*				ゴールのx座標に関連してボールが止まるバグがある。	同じ意味を持つ関数があるかも?				*/
/*	How to use:	クリック→その場所にボールがいってスタート	a→一時停止	s→再開									*/
/*--------------------------------------------------------------------------------------------------------------*/



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

struct P_data{			//プレイヤーデータ構造体
	double X, Y, ang, r_x, r_y, l_x, l_y, vx, vy, vang, v;
	int mode, cnd, have,re;
	char team;
	int debug;
};

struct B_data{			//ボールデータ構造体
	double X, Y,vx,vy;
	int cnd = 0, have, state;//haveが+→A,-→B
};

double  x, y;
double B_dist[11];
int Atean_mode[11];
int Ateam_strategy,Bteam_strategy;
int n,A_div=0;
int col = 0;
int game=0;
int goalflag;


struct P_data A[5], B[5];
struct B_data ball;

void circle(double x,double y,double r)//円を描く関数(x,y:中心座標,r:半径)
{
	double s = P / 18;
	int i;
	i = 0;
	glBegin(GL_POLYGON);
	while (i < 35){
		glVertex2d(x+r*cos(s*i), y+r*sin(s*i));
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
		glVertex2d(40 * cos(s*(i+3)), 40 * sin(s*(i+3)));
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




void A_init(void){//Aチームの初期位置関数
	int n = 1;
	A[1].X = 500;
	A[1].Y = 200;
	A[1].ang = -90;
	A[2].X = -500;
	A[2].Y = 200;
	A[2].ang = -90;
	A[3].X = 200;
	A[3].Y = 600;
	A[3].ang = -90;
	A[4].X = -200;
	A[4].Y = 600;
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

void B_init(void){//Bチームの初期位置関数
	int n = 1;
	B[1].X = 0;
	B[1].Y = -400;
	B[1].ang = 90;
	B[2].X = 400;
	B[2].Y = -200;
	B[2].ang = 90;
	B[3].X = -400;
	B[3].Y = -200;
	B[3].ang = 90;
	B[4].X = 0;
	B[4].Y = -700;
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

double dist(double x1, double y1, double x2, double y2){//(x1,y1)と(x2,y2)二点間の距離を測る関数
	double re;
	re = (x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2);
	re = pow(re, 0.5);
	return re;
}

void player_wall(void){//人にボールが当たったかの関数
	double range,ang;
	int judge,cnt=0,have=0;//judge判定	cnt内側に入った数
	n = 1;
	while (n <= PLAYER){
		range = dist(A[n].X, A[n].Y, ball.X, ball.Y);
		ang = A[n].ang - (atan2(-A[n].Y + ball.Y, -A[n].X + ball.X) * 180 / P);
		if (ang > 180){ //角度を-180～180にする
			ang=ang - 360;
		}else if (ang < -180){
			ang=ang + 360;
		}
		if (range < 56 && fabs(ang)>30){//ボールの内側に入った時にボールを持つかはじくかの判定
			judge = 1;
			cnt = cnt + 1;
		}else if (range < 56 && fabs(ang) < 30){
			have = n;
		}

		range = dist(B[n].X, B[n].Y, ball.X, ball.Y);//Bチームも同様							
		ang = B[n].ang - (atan2(-B[n].Y + ball.Y, -B[n].X + ball.X) * 180 / P);
		if (ang > 180){
			ang=ang - 360;
		}else if (ang < -180){
			ang=ang + 360;
		}
		if (range < 56 && fabs(ang)>30){
			judge = 1;
			cnt = cnt + 1;
		}else if (range < 56 && fabs(ang) < 30){
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

void goal(void){//ゴール判定関数
	if (l_g < ball.X&&ball.X<r_g&&ball.Y>A_g){
		A_init();
		B_init();
		goalflag = 1;
	}
	else if (l_g < ball.X&&ball.X < r_g&&ball.Y < B_g){
		A_init();
		B_init();
		goalflag = 1;
	}
}


void debuger(int com){//デバッグ用のプリント関数　1:位置	2:mode	3:ang	4:cnd
	switch (com){
		case 1:
		printf("A[2]:%3.0f,%3.0f	A[4]:%3.0f,%3.0f	A[3]:%3.0f,%3.0f	A[1]:%3.0f,%3.0f\n", A[2].X, A[2].Y,A[4].X,A[4].Y, A[3].X, A[3].Y, A[1].X, A[1].Y);
		break;
		case 2:
			printf("A[2]:%d	A[4]:%d	A[3]:%d	A[1]:%d\n", A[2].mode,A[4].mode, A[3].mode, A[1].mode);
		break;
		case 3:
			printf("A[2]:%3.1f	A[4]:%3.1f	A[3]:%3.1f	A[1]:%3.1f\n", A[2].ang,A[4].ang, A[3].ang, A[1].ang);
		break;
		case 4:
			printf("A[2]:%d	A[4]:%d	A[3]:%d	A[1]:%d\n", A[2].cnd,A[4].cnd, A[3].cnd, A[1].cnd);
		break;

	}

}


struct P_data turn(struct P_data play, double ang){//回転関数(引数はdeg)(完了時pray.reが1になる)
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

struct P_data G_go(struct P_data play,int meter){		//ゴールへ進む関数(完了時pray.reが1になる)　meter:ゴールとの距離
	int side;
	if (play.team == 'A'){
		side = -1;
	}
	else if (play.team == 'B'){
		side = 1;
	}
	printf("%d\n", play.re);
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
		if (dist(play.X,play.Y,xx,yy)>2&&play.re==1){
		play=go(play);
		play.re = 0;
	}
	else if (dist(play.X, play.Y, xx, yy)<2){
		play.re = 1;
	}
	return play;
}



struct P_data find(struct P_data play){	//ボール発見関数

	double ang;

	ang = atan2(ball.Y - play.Y, ball.X - play.X)*(180 / P) - play.ang ;
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
		play.vang =   0.02;
		}
		else if (ang <= 0)
		{
			play.vang =  -0.02;
		}
	}else{
		play.vang = 0.0;
		play.re=1;
	}
	return play;

}


struct P_data pass(struct P_data passer, struct P_data getter){//passerがgetterへボールをける関数(完了時pray.reが1になる)
	double pass_ang,ang;
	ang = atan2(getter.Y - ball.Y, getter.X - ball.X)*(180/P);
	pass_ang = ang-passer.ang;
	if (passer.have == 1){
		if (fabs(pass_ang) < 30){
			ball.vx = 0.5*cos(ang*P / 180);
			ball.vy = 0.5*sin(ang*P / 180);
			passer.have = 0;
			passer.re = 1;
		}
		else{
			passer = turn_P(passer,getter);
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

	/*double x1, x2, y1, y2;

	x1 = play.X + 40 * cos((P / 18)*(3) + (play.ang*(P / 180)));
	y1 = play.Y + 40 * sin((P / 18)*(3) + (play.ang*(P / 180)));
	x2 = play.X + 40 * cos((P / 18)*(34) + (play.ang*(P / 180)));
	y2 = play.Y + 40 * sin((P / 18)*(34) + (play.ang*(P / 180)));

	x = (1 / (pow(y2 - y1, 2) + pow(x2 - x1, 2)))*(((x2 - x1)*(y2 - y1)*ball.Y) + pow(x2 - x1, 2)*ball.X - (x2*y1 - x1*y2)*(y2 - y1));
	y = ((y2 - y1) / (x2 - x1))*x + (y1*x2 - y2*x1) / (x2 - x1);*/
	if (play.re == 0){
		play = turn_B(play);
		if (play.re = 1){
			play.re = 3;
		}
	}

	if (play.re == 3){
		play.v = 0.1;
		if (play.have == 1){
			play.v = 0;
			play.re = 1;
		}
	}
	return play;
}




struct P_data shoot(struct P_data play){//シュートする関数(ただ思いっきりけるだけ)(完了時pray.reが1になる)
	if (play.have = 1){
		play.v = 0;
		play.vang = 0;
		ball.vx = cos(play.ang*P/180);
		ball.vy = sin(play.ang*P/180);
		play.have = 0;
		play.re = 1;
	}
	return play;
}







struct P_data oneman(struct P_data play){//ワンマンプレー用の関数(Aチーム専用)
	if (ball.cnd != 1){

		if (play.cnd==0){
			play = find(play);
			if (play.re == 1){
				play.cnd = 1;
				play.re = 0;
			}
		}
		if (play.cnd == 1){
			play = get(play);
			if (play.have==1){
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
			play = G_go(play,400);
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
	}

	return play;
}

void stop(char side){//チーム全員をストップ(オーバーロードで個別にも止まれるようにした)
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

void stop(struct P_data play){//一人をストップ
	play.v = 0.0;
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

void B_offence_init(void){//Bチームの攻撃初期位置関数
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




void A_strategy(void){	//Aチーム戦略関数(みんなにここにいろいろ書いてもらう)
	double A_watch[5];//比較対象
	double ang;
	int k,coner,coner_passer;
	double judge_max,judge_min;
	if (Ateam_strategy == 0 || Ateam_strategy == 1){
		if (ball.have >= 0){
			Ateam_strategy = 0;
		}
		else if (ball.have < 0){
			Ateam_strategy = 1;
		}
	}

	//printf("%d\n",Ateam_strategy);
	switch (Ateam_strategy){
	case 0://攻撃
		n = 1;
		while (n <= PLAYER){
			B_dist[n] = dist(A[n].X, A[n].Y, ball.X, ball.Y);	//ボールとの距離を測る
			n++;
		}
		n = 1;
		while (n <= PLAYER && A_div == 0){
			if (fmin(fmin(fmin(B_dist[1], B_dist[2]), B_dist[3]), B_dist[4]) == dist(A[n].X, A[n].Y, ball.X, ball.Y)){	//人数が増えるときに書き直すところ
				A[n].mode = 1;		//ボールに一番近い人にmode1を振り分ける。
				Atean_mode[1] = n;
			}
			n++;
		}
		n = 1;
		k = 1;


		while (n <= 4 && A_div == 0){
			if (n != Atean_mode[1]){
				A_watch[k] = dist(A[n].X, A[n].Y, -500, -500);
				k++;
			}
			n++;
		}
		n = 1;
		while (n <= 4 && A_div == 0){
			if (n != Atean_mode[1]){
				if (fmin(fmin(A_watch[1], A_watch[2]), A_watch[3]) == dist(A[n].X, A[n].Y, -500, -500)){
					A[n].mode = 3;
					Atean_mode[3] = n;
				}
			}
			n++;
		}
		n = 1;
		k = 1;


		while (n <= 4 && A_div == 0){
			if (n != Atean_mode[1] && n != Atean_mode[3]){
				A_watch[k] = dist(A[n].X, A[n].Y, 500, 100);
				k++;
			}
			n++;
		}
		n = 1;
		while (n <= 4 && A_div == 0){
			if (n != Atean_mode[1] && n != Atean_mode[3]){
				if (fmin(fmin(A_watch[1], A_watch[2]), A_watch[3]) == dist(A[n].X, A[n].Y, 500, 100)){
					A[n].mode = 2;
					Atean_mode[2] = n;
				}
			}
			n++;
		}
		n = 1;
		while (n <= 4 && A_div == 0){
			if ((n != Atean_mode[1] && n != Atean_mode[2]) && n != Atean_mode[3]){
				A[n].mode = 4;
				Atean_mode[4] = n;
			}
			n++;
		}
		A_div = 1;

		n = 1;
		while (n <= PLAYER){
			if (A[n].mode == 1){
				//A[n] = oneman(A[n]);
				if ((A[n].cnd != 4 || A[n].cnd != 2) && A[n].cnd != 3){

					A[n] = move(A[n], ball.X, ball.Y);
					if (A[n].have == 1){
						A[n].v = 0;
						A[n].cnd = 2;
						A[n].re = 0;
					}
					//A[n] = find(A[n]);
				}
				if (A[n].cnd == 1){
					A[n] = get(A[n]);
					if (A[n].re == 1){
						A[n].re = 0;
						A[n].cnd = 2;
					}
				}
				if (A[n].cnd == 2){
					A[n] = pass(A[n], A[Atean_mode[4]]);
					//A[n] = turn(A[n], 45);
					if (A[n].re == 1){
						A[n].cnd = 3;
						A[n].re = 0;
					}
				}
				if (A[n].cnd == 3){
					A[n] = move(A[n], 0, 800);
					//A[n] = shoot(A[n]);
					if (A[n].re == 1){
						A[n].cnd = 4;
						A[n].re = 0;
					}

				}
				if (A[n].cnd == 4){
					A[n] = turn(A[n], (atan2(ball.Y - A[n].Y, ball.X - A[n].X) * 180 / P));
					if (A[n].have == 1){
						A[n].cnd = 2;
						if (A[n].re == 1){
							A[n].re = 0;
						}
					}
				}

			}
			else if (A[n].mode == 2){
				if (A[n].cnd == 0){
					A[n] = move(A[n], 500, 100);
					if (A[n].re == 1){
						A[n].cnd = 1;
						A[n].re = 0;
					}
				}

				else if (A[n].cnd == 1){
					A[n] = turn_P(A[n], A[Atean_mode[4]]);
					if (A[n].have == 1){
						A[n].cnd = 2;
						A[n].re = 0;
					}
				}

				else if (A[n].cnd == 2 && A[n].have == 1){

					A[n] = turn_P(A[n], A[Atean_mode[3]]);
					if (A[n].re == 1){
						A[n].cnd = 3;
						A[n].re = 0;
					}
				}

				else if (A[n].cnd == 3 && A[n].have == 1){
					A[n] = pass(A[n], A[Atean_mode[3]]);
					if (A[n].re == 1){
						A[n].re = 0;
						A[n].cnd = 0;
					}
				}


			}
			else if (A[n].mode == 3){
				//A[n]=turn_P(A[n], A[3 - n]);
				if (A[n].cnd == 0){
					A[n] = move(A[n], -500, -500);
					if (A[n].re == 1){
						A[n].cnd = 1;
						A[n].re = 0;
					}
				}

				else if (A[n].cnd == 1){
					A[n] = turn_P(A[n], A[Atean_mode[2]]);
					if (A[n].have == 1){
						A[n].cnd = 2;
						A[n].re = 0;
					}
				}

				else if (A[n].cnd == 2 && A[n].have == 1){

					A[n] = turn_G(A[n]);
					if (A[n].re == 1){
						A[n].cnd = 3;
						A[n].re = 0;
					}
				}

				else if (A[n].cnd == 3 && A[n].have == 1){
					A[n] = shoot(A[n]);
					if (A[n].re == 1){
						A[n].re = 0;
					}
				}
			}


			else if (A[n].mode == 4){
				if (A[n].cnd == 0){
					A[n] = move(A[n], -400, 600);
					if (A[n].re == 1){
						A[n].cnd = 1;
						A[n].re = 0;
					}
				}
				if (A[n].cnd == 1){
					A[n] = turn_P(A[n], A[Atean_mode[1]]);
					if (A[n].have == 1){
						A[n].cnd = 2;
						A[n].re = 0;
					}
				}
				if (A[n].cnd == 2 && A[n].have == 1){
					A[n] = turn_P(A[n], A[Atean_mode[2]]);
					if (A[n].re == 1){
						A[n].cnd = 3;
						A[n].re = 0;
					}
				}
				if (A[n].cnd == 3 && A[n].have == 1){
					A[n] = pass(A[n], A[Atean_mode[2]]);
					if (A[n].re == 1){
						A[n].re = 0;
						A[n].cnd = 0;
					}
				}
			}
			n++;
		}
		break;
	case 1://守備

		n = 1;
		while (n <= 4){
			if (n != Atean_mode[1]){
				A[n].cnd = 0;
			}
			n++;

		}

		ang = atan2(ball.Y - A[Atean_mode[1]].Y, ball.X - A[Atean_mode[1]].X) * 180 / P;
		//if (ang >= 180){
		//ang = ang - 360;
		//}
		//else if (ang <= -180){
		//ang = ang + 360;
		//}
		if (A[Atean_mode[1]].cnd != 4){
			A[Atean_mode[1]] = move(A[Atean_mode[1]], 0, 800);
			if (A[Atean_mode[1]].re == 1){
				A[Atean_mode[1]].re = 0;
				A[Atean_mode[1]].cnd = 4;
			}
		}
		else if (A[Atean_mode[1]].cnd == 4){
			A[Atean_mode[1]] = turn_B(A[Atean_mode[1]]);
		}
		break;


	case 2://スローイン
		n = 2;
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
				A[3] = turn_P(A[3],A[2]);
				if (A[3].re == 1){
					A[3].re = 0;
					A[3].cnd = 1;
				}
				break;
			case 1:
				A[3] = pass(A[3],A[2]);
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
		if (A[4].have == 1){
			switch (A[4].cnd){
			case 0:
				A[4] = turn_G(A[4]);
				if (A[4].re == 1){
					A[4].re = 0;
					A[4].cnd = 1;
				}
				break;
			case 1:
				A[4] = shoot(A[4]);
				if (A[4].re == 1){
					A[4].re = 0;
					A[4].cnd = 0;
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

void B_Keeper(void){
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

void B_strategy(void){//Bチームの戦略関数(みんなにここにいろいろ書いてもらう)
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

		B_Keeper();

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
		printf("%d\n", B[2].cnd);
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
				printf("NO!\n");
				B[2].cnd = 3;
				B[2].re = 0;
			}

			if (B[2].cnd == 3){
				B[2] = shoot(B[2]);
			}
		}
		if (B[2].have == 0&&B[2].cnd==2){
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
		A[1].ang = 90 + side*90;
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
		else if(ball.Y<-500){//敵ゴールに近い
			A[2].X = side*200;
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
		A[1].X = 0;
		A[1].Y = 855;
		A[1].ang = -90;

		A[2].X = 300;
		A[2].Y = 600;
		A[2].ang = -180 + atan2(A[2].Y - ball.Y, A[2].X - ball.X) * 180 / P;
		A[3].X = -300;
		A[3].Y = 600;
		A[3].ang = -180 + atan2(A[3].Y - ball.Y, A[3].X - ball.X) * 180 / P;
		A[4].X = 0;
		A[4].Y = -400;
		A[4].ang = -180 + atan2(A[4].Y - ball.Y, A[4].X - ball.X) * 180 / P;
		Ateam_strategy = 4;
		break;
	case 3:
		//自分のチームのコーナキック
		ball.X = side*630;
		ball.Y = -905;
		A[1].X = side * 670;
		A[1].Y = -945;
		A[1].ang = 45;
		A[1].have = 1;

		A[2].X = side * 400;
		A[2].Y = -500;
		A[2].ang = -180+atan2(A[2].Y-ball.Y, A[2].X - ball.X) * 180 / P ;

		A[3].X = -side*100;
		A[3].Y = -550;
		A[3].ang = -180+atan2(A[3].Y - ball.Y, A[3].X - ball.X) * 180 / P;

		A[4].X = side*(-400);
		A[4].Y = -700;
		A[4].ang = -180+atan2(A[4].Y - ball.Y, A[4].X - ball.X) * 180 / P;
		Ateam_strategy = 3;
		break;
	}
	glutIdleFunc(NULL);
	
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
		ball.X = side*630;
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
		game=1;
		A_lineover_init();
		B_lineover_init();
		
	}
	else if (goalflag == 1){
		if ((ball.Y > 940 || ball.Y<-940) || (ball.X>r_g || ball.X < l_g)){
			ball.vx = 0;
			ball.vy = 0;
			goalflag = 0;
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


void display(void){
	glClear(GL_COLOR_BUFFER_BIT);

	ground();


	glColor3f(1.0, 0, 0);		//A[1]
	glPushMatrix();
	glTranslatef(A[1].X, A[1].Y, 0);				
	glRotatef(A[1].ang, 0, 0, 1.0);					
	player();
	glPopMatrix();

	glColor3f(1.0, 0, 0);		//A[2]
	glPushMatrix();
	glTranslatef(A[2].X, A[2].Y, 0);
	glRotatef(A[2].ang, 0, 0, 1.0);
	player();
	glPopMatrix();

	glColor3f(1.0, 0, 0);		//A[3]
	glPushMatrix();
	glTranslatef(A[3].X, A[3].Y, 0);
	glRotatef(A[3].ang, 0, 0, 1.0);
	player();
	glPopMatrix();

	glColor3f(1.0, 0, 0);		//A[4]
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

	glColor3f(0, 0, 1.0);		//B[2]
	glPushMatrix();
	glTranslatef(B[2].X, B[2].Y, 0);
	glRotatef(B[2].ang, 0, 0, 1.0);
	player();
	glPopMatrix();

	glColor3f(0, 0, 1.0);		//B[3]
	glPushMatrix();
	glTranslatef(B[3].X, B[3].Y, 0);
	glRotatef(B[3].ang, 0, 0, 1.0);
	player();
	glPopMatrix();

	glColor3f(0, 0, 1.0);		//B[4]
	glPushMatrix();
	glTranslatef(B[4].X, B[4].Y, 0);
	glRotatef(B[4].ang, 0, 0, 1.0);
	player();
	glPopMatrix();


	glColor3f(0, 0, 0);		
	glPushMatrix();
	glTranslatef(ball.X, ball.Y, 0);
	circle(0.0,0.0,20);
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
	/*double A_watch[5];//比較対象
	int k;
	debuger(4);
	//printf("%f	%f\n", ball.X, ball.Y);

	n = 1;
	while (n <= PLAYER){
		B_dist[n] = dist(A[n].X, A[n].Y, ball.X, ball.Y);	//ボールとの距離を測る
		n++;
	}
	n = 1;
	while (n <= PLAYER && A_div == 0){
		if (fmin(fmin(fmin(B_dist[1], B_dist[2]), B_dist[3]), B_dist[4]) == dist(A[n].X, A[n].Y, ball.X, ball.Y)){	//人数が増えるときに書き直すところ
			A[n].mode = 1;		//ボールに一番近い人にmode1を振り分ける。
			Atean_mode[1] = n;	
		}
		n++;
	}
	n = 1;
	k = 1;


	while (n <= 4 && A_div == 0){
		if (n != Atean_mode[1]){
			A_watch[k] = dist(A[n].X, A[n].Y, -500, -500);
			k++;
		}
		n++;
	}
	n = 1;
	while (n <= 4 && A_div == 0){
		if (n != Atean_mode[1]){
			if (fmin(fmin(A_watch[1], A_watch[2]), A_watch[3]) == dist(A[n].X, A[n].Y, -500, -500)){
				A[n].mode = 3;
				Atean_mode[3] = n;
			}
		}
		n++;
	}
	n = 1;
	k = 1;


	while (n <= 4 && A_div == 0){
		if (n != Atean_mode[1] && n != Atean_mode[3]){
			A_watch[k] = dist(A[n].X, A[n].Y, 500, 100);
			k++;
		}
		n++;
	}
	n = 1;
	while (n <= 4 && A_div == 0){
		if (n != Atean_mode[1] && n != Atean_mode[3]){
			if (fmin(fmin(A_watch[1], A_watch[2]), A_watch[3]) == dist(A[n].X, A[n].Y, 500, 100)){
				A[n].mode = 2;
				Atean_mode[2] = n;
			}
		}
		n++;
	}
	n = 1;
	while (n <= 4&&A_div==0){
		if ((n != Atean_mode[1] && n != Atean_mode[2]) && n != Atean_mode[3]){
			A[n].mode = 4;
			Atean_mode[4] = n;
		}
		n++;
	}
	A_div = 1;*/
	

	
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
		goal();

		n = 1;
		
			A_strategy();
			B_strategy();
		//A[1] = oneman(A[1]);
		/*while (n <= PLAYER){
			if (A[n].mode == 1){
				//printf("%d\n", A[n].cnd);
				//A[n] = oneman(A[n]);
				if ((A[n].cnd != 4 || A[n].cnd != 2) && A[n].cnd != 3){
					A[n] = find(A[n]);
				}
				if (A[n].cnd == 1){
					A[n] = get(A[n]);
				}
				if (A[n].cnd == 2){
					A[n] = pass(A[n], A[Atean_mode[4]]);
					if (A[n].re == 1){
						//A[n].cnd = 3;
						A[n].re = 0;
					}
				}
				if (A[n].cnd == 3){
					A[n].vang = 0;
					A[n].v = 0;
				}

			}
			else if (A[n].mode == 2){
				//printf("%d", A[n].cnd);
				//printf("%d\n", A[n].have);
				//A[n]=turn_P(A[n], A[3 - n]);
				//if (A[n].cnd == 0){
				if (A[n].cnd == 0){
					A[n] = move(A[n], 500, 100);
					//printf("%d\n", A[n].re);
					if (A[n].re == 1){
						A[n].cnd = 1;
						A[n].re = 0;
						//printf("OK!\n");
					}
				}

				else if (A[n].cnd == 1){
					A[n] = turn_P(A[n], A[Atean_mode[4]]);
					if (A[n].have == 1){
						A[n].cnd = 2;
						A[n].re = 0;
					}
				}

				else if (A[n].cnd == 2 && A[n].have == 1){

					A[n] = turn_P(A[n], A[Atean_mode[3]]);
					if (A[n].re == 1){
						//glutIdleFunc(NULL);
						A[n].cnd = 3;
						A[n].re = 0;
					}
				}

				else if (A[n].cnd == 3 && A[n].have == 1){
					A[n] = pass(A[n], A[Atean_mode[3]]);
				}


			}
			else if (A[n].mode == 3){
				//printf("%d", A[n].cnd);
				//printf("%d\n", A[n].have);
				//A[n]=turn_P(A[n], A[3 - n]);
				//if (A[n].cnd == 0){
				if (A[n].cnd == 0){
					A[n] = move(A[n], -500, -500);
					//printf("%d\n", A[n].re);
					if (A[n].re == 1){
						A[n].cnd = 1;
						A[n].re = 0;
						//printf("OK!\n");
					}
				}

				else if (A[n].cnd == 1){
					A[n] = turn_P(A[n], A[Atean_mode[2]]);
					if (A[n].have == 1){
						A[n].cnd = 2;
						A[n].re = 0;
					}
				}

				else if (A[n].cnd == 2 && A[n].have == 1){

					A[n] = turn_G(A[n]);
					if (A[n].re == 1){
						//glutIdleFunc(NULL);
						A[n].cnd = 3;
						A[n].re = 0;
					}
				}

				else if (A[n].cnd == 3 && A[n].have == 1){
					A[n] = shoot(A[n]);
				}
			}


			else if (A[n].mode == 4){
				if (A[n].cnd == 0){
					A[n] = move(A[n], -400, 600);
					if (A[n].re == 1){
						A[n].cnd = 1;
						A[n].re = 0;
					}
				}
				if (A[n].cnd == 1){
					A[n] = turn_P(A[n], A[Atean_mode[1]]);
					if (A[n].have == 1){
						A[n].cnd = 2;
						A[n].re = 0;
					}
				}
				if (A[n].cnd == 2 && A[n].have == 1){
					A[n] = turn_P(A[n], A[Atean_mode[2]]);
					if (A[n].re == 1){
						A[n].cnd = 3;
						A[n].re = 0;
					}
				}
				if (A[n].cnd == 3 && A[n].have == 1){
					A[n] = pass(A[n], A[Atean_mode[2]]);
				}
			}


			//printf("%d		\n", judge);
			//printf("%f	%f	%f	%f\n", A[1].X, A[1].Y, A[2].X, A[2].Y);
			//printf("%f,%f	%d\n", ball.vx, ball.vy,judge);







			//printf("%f,		%f,		\n", A[1].ang, atan2(A[2].Y - A[1].Y, A[2].X - A[1].X)*(180 / P));
			n++;
		}*/
		glutPostRedisplay();//再描画
	
}



void init(void)
{
	glClearColor(g_R, g_G, g_B, 0.0);			//ウィンドウ全体の色
	glShadeModel(GL_FLAT);					//濃淡
	A_init();
	B_init();
	
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
			while (n <= PLAYER){
				A[n].cnd = 0;
				A[n].mode = 0;
				A[n].re = 0;
				A[n].vang = 0;
				A[n].have = 0;

				B[n].cnd = 0;
				B[n].mode = 0;
				B[n].re = 0;
				B[n].vang = 0;
				B[n].have = 0;
				n++;
			}
			ball.cnd = 0;
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