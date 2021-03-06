/*--------------------------------------------------------------------------------------------------------------
*	Name:		R-Imai.cpp
*	Author:		R.Imai
*	Created:	2015 / 09 / 21
*	Last Date:	2016 / 01 / 17
*	Note:
*
*--------------------------------------------------------------------------------------------------------------*/
#include"R_Imai.h"
#include"SampleB.h"

R_Imai::R_Imai(int side) :Team(side)
{
	this->initChecker = -1;
	sw = 1;
}

R_Imai::~R_Imai()
{
}

void R_Imai::defence_init(){
	this->initChecker = -1;
	this->player[1].set(300, this->side * 400, -side*90);
	this->player[2].set(-500, this->side * 200, -side*90);
	this->player[3].set(-100, this->side * 400, -side*90);
	this->player[4].set(0, this->side * 800, -side*90);
	this->teamStrategy = 1;
}

void R_Imai::offence_init(){
	this->initChecker = -1;
	this->player[1].set(-50, this->side * 50, -side * 45);
	this->player[2].set(450, this->side * 100, -side * 135);
	this->player[3].set(-400, this->side * 45, -side * 90);
	this->player[4].set(0, this->side * 800, -side * 90);
	this->teamStrategy = 0;
}

void R_Imai::lineover_init(){
	int workSpase;
	int enemyMin;
	int enemyMax;
	int ballSide;
	if (ball.x > 0){
		ballSide = 1;
	}
	else{
		ballSide = -1;
	}

	int n = 1;
	this->stop();
	while (n <= 4){
		A.player[n].cnd = 0;
		A.player[n].mode = 0;
		n++;
	}

	cout << ball.state << "\n";
	switch (this->teamState(ball.state))
	{
	case -3:
		//相手チームのコーナーキック
		this->player[1].set(ballSide * 250, this->side * (Y_MAX - 125), 180 + atan2(A.player[1].y - this->side * Y_MAX, A.player[1].x - ballSide*X_MAX) * 180 / P);
		this->player[1].set(ballSide * 250, this->side * (Y_MAX - 125), 180 + atan2(A.player[1].y - this->side * Y_MAX, A.player[1].x - ballSide*X_MAX) * 180 / P);
		this->player[2].set(ballSide * 200, this->side * (Y_MAX - 250), 180 + atan2(A.player[2].y - this->side * Y_MAX, A.player[2].x - ballSide*X_MAX) * 180 / P);
		this->player[2].set(ballSide * 200, this->side * (Y_MAX - 250), 180 + atan2(A.player[2].y - this->side * Y_MAX, A.player[2].x - ballSide*X_MAX) * 180 / P);
		this->player[3].set(ballSide * 100, this->side * (Y_MAX - 350), 180 + atan2(A.player[3].y - this->side * Y_MAX, A.player[3].x - ballSide*X_MAX) * 180 / P);
		this->player[3].set(ballSide * 100, this->side * (Y_MAX - 350), 180 + atan2(A.player[3].y - this->side * Y_MAX, A.player[3].x - ballSide*X_MAX) * 180 / P);
		break;
	case -2:
		//相手チームのゴールキック
		enemyMax = 1;
		for (int i = 2; i <= PLAYER; i++){
			if (fmax(this->side*R_ImaiEnemy.player[enemyMax].y, this->side*R_ImaiEnemy.player[i].y) == R_ImaiEnemy.player[i].y){
				enemyMax = i;
			}
		};
		enemyMin = 1;
		for (int i = 2; i <= PLAYER; i++){
			if (fmin(this->side*R_ImaiEnemy.player[enemyMin].y, this->side*R_ImaiEnemy.player[i].y) == R_ImaiEnemy.player[i].y){
				enemyMin = i;
			}
		}
		workSpase = enemyMin;
		if (workSpase != 1){
			enemyMin = 1;
		}
		else{
			enemyMin = 2;
		}
		for (int i = 2; i <= PLAYER; i++){
			if (fmin(this->side*R_ImaiEnemy.player[enemyMin].y, this->side*R_ImaiEnemy.player[i].y) == R_ImaiEnemy.player[i].y && i != workSpase){
				enemyMin = i;
			}
		}
		this->player[1].set((ball.x + R_ImaiEnemy.player[enemyMax].x) / 2, (ball.y + R_ImaiEnemy.player[enemyMax].y) / 2, 180 + atan2(A.player[1].y - ball.y, A.player[1].x - ball.x) * 180 / P);
		this->player[1].set((ball.x + R_ImaiEnemy.player[enemyMax].x) / 2, (ball.y + R_ImaiEnemy.player[enemyMax].y) / 2, 180 + atan2(A.player[1].y - ball.y, A.player[1].x - ball.x) * 180 / P);
		this->player[2].set((ball.x + R_ImaiEnemy.player[enemyMin].x) / 2, (ball.y + R_ImaiEnemy.player[enemyMin].y) / 2, 180 + atan2(A.player[2].y - ball.y, A.player[2].x - ball.x) * 180 / P);
		this->player[2].set((ball.x + R_ImaiEnemy.player[enemyMin].x) / 2, (ball.y + R_ImaiEnemy.player[enemyMin].y) / 2, 180 + atan2(A.player[2].y - ball.y, A.player[2].x - ball.x) * 180 / P);
		this->player[3].set(-this->player[2].x, this->player[2].y, 180 + atan2(A.player[3].y - ball.y, A.player[3].x - ball.x) * 180 / P);
		this->player[3].set(-this->player[2].x, this->player[2].y, 180 + atan2(A.player[3].y - ball.y, A.player[3].x - ball.x) * 180 / P);
		break;
	case -1:
		//相手チームのスローイン
		if (this->side*ball.y > Y_MAX - 250){
			this->player[1].set(ballSide * 250, this->side * (Y_MAX - 125), 180 + atan2(A.player[1].y - this->side * Y_MAX, A.player[1].x - ballSide*X_MAX) * 180 / P);
			this->player[1].set(ballSide * 250, this->side * (Y_MAX - 125), 180 + atan2(A.player[1].y - this->side * Y_MAX, A.player[1].x - ballSide*X_MAX) * 180 / P);
			this->player[2].set(ballSide * 200, this->side * (Y_MAX - 250), 180 + atan2(A.player[2].y - this->side * Y_MAX, A.player[2].x - ballSide*X_MAX) * 180 / P);
			this->player[2].set(ballSide * 200, this->side * (Y_MAX - 250), 180 + atan2(A.player[2].y - this->side * Y_MAX, A.player[2].x - ballSide*X_MAX) * 180 / P);
			this->player[3].set(ballSide * 100, this->side * (Y_MAX - 350), 180 + atan2(A.player[3].y - this->side * Y_MAX, A.player[3].x - ballSide*X_MAX) * 180 / P);
			this->player[3].set(ballSide * 100, this->side * (Y_MAX - 350), 180 + atan2(A.player[3].y - this->side * Y_MAX, A.player[3].x - ballSide*X_MAX) * 180 / P);
		}
		else if (this->side*ball.y > 0){
			this->player[1].set(-200, ball.y + this->side * 150, calcAng(this->player[1], ball));
			this->player[1].set(-200, ball.y + this->side * 150, calcAng(this->player[1], ball));
			this->player[2].set(ballSide * 400, ball.y + this->side * 100, calcAng(this->player[2], ball));
			this->player[2].set(ballSide * 400, ball.y + this->side * 100, calcAng(this->player[2], ball));
			this->player[3].set(200, ball.y + this->side * 150, calcAng(this->player[3], ball));
			this->player[3].set(200, ball.y + this->side * 150, calcAng(this->player[3], ball));
		}
		else if (this->side*ball.y < 0){
			this->player[1].set(-200, this->side * 150, calcAng(this->player[1], ball));
			this->player[1].set(-200, this->side * 150, calcAng(this->player[1], ball));
			this->player[2].set(ballSide * 400, ball.y + this->side * 250, calcAng(this->player[2], ball));
			this->player[2].set(ballSide * 400, ball.y + this->side * 250, calcAng(this->player[2], ball));
			this->player[3].set(200, this->side * 150, calcAng(this->player[3], ball));
			this->player[3].set(200, this->side * 150, calcAng(this->player[3], ball));
		}
		break;
	case 1:
		//自分のチームのスローイン
		ball.x = ballSide * 630;
		this->player[1].set(ball.x + ballSide * 55, ball.y, 90 + ballSide * 90);
		
		if (ball.y > 800){//自ゴールに近い
			this->player[2].set(ballSide * 300, ball.y - this->side * 500, -atan2(A.player[2].y - ball.y, A.player[2].x - ball.x) * 180 / P);
			this->player[2].set(ballSide * 300, ball.y - this->side * 500, -atan2(A.player[2].y - ball.y, A.player[2].x - ball.x) * 180 / P);
			this->player[3].set(-ballSide * 300, ball.y - this->side * 150, -180 + atan2(A.player[3].y - ball.y, A.player[3].x - ball.x) * 180 / P);
			this->player[3].set(-ballSide * 300, ball.y - this->side * 150, -180 + atan2(A.player[3].y - ball.y, A.player[3].x - ball.x) * 180 / P);
			//this->player[4].set(0, 800, -180 + atan2(A.player[4].y - ball.y, A.player[4].x - ball.x) * 180 / P);
			cout << "1\n";
		}

		else if (ball.y<-500){//敵ゴールに近い
			this->player[2].set(ballSide * 200, ball.y + this->side * 200, -180 + atan2(A.player[2].y - ball.y, A.player[2].x - ball.x) * 180 / P);
			this->player[2].set(ballSide * 200, ball.y + this->side * 200, -180 + atan2(A.player[2].y - ball.y, A.player[2].x - ball.x) * 180 / P);
			this->player[3].set(-ballSide * 300, ball.y + this->side * 50, -180 + atan2(A.player[3].y - ball.y, A.player[3].x - ball.x) * 180 / P);
			//this->player[4].set(0, 300, -180 + atan2(A.player[4].y - ball.y, A.player[4].x - ball.x) * 180 / P);
			cout << "2\n";
		}
		else{//その他
			this->player[2].set(-ballSide * 200, ball.y - this->side * 100, -180 + atan2(A.player[2].y - ball.y, A.player[2].x - ball.x) * 180 / P);
			this->player[2].set(-ballSide * 200, ball.y - this->side * 100, -180 + atan2(A.player[2].y - ball.y, A.player[2].x - ball.x) * 180 / P);
			this->player[3].set(ballSide * 200, ball.y - this->side * 400, -180 + atan2(A.player[3].y - ball.y, A.player[3].x - ball.x) * 180 / P);
			//this->player[4].set(-ballSide * 300, ball.y - this->side * 200, -180 + atan2(A.player[4].y - ball.y, A.player[4].x - ball.x) * 180 / P);
		}
		this->teamStrategy = 2;
		break;
	case 2:
		//自分のチームのゴールーキック
		ball.x = 0;
		ball.y = 800;
		this->player[4].set(0, this->side*855, -90);
		this->player[3].set(-300, this->side * 500, -180 + atan2(A.player[3].y - ball.y, A.player[3].x - ball.x) * 180 / P);
		this->player[3].set(-300, this->side * 500, -180 + atan2(A.player[3].y - ball.y, A.player[3].x - ball.x) * 180 / P);
		this->player[2].set(500, this->side * 400, -180 + atan2(A.player[2].y - ball.y, A.player[2].x - ball.x) * 180 / P);
		this->player[2].set(500, this->side * 400, -180 + atan2(A.player[2].y - ball.y, A.player[2].x - ball.x) * 180 / P);
		this->player[1].set(-400, -this->side * 600, -180 + atan2(A.player[4].y - ball.y, A.player[4].x - ball.x) * 180 / P);
		this->player[1].set(-400, -this->side * 600, -180 + atan2(A.player[4].y - ball.y, A.player[4].x - ball.x) * 180 / P);

		this->teamStrategy = 4;
		break;
	case 3:
		//自分のチームのコーナキック
		ball.x = ballSide * 630;
		ball.y = -905;
		this->player[1].set(ballSide * 670, -this->side * 945,this->side * 90 + this->side * ballSide * 45);
		this->player[1].have = 1;

		this->player[2].set(ballSide * 400, -this->side * 500, -180 + atan2(A.player[2].y - ball.y, A.player[2].x - ball.x) * 180 / P);
		this->player[2].set(ballSide * 400, -this->side * 500, -180 + atan2(A.player[2].y - ball.y, A.player[2].x - ball.x) * 180 / P);
		this->player[3].set(-ballSide * 200, -this->side * 550, -180 + atan2(A.player[3].y - ball.y, A.player[3].x - ball.x) * 180 / P);
		this->player[3].set(-ballSide * 200, -this->side * 550, -180 + atan2(A.player[3].y - ball.y, A.player[3].x - ball.x) * 180 / P);
		//this->player[4].set(-ballSide*-400, -this->side * 700, -180 + atan2(A.player[4].y - ball.y, A.player[4].x - ball.x) * 180 / P);

		this->teamStrategy = 3;
		break;
	}
	glutIdleFunc(NULL);
}

void R_Imai::click_init(){
	this->div = 0;
	this->teamStrategy = 0;
	this->initChecker = -1;
	for (int n = 0; n <= PLAYER; n++){
		this->player[n].cnd = 0;
		this->player[n].mode = 0;
		this->player[n].re = 0;
		this->player[n].vang = 0;
		this->player[n].have = 0;
		this->player[n].vx = 0;
		this->player[n].vy = 0;
	}
}

void R_Imai::keeper(){
	double A4_X = pow(10000 / (1 + pow((ball.y - 905) / ball.x, 2)), 0.5);

	if (ball.x < 0 && A4_X>0){
		A4_X = -A4_X;
	}
	if (this->player[4].re == 0){
		this->player[4].move(A4_X, ((ball.y - 905) / ball.x)*A4_X + 905);
		if (this->player[4].re == 1){
			this->player[4].re = 5;
		}
	}
	if (this->player[4].re == 5){
		this->player[4].turn_B();
		if (this->player[4].re == 1){
			this->player[4].re = 5;
		}
	}
	if (dist(this->player[4].x, this->player[4].y, A4_X, ((ball.y - 905) / ball.x)*A4_X + 905) > 10 && this->player[4].have == 0){
		this->player[4].re = 0;
	}
}

int R_Imai::minplayer(){
	int re;
	double B_dist[PLAYER];
	for (int n = 1; n <= PLAYER;n++){
		B_dist[n] = dist(this->player[n].x, this->player[n].y, ball.x, ball.y);	//ボールとの距離を測る
	}
	for (int n = 1; n <= PLAYER; n++){
		if (fmin(fmin(fmin(B_dist[1], B_dist[2]), B_dist[3]), B_dist[4]) == B_dist[n]){	//人数が増えるときに書き直すところ
			this->player[n].mode = 1;		//ボールに一番近い人にmode1を振り分ける。
			re = n;
		}
	}
	return re;
}

#define OFFENCE1 teamMode[1]
#define OFFENCE2 teamMode[2]

void R_Imai::divide(int mode){
	double A_watch[4];//比較対象
	double B_dist[11];
	int k;
	this->player[4].mode = 4;		//4はキーパー
	teamMode[4] = 4;

	for (int n = 1; n < PLAYER; n++){
		B_dist[n] = dist(this->player[n].x, this->player[n].y, ball.x, ball.y);	//ボールとの距離を測る
	}

	switch(mode){
	case 0:
		/**
		*	攻撃時のモード:
		*		 1:ボールに一番近い人
		*		 2:パスを受ける先の人
		*		 3:相手ゴールに一番近い人
		*		 4:キーパー
		*/
		for (int n = 1; n < PLAYER; n++){
			if (fmin(fmin(B_dist[1], B_dist[2]), B_dist[3]) == B_dist[n]){	//人数が増えるときに書き直すところ
				this->player[n].mode = 1;		//ボールに一番近い人にmode1を振り分ける。
				teamMode[1] = n;
			}
		}
		k = 1;


		for (int n = 1; n < PLAYER; n++){
			if (n != teamMode[1]){
				A_watch[k] = -this->player[n].y*this->side;//dist(this->player[n].x, this->player[n].y, -500, -500);
				k++;
			}
		}

		for (int n = 1; n < PLAYER; n++){
			if (n != teamMode[1]){
				if (fmax(A_watch[1], A_watch[2]) == -this->player[n].y*this->side){
					this->player[n].mode = 3;
					teamMode[3] = n;
				}
			}
		}

		k = 1;


		for (int n = 1; n < PLAYER; n++){
			if (n != teamMode[1] && n != teamMode[3]){
				this->player[n].mode = 2;
				teamMode[2] = n;
				break;
			}
		}
		div = 1;
		break;

	case 1:
		/**
		*	守備時のモード
		*		1:	自陣に一番近い人 
		*		2:	ボールに一番近い人
		*		3:	
		*/
		for (int i = 1; i < PLAYER; i++){
			if (fmax(fmax(this->side*this->player[1].y, this->side*this->player[2].y), this->side*this->player[3].y) == this->side*this->player[i].y){
				this->player[i].mode = 1;
				teamMode[1] = i;
			}
		}
		k = 1;
		for (int i = 1; i < PLAYER; i++){
			if (i != teamMode[1]){
				A_watch[k] = B_dist[i];
				k++;
			}
		}

		for (int i = 1; i < PLAYER; i++){
			if (fmin(A_watch[1], A_watch[2]) == B_dist[i]){
				this->player[i].mode = 2;
				teamMode[2] = i;
			}
		}

		for (int i = 1; i < PLAYER; i++){
			if (i != teamMode[1] && i != teamMode[2]){
				this->player[i].mode = 3;
				teamMode[3] = i;
			}
		}

		break;
	}
}

int R_Imai::getPos(Player play, char axis, double start, double end){
	switch (axis)
	{
	case 'x':
		if (play.x < start){
			return -1;
		}
		else if (start <= play.x&&play.x < end){
			return 0;
		}
		else if (end <= play.x){
			return 1;
		}
	case 'y':
		if (play.y < start){
			return -1;
		}
		else if (start <= play.y&&play.y < end){
			return 0;
		}
		else if (start <= play.y){
			return 1;
		}
	}
}

int R_Imai::enemyCount(char axis, double start, double end){
	int cnt = 0;
	switch (axis)
	{
	case 'x':
		for (int i = 1; i <= PLAYER; i++){
			if (start < R_ImaiEnemy.player[i].x&&R_ImaiEnemy.player[i].x < end){
				cnt++;
			}
		}
		break;
	case 'y':
		for (int i = 1; i <= PLAYER; i++){
			if (start < R_ImaiEnemy.player[i].y&&R_ImaiEnemy.player[i].y < end){
				cnt++;
			}
		}
		break;
	}
	return cnt;
}


bool R_Imai::checkPass(Player passer, Player getter){
	int direc = 0, re = 1;
	double ang = atan2(getter.y - passer.y, getter.x - passer.x)*(180 / P);
	double TAN = tan(ang*(P / 180));
	double sec = -TAN*passer.x + passer.y;
	double a = 1 + (TAN*TAN), b = -1, c = -passer.x*TAN + passer.y, d;
	double range = 80;
	if (fabs(ang) < 90){
		direc = 1;
	}
	else{
		direc = -1;
	}

	//printf("%f\t%d\n", ang, direc);
	for (int i = 1; i <= PLAYER; i++){
		d = fabs(TAN*R_ImaiEnemy.player[i].x + b*R_ImaiEnemy.player[i].y + c) / sqrt(TAN*TAN + b*b);
		if (d <= range && direc*passer.x < direc*R_ImaiEnemy.player[i].x){
			re = 0;
		}
	}
	return re;
}


bool R_Imai::checkPass(Player passer,double x, double y){
	int direc = 0, re = 1;
	double ang = atan2(y - passer.y, x - passer.x)*(180 / P);
	double TAN = tan(ang*(P / 180));
	double sec = -TAN*passer.x + passer.y;
	double a = 1 + (TAN*TAN), b = -1, c = -passer.x*TAN + passer.y, d;
	double range = 80;
	if (fabs(ang) < 90){
		direc = 1;
	}
	else{
		direc = -1;
	}

	//printf("%f\t%d\n", ang, direc);
	for (int i = 1; i <= PLAYER; i++){
		d = fabs(TAN*R_ImaiEnemy.player[i].x + b*R_ImaiEnemy.player[i].y + c) / sqrt(TAN*TAN + b*b);
		if (d <= range && direc*passer.x < direc*R_ImaiEnemy.player[i].x){
			re = 0;
		}
	}
	return re;
}

void R_Imai::debugStrategy(){
	if (this->player[3].have == 0){
		this->player[3].move(ball.x, ball.y);
	}
	if (this->player[3].have == 1){
		this->player[3].v = 0;
		this->player[3].cnd = 1;
		if (this->player[3].re == 1){
			this->player[3].cnd = 1;
			this->player[3].re = 0;
		}
	}
	if (this->player[3].cnd == 1){
		this->player[3].shoot();
	}

}


void R_Imai::strategy(){
	double B_dist[PLAYER + 1];
	double ang;
	int coner, coner_passer;
	int Ycnt[3] = { 0, 0, 0 };
	int root;
	int ballSide;
	int List[PLAYER];
	int k;
	int enemyPoint;
	double workSpace;
	double min_dist;
	double judge_max, judge_min;
	double A_pos[PLAYER + 1];
	double A_watch[4];
	double pointY;
	int passMode[] = { 0, 5, 1 };

	//cout << "player4.cmd:" << A.player[4].cnd<<"\n";
	if ((this->teamStrategy == 0 || this->teamStrategy == 1) && ball.have >= 0){
		this->teamStrategy = 0;
	}
	if (ball.have < 0 && this->teamStrategy < 2){
		this->teamStrategy = 1;
	}
	for (int n = 1; n <= PLAYER; n++){
		B_dist[n] = dist(this->player[n].x, this->player[n].y, ball.x, ball.y);	//ボールとの距離を測る
	}

	if (this->player[4].have == 0){
		this->keeper();
	}
	//cout << "teamstrategy is " << this->teamStrategy << "\n";
	switch (this->teamStrategy){
	case 0://攻撃
		if (this->initChecker != 0){
			for (int i = 0; i < PLAYER; i++){
				this->player[i].cnd = 0;
			}
			divide(0);
			sw = 0;
		}
		//cout << this->player[1].cnd << "\t" << this->player[2].cnd << "\t" << this->player[3].cnd << "\t" << this->player[4].cnd << "\n";
			Ycnt[0] = 0;//enemyCount('x', -650.0, -216.0);
			Ycnt[1] = 1;//enemyCount('x', -216.0, 216.0);
			Ycnt[2] = 0;//enemyCount('x', 216.0, 650.0);

		if (Ycnt[0] == fmin(fmin(Ycnt[0], double(Ycnt[1])), Ycnt[2])){
			root = 1;
		}
		else if (Ycnt[1] == fmin(fmin(double(Ycnt[0]), Ycnt[1]), Ycnt[2])){
			root = 2;
		}
		else if (Ycnt[2] == fmin(fmin(Ycnt[0], double(Ycnt[1])), Ycnt[2])){
			root = 3;
		}

		for (int n = 1; n <= PLAYER; n++){
			switch (this->player[n].mode){
			/**
			*	攻撃の主軸プレイヤー
			*	- ボールを取りに行く
			*
			*/
			case 1:
				switch (this->player[n].cnd){
				case 0:

					this->player[n].get();
					if (this->player[n].re == 1){
						this->player[n].cnd = 1;
						this->player[n].re = 0;
					}
					break;
				case 1:
					min_dist = 10000.0;
					for (int i = 1; i < PLAYER; i++){
						min_dist = fmin(min_dist, dist(this->player[n].x, this->player[n].y, R_ImaiEnemy.player[i].x, R_ImaiEnemy.player[i].y));
					}
					if (min_dist>RANGE){
						if (this->player[n].x != 0){
							ballSide = this->player[n].x / fabs(this->player[n].x);
						}
						this->player[n].move(ballSide * 400, -750);
						if (this->player[n].re == 1){
							this->player[n].cnd = 2;
							this->player[n].re = 0;
						}
					}
					else{
						//cout << checkPass(this->player[n], this->player[teamMode[2]]) << "\n";
						if (checkPass(this->player[n], this->player[teamMode[2]])){
							this->player[teamMode[2]].cnd = 1;
							this->player[n].pass(this->player[teamMode[2]]);
						}
						else if (checkPass(this->player[n], this->player[4])){
							this->player[4].cnd = 2;
							this->player[n].pass(this->player[4]);
						}
						if (this->player[n].re == 1){
							this->player[n].cnd = 4;
							this->player[n].re = 0;
						}
					}
					break;
				case 2:
					if (this->player[teamMode[3]].cnd == 5){
						this->player[n].pass(this->player[teamMode[3]]);
					}
					this->player[teamMode[3]].cnd = 2;
					if (this->player[n].re == 1){
						this->player[n].cnd = 3;
						this->player[n].re = 0;
					}
					break;
				case 3:

					break;
				case 4:
					pointY = this->player[teamMode[2]].y - this->side * 300;
					if (-this->side * pointY > Y_MAX){
						pointY = -this->side*Y_MAX*0.9;
					}
					this->player[n].move(-getPos(this->player[teamMode[2]], 'x', -216, 216) * 520, pointY);
					if (this->player[n].re == 1){
						this->player[n].re = 0;
					}
					break;
				case 5:
					if (this->player[n].have == 1){
						this->player[n].cnd = 6;
					}
					this->player[n].turn_B();
					if (this->player[n].re == 1){
						this->player[n].re = 0;
					}

					break;
				case 6:
					if (this->player[n].y > -600){
						min_dist = 10000.0;
						for (int i = 1; i < PLAYER; i++){
							min_dist = fmin(min_dist, dist(this->player[n].x, this->player[n].y, R_ImaiEnemy.player[i].x, R_ImaiEnemy.player[i].y));
						}
						if (min_dist>300){
							if (this->player[n].x != 0){
								ballSide = this->player[n].x / fabs(this->player[n].x);
							}
							this->player[n].move(ballSide * 600, -700);
							if (this->player[n].re == 1){
								this->player[n].re = 0;
							}
						}
						else{
							if (checkPass(this->player[n], this->player[teamMode[2]])){
								this->player[teamMode[2]].cnd = 1;
								this->player[n].pass(this->player[teamMode[2]]);
							}
							else if (checkPass(this->player[n], this->player[4])){
								this->player[4].cnd = 2;
								this->player[n].pass(this->player[4]);
							}
							if (this->player[n].re == 1){
								this->player[n].cnd = 4;
								this->player[n].re = 0;
							}
						}

					}
					else{

						this->player[n].turn_G();
						if (this->player[n].re == 1){
							this->player[n].cnd = 2;
							this->player[n].re = 0;
						}
					}
					break;
				case 7:
					this->player[n].shoot();
					if (this->player[n].re == 1){
						this->player[n].re = 0;
						this->player[n].cnd = 9;
					}
					break;
				}
				break;

			/**
			*	攻撃の主軸プレイヤー2
			*	- 最初はパスを受ける
			*/
			case 2:
				switch (this->player[n].cnd){
				case 0:
					pointY = this->player[teamMode[1]].y - this->side * 300;
					if (-this->side * pointY > Y_MAX){
						pointY = -this->side*Y_MAX*0.9;
					}
					this->player[n].move(-getPos(this->player[teamMode[1]], 'x', -216, 216) * 520, pointY);
					if (this->player[n].re == 1){
						this->player[n].re = 0;
					}
					break;

				case 1:
					this->player[n].turn_B();
					if (this->player[n].re == 1){
						this->player[n].re = 0;
					}
					if (this->player[n].have == 1){
						this->player[n].cnd = 2;
					}
					break;

				case 2:

					if (this->player[n].y > -600){

						min_dist = 10000.0;
						for (int i = 1; i < PLAYER; i++){
							min_dist = fmin(min_dist, dist(this->player[n].x, this->player[n].y, R_ImaiEnemy.player[i].x, R_ImaiEnemy.player[i].y));
						}
						if (min_dist>RANGE){
							if (this->player[n].x != 0){
								ballSide = this->player[n].x / fabs(this->player[n].x);
							}
							this->player[n].move(getPos(this->player[teamMode[2]], 'x', -216, 216) * 600, -700);
							if (this->player[n].re == 1){
								this->player[n].cnd = 3;
								this->player[n].re = 0;
							}
						}
						else{
							if (checkPass(this->player[n], this->player[teamMode[1]])){
								this->player[teamMode[1]].cnd = 5;
								this->player[n].pass(this->player[teamMode[1]]);
							}
							else if (checkPass(this->player[n], this->player[4])){
								this->player[4].cnd = 1;
								this->player[n].pass(this->player[4]);
							}
							if (this->player[n].re == 1){
								this->player[n].cnd = 0;
								this->player[n].re = 0;
							}
						}
					}
					else{
						if (this->player[teamMode[3]].cnd == 5){
							this->player[n].pass(this->player[teamMode[3]]);
						}
						this->player[teamMode[3]].cnd = 2;
						if (this->player[n].re == 1){
							this->player[n].cnd = 4;
							this->player[n].re = 0;
						}
					}
					break;
				case 3:
					this->player[n].shoot();
					if (this->player[n].re == 1){
						this->player[n].re = 0;
						this->player[n].cnd = 4;
					}
					break;
				}
				break;

			/**
			*	シュートする人
			*	- ゴール前待機
			*/
			case 3:
				switch (this->player[n].cnd){
				case 0:
					this->player[n].move(-fabs(ball.x) / ball.x * r_g, -750);
					if (this->player[n].re == 1){
						this->player[n].re = 0;
						this->player[n].cnd = 1;
					}
					break;

				case 1:
					if (-fabs(this->player[n].x) / this->player[n].x == -fabs(ball.x) / ball.x){
						this->player[n].cnd = 0;
					}
					this->player[n].turn_B();
					if (this->player[n].re == 1){
						this->player[n].re = 0;
					}
					if (this->player[n].have == 1){
						this->player[n].cnd = 3;
					}
					break;
				case 2:
				case 5:
					//cout << fabs(-fabs(ball.x) / ball.x * this->player[n].x) <<"\t"<< fabs(-fabs(ball.x) / ball.x * r_g) << "\n";
					if (fabs(-fabs(ball.x) / ball.x * this->player[n].x) + 50 < fabs(-fabs(ball.x) / ball.x * r_g)){
						this->player[n].move(-fabs(ball.x) / ball.x * r_g, -750);
						if (this->player[n].re == 1){
							this->player[n].re = 0;
						}
					}
					else{
						this->player[n].cnd = 5;
						this->player[n].turn_B();
						if (this->player[n].re == 1){
							this->player[n].re = 0;
						}
					}
					if (this->player[n].have == 1){
						this->player[n].cnd = 3;
					}
					
					break;

				case 3:
					this->player[n].turn_G();
					if (this->player[n].re == 1){
						this->player[n].cnd = 4;
						this->player[n].re = 0;
						//}
					}
					break;
				case 4:
					this->player[n].shoot();
					if (this->player[n].re == 1){
						this->player[n].re = 0;
						this->player[n].cnd = 0;
					}
					break;
				}
				break;
			/**
			*	キーパー
			*	- パスの相手はcndに従う
			*/
			case 4:
				Player Kpasser = this->player[teamMode[this->player[4].cnd]];
				if (this->player[4].have == 1){
					//cout << "player4.have!" << this->player[4].cnd << "\n";
					if (checkPass(this->player[4], Kpasser)){
						this->player[teamMode[this->player[4].cnd]].cnd = passMode[this->player[4].cnd];
						this->player[4].pass(Kpasser);
						if (this->player[4].re == 1){
							this->player[4].re = 0;
							this->player[4].cnd = 2;
						}
					}
					else{
						this->player[4].move(Kpasser.x, this->player[4].y);
						if (this->player[4].re == 1){
							this->player[4].re = 0;
						}
					}
				}
				break;
			}
		}
		this->initChecker = 0;
		break;
	/**
	*	mode:
	*		1: ボールを追いかける
	*		2: ボールと自分のゴールに一番近い敵プレイヤーの間に行く
	*		3: 反撃準備(本当にやばくなったら守備に)
	*/
	case 1://守備
		if (this->initChecker != 1){
			this->divide(1);
			for (int i = 1; i < PLAYER; i++){
				this->player[i].cnd = 0;
			}
			this->player[4].cnd = this->teamMode[2];
		}
		for (int i = 1; i < PLAYER; i++){
			switch (this->player[i].mode){
			case 1:
				for (int g = 1; g <= PLAYER; g++){
				}
				for (int j = 1; j <= PLAYER; j++){
					if (fmax(fmax(this->side*R_ImaiEnemy.player[1].y, this->side*R_ImaiEnemy.player[2].y), fmax(this->side*R_ImaiEnemy.player[3].y, this->side*R_ImaiEnemy.player[4].y)) == R_ImaiEnemy.player[j].y){
						enemyPoint = j;
					}
				}
				switch (this->player[i].cnd){
				case 0:

					this->player[i].move((4*R_ImaiEnemy.player[enemyPoint].x + ball.x) / 5, (4*R_ImaiEnemy.player[enemyPoint].y + ball.y) / 5);
					if (this->player[i].re == 1){
						this->player[i].cnd = 1;
						this->player[i].re = 0;
					}
					break;
				case 1:
					this->player[i].turn_B();
					if (this->player[i].re == 1){
						this->player[i].re = 0;
					}
					if (dist((4*R_ImaiEnemy.player[enemyPoint].x + ball.x) / 5, (4*R_ImaiEnemy.player[enemyPoint].y + ball.y) / 5, this->player[i].x, this->player[i].y) > 30){
						this->player[i].cnd = 0;
					}
					break;
				}
				break;
			case 2:
				this->player[i].get();
				if (this->player[i].re == 1){
					this->player[i].re = 0;
				}
				break;
			case 3:
				this->player[i].get();
				if (this->player[i].re == 1){
					this->player[i].re = 0;
				}
				break;
			}
		}
		/*min_dist = dist(this->player[1].x, this->player[1].y, ball.x, ball.y);
		for (int i = 2; i < PLAYER; i++){
			min_dist = fmin(min_dist, dist(this->player[i].x, this->player[i].y, ball.x, ball.y));
		}
		for (int i = 1; i < PLAYER; i++){
			if (min_dist == dist(this->player[i].x, this->player[i].y, ball.x, ball.y)){
				this->player[i].mode = 1;
				teamMode[1] = i;
				break;
			}
		}
		k = 0;
		for (int i = 1; i < PLAYER; i++){
			if (i != teamMode[1]){
				A_watch[k] = this->player[i].y*this->side;
				k++;
			}
		}
		for (int i = 1; i < PLAYER; i++){
			if (i != teamMode[1]){
				if (fmax(A_watch[0], A_watch[1]) == -this->player[i].y*this->side){
					this->player[i].mode = 2;
					teamMode[2] = i;
				}
			}
		}
		for (int i = 1; i < PLAYER; i++){
			if (i != teamMode[1] && i != teamMode[2]){
				this->player[i].mode = 3;
				teamMode[3] = i;
			}
		}
		for (int i = 1; i < PLAYER; i++){
			switch (this->player[i].mode){
			case 1:
				this->player[i].get();
				
			case 2:
				workSpace = R_ImaiEnemy.player[1].y*this->side;
				for (int j = 2; j <= PLAYER; j++){
					workSpace = fmax(workSpace, R_ImaiEnemy.player[i].y*this->side);
				}
				for (int i = 1; i < PLAYER; i++){
					if (workSpace == R_ImaiEnemy.player[i].y*this->side){
						this->player[i].move((ball.x + 3 * R_ImaiEnemy.player[i].x) / 4, (ball.y + 3 * R_ImaiEnemy.player[i].y) / 4);
						break;
					}
				}
			}
		}*/
		this->initChecker = 1;
		break;


	case 2://スローイン
		if (this->initChecker != 2){
			for (int i = 0; i <= PLAYER; i++){
				this->player[i].cnd = 0;
			}
		}
		if (this->player[1].cnd == 0){
			this->player[1].pass(this->player[2]);
			if (this->player[1].re == 1){
				this->player[1].re = 0;
				this->player[1].cnd = 1;
			}
		}
		if (this->player[1].cnd == 1){
			this->player[1].go_G(0);
		}
		if (this->player[2].have == 1){
			this->teamStrategy = 0;
			/*switch (this->player[2].cnd){
			case 0:
				this->player[2].turn_G();
				if (this->player[2].re == 1){
					this->player[2].re = 0;
					this->player[2].cnd = 1;
				}
				break;
			case 1:
				this->player[2].shoot();
				if (this->player[2].re == 1){
					this->player[2].re = 0;
					this->player[2].cnd = 0;
				}
				break;
			}*/
		}
		this->initChecker = 2;
		break;
	case 3://コーナーキック
		if (this->initChecker != 3){
			for (int i = 0; i <= PLAYER; i++){
				this->player[i].cnd = 0;
			}
		}
		if (this->player[1].cnd == 0){
			this->player[1].pass(this->player[3]);
			if (this->player[1].re == 1){
				this->player[1].re = 0;
				this->player[1].cnd = 0;
			}
		}
		if (this->player[3].have == 1){
			switch (this->player[3].cnd){
			case 0:
				this->player[3].turn_G();
				if (this->player[3].re == 1){
					this->player[3].re = 0;
					this->player[3].cnd = 1;
				}
				break;
			case 1:
				this->player[3].shoot();
				if (this->player[3].re == 1){
					this->player[3].re = 0;
					this->player[3].cnd = 0;
				}
				break;
			}
		}
		this->initChecker = 3;
		break;
	case 4://ゴールキック
		if (this->initChecker != 4){
			for (int i = 1; i < PLAYER; i++){
				this->player[i].cnd = 0;
			}
		}
		//cout << this->player[1].cnd << "\t" << this->player[2].cnd << "\t" << this->player[3].cnd << "\t" << this->player[4].cnd << "\n";
		if (this->player[4].cnd == 0){
			if (this->checkPass(this->player[4], this->player[3])){
				this->player[4].pass(this->player[3]);
				if (this->player[4].re == 1){
					this->player[4].re = 0;
					//this->teamStrategy = 0;
				}
			}
			else if (this->checkPass(this->player[4], this->player[2])){
				this->player[4].pass(this->player[2]);
				if (this->player[4].re == 1){
					this->player[4].re = 0;
					//this->teamStrategy = 0;
				}
			}
			else{
				this->player[4].move(this->player[3].x, this->player[4].y);
			}
		}
		if (this->player[3].have == 1){
			switch (this->player[3].cnd){
			case 0:
				this->player[3].turn(this->player[2]);
				if (this->player[3].re == 1){
					this->player[3].re = 0;
					this->player[3].cnd = 1;
				}
				break;
			case 1:
				if (this->checkPass(this->player[3], this->player[2])){
					this->player[3].pass(this->player[2]);
					if (this->player[3].re == 1){
						this->player[3].re = 0;
						this->player[3].cnd = 2;
					}
				}
				else{
					this->teamStrategy = 0;
				}
				break;
			case 2:
				this->player[3].move(500, -this->side * 720);
				if (this->player[3].re == 1){
					this->player[3].re = 0;
				}
				break;
			case 3:
				this->player[3].cnd = 4;
				break;
			case 4:
				this->player[3].turn_G();
				if (this->player[3].re == 1){
					this->player[3].re = 0;
					this->player[3].cnd = 5;
				}
				break;
			case 5:
				if (this->checkPass(this->player[3], 0, -this->side*Y_MAX)){
					this->player[3].shoot();
					if (this->player[3].re == 1){
						this->player[3].re = 0;
					}
				}
				else if (this->checkPass(this->player[3], this->player[2])){
					this->player[3].pass(this->player[2]);
					if (this->player[3].re == 1){
						this->player[3].re = 0;
						this->player[3].cnd = 2;
					}
					this->player[2].cnd = 3;
				}
				else{
					this->teamStrategy = 0;
				}
				break;
			}
		}
		else if (this->player[3].cnd == 2){
			this->player[3].move(-500, -this->side * 720);
			if (this->player[3].re == 1){
				this->player[3].re = 0;
			}
		}
		else if (this->player[3].cnd == 3){
			this->player[3].turn_B();
			if (this->player[3].re == 1){
				this->player[3].re = 0;
			}
			if (this->player[3].have == 1){
				this->player[3].cnd = 4;
			}
		}
		if (this->player[2].have == 0 && this->player[2].cnd != 2){
			this->player[2].turn_B();
			if (this->player[2].re == 1){
				this->player[2].re = 0;
			}
		}
		if (this->player[2].have == 1){
			switch (this->player[2].cnd){
			case 0:
				this->player[2].turn(this->player[1]);
				if (this->player[2].re == 1){
					this->player[2].re = 0;
					this->player[2].cnd = 1;
				}
				break;
			case 1:
				if (this->checkPass(this->player[2], this->player[1])){
					this->player[2].pass(this->player[1]);
					if (this->player[2].re == 1){
						this->player[2].re = 0;
						this->player[2].cnd = 2;
					}
				}
				else{
					this->teamStrategy = 0;
				}
				break;
			case 2:
				this->player[2].move(500,-this->side*720);
				if (this->player[2].re == 1){
					this->player[2].re = 0;
				}
				break;
			case 3:
				this->player[2].cnd = 4;
				break;
			case 4:
				this->player[2].turn_G();
				if (this->player[2].re == 1){
					this->player[2].re = 0;
					this->player[2].cnd = 5;
				}
				break;
			case 5:
				if (this->checkPass(this->player[2], 0, -this->side*Y_MAX)){
					this->player[2].shoot();
					if (this->player[2].re == 1){
						this->player[2].re = 0;
						this->player[2].cnd = 2;
					}
				}
				else if (this->checkPass(this->player[2], this->player[3])){
					this->player[2].pass(this->player[3]);
					if (this->player[2].re == 1){
						this->player[2].re = 0;
						this->player[2].cnd = 2;
					}
					this->player[3].cnd = 3;
				}
				else{
					this->teamStrategy = 0;
				}
				break;
			}
		}
		else if (this->player[2].cnd == 2){
			this->player[2].move(500, -this->side * 720);
			if (this->player[2].re == 1){
				this->player[2].re = 0;
			}
		}
		else if (this->player[2].cnd == 3){
			this->player[2].turn_B();
			if (this->player[2].re == 1){
				this->player[2].re = 0;
			}
			if (this->player[2].have == 1){
				this->player[2].cnd = 4;
			}
		}
		this->player[1].turn_B();
		if (this->player[1].re == 1){
			this->player[1].re = 0;
		}
		if (this->player[1].have == 1){
			switch (this->player[1].cnd){
			case 0:
				this->player[1].turn_G();
				if (this->player[1].re == 1){
					this->player[1].re = 0;
					this->player[1].cnd = 1;
				}
				break;
			case 1:
				if (this->checkPass(this->player[1], 0, -this->side*Y_MAX)){
					this->player[1].shoot();
					if (this->player[1].re == 1){
						this->player[1].re = 0;
						this->player[1].cnd = 0;
					}
				}
				else if (checkPass(this->player[1], this->player[2])){
					this->player[1].pass(this->player[2]);
					this->player[2].cnd = 3;
				}
				else if (checkPass(this->player[1], this->player[3])){
					this->player[1].pass(this->player[3]);
					this->player[3].cnd = 3;
				}
				else {
					this->teamStrategy = 0;
				}
				break;
			}
		}
		this->initChecker = 4;
		break;
	case 5:
		break;
	case 6:
		break;
	}

}
