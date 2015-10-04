/*--------------------------------------------------------------------------------------------------------------
*	Name:		SampleA.cpp
*	Author:		R.Imai
*	Created:	2015 / 09 / 21
*	Last Date:	2015 / 09 / 27
*	Note:
*
*--------------------------------------------------------------------------------------------------------------*/
#include"SampleA.h"
#include"SampleB.h"

SampleA::SampleA(int side) :Team(side)
{
}

SampleA::~SampleA()
{
}

void SampleA::defence_init(){
	this->player[1].set(500, this->side * 200, -side*90);
	this->player[2].set(-500, this->side * 200, -side*90);
	this->player[3].set(0, this->side * 400, -side*90);
	this->player[4].set(0, this->side * 800, -side*90);
}

void SampleA::offence_init(){
	this->player[1].set(-50, this->side * 50, -side * 45);
	this->player[2].set(100, this->side * 100, -side * 135);
	this->player[3].set(-400, this->side * 45, -side * 90);
	this->player[4].set(0, this->side * 400, -side * 90);
}

void SampleA::lineover_init(){
	int side;
	if (ball.x > 0){
		side = 1;
	}
	else{
		side = -1;
	}

	int n = 1;
	this->stop();
	while (n <= 4){
		A.player[n].cnd = 0;
		A.player[n].mode = 0;
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
		ball.x = side * 630;
		A.player[1].x = ball.x + side * 55;
		A.player[1].y = ball.y;
		A.player[1].ang = 90 + side * 90;
		if (ball.y > 800){//自ゴールに近い
			A.player[2].x = side * 800;
			A.player[2].y = ball.y - 300;
			A.player[2].ang = -180 + atan2(A.player[2].y - ball.y, A.player[2].x - ball.x) * 180 / P;
			A.player[3].x = -side * 300;
			A.player[3].y = ball.y - 150;
			A.player[3].ang = -180 + atan2(A.player[3].y - ball.y, A.player[3].x - ball.x) * 180 / P;
			A.player[4].x = 0;
			A.player[4].y = 800;
			A.player[4].ang = -180 + atan2(A.player[4].y - ball.y, A.player[4].x - ball.x) * 180 / P;

		}
		else if (ball.y<-500){//敵ゴールに近い
			A.player[2].x = side * 200;
			A.player[2].y = ball.y + 200;
			A.player[2].ang = -180 + atan2(A.player[2].y - ball.y, A.player[2].x - ball.x) * 180 / P;
			A.player[3].x = -side * 300;
			A.player[3].y = ball.y + 50;
			A.player[3].ang = -180 + atan2(A.player[3].y - ball.y, A.player[3].x - ball.x) * 180 / P;
			A.player[4].x = 0;
			A.player[4].y = 300;
			A.player[4].ang = -180 + atan2(A.player[4].y - ball.y, A.player[4].x - ball.x) * 180 / P;

		}
		else{//その他
			A.player[2].x = side * 400;
			A.player[2].y = ball.y + 100;
			A.player[2].ang = -180 + atan2(A.player[2].y - ball.y, A.player[2].x - ball.x) * 180 / P;
			A.player[3].x = side * 200;
			A.player[3].y = ball.y - 400;
			A.player[3].ang = -180 + atan2(A.player[3].y - ball.y, A.player[3].x - ball.x) * 180 / P;
			A.player[4].x = -side * 300;
			A.player[4].y = ball.y - 200;
			A.player[4].ang = -180 + atan2(A.player[4].y - ball.y, A.player[4].x - ball.x) * 180 / P;
		}
		this->teamStrategy = 2;
		break;
	case 2:
		//自分のチームのゴールーキック
		ball.x = 0;
		ball.y = 800;
		A.player[4].x = 0;
		A.player[4].y = 855;
		A.player[4].ang = -90;

		A.player[2].x = 300;
		A.player[2].y = 600;
		A.player[2].ang = -180 + atan2(A.player[2].y - ball.y, A.player[2].x - ball.x) * 180 / P;
		A.player[3].x = -300;
		A.player[3].y = 600;
		A.player[3].ang = -180 + atan2(A.player[3].y - ball.y, A.player[3].x - ball.x) * 180 / P;
		A.player[1].x = 0;
		A.player[1].y = -400;
		A.player[1].ang = -180 + atan2(A.player[4].y - ball.y, A.player[4].x - ball.x) * 180 / P;
		A.player[1].cnd = 0;
		this->teamStrategy = 4;
		break;
	case 3:
		//自分のチームのコーナキック
		ball.x = side * 630;
		ball.y = -905;
		A.player[1].x = side * 670;
		A.player[1].y = -945;
		A.player[1].ang = 45;
		A.player[1].have = 1;

		A.player[2].x = side * 400;
		A.player[2].y = -500;
		A.player[2].ang = -180 + atan2(A.player[2].y - ball.y, A.player[2].x - ball.x) * 180 / P;

		A.player[3].x = -side * 100;
		A.player[3].y = -550;
		A.player[3].ang = -180 + atan2(A.player[3].y - ball.y, A.player[3].x - ball.x) * 180 / P;

		A.player[4].x = side*(-400);
		A.player[4].y = -700;
		A.player[4].ang = -180 + atan2(A.player[4].y - ball.y, A.player[4].x - ball.x) * 180 / P;
		this->teamStrategy = 3;
		break;
	}
	glutIdleFunc(NULL);
}

void SampleA::keeper(){
	double A4_X = pow(10000 / (1 + pow((ball.y - 905) / ball.x, 2)), 0.5);
	if (ball.x < 0 && A4_X>0){
		A4_X = -A4_X;
	}
	if (this->player[4].cnd == 0){
		this->player[4].move(A4_X, ((ball.y - 905) / ball.x)*A4_X + 905);
		if (this->player[4].re == 1){
			this->player[4].cnd = 1;
			this->player[4].re = 0;
		}
	}
	if (this->player[4].cnd == 1){
		this->player[4].turn_B();
	}
	if (this->player[4].have == 1){
		this->player[4].cnd = 2;
	}
	if (this->player[4].cnd == 2){
		this->player[4].pass(this->player[teamMode[1]]);
	}
	if (dist(this->player[4].x, this->player[4].y, A4_X, ((ball.y - 905) / ball.x)*A4_X + 905) > 10 && this->player[4].have == 0 && this->player[4].cnd != 0){
		this->player[4].cnd = 0;
	}
}

int SampleA::minplayer(){
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

void SampleA::divide(){
	double A_watch[4];//比較対象
	double B_dist[11];
	int k;

	for (int n = 1; n < PLAYER; n++){
		B_dist[n] = dist(this->player[n].x, this->player[n].y, ball.x, ball.y);	//ボールとの距離を測る
	}
	
	for (int n = 1; n < PLAYER; n++){
		if (fmin(fmin(B_dist[1], B_dist[2]), B_dist[3]) == B_dist[n]){	//人数が増えるときに書き直すところ
			this->player[n].mode = 1;		//ボールに一番近い人にmode1を振り分ける。
			teamMode[1] = n;
		}
	}
	k = 1;


	for (int n = 1; n < PLAYER; n++){
		if (n != teamMode[1]){
			A_watch[k] = dist(this->player[n].x, this->player[n].y, -500, -500);
			k++;
		}
	}
	
	for (int n = 1; n < PLAYER; n++){
		if (n != teamMode[1]){
			if (fmin(A_watch[1], A_watch[2]) == dist(this->player[n].x, this->player[n].y, -500, -500)){
				this->player[n].mode = 3;
				teamMode[3] = n;
			}
		}
	}
	
	k = 1;


	for (int n = 1; n < PLAYER; n++){
		if (n != teamMode[1] && n != teamMode[3]){
			A_watch[k] = dist(this->player[n].x, this->player[n].y, 500, 100);
			k++;
		}
	}
	for (int n = 1; n < PLAYER; n++){
		if (n != teamMode[1] && n != teamMode[3]){
			if (fmin(A_watch[1], A_watch[2]) == dist(this->player[n].x, this->player[n].y, 500, 100)){
				this->player[n].mode = 2;
				teamMode[2] = n;
			}
		}
	}
	
	div = 1;
}

int SampleA::getPos(Player play, char axis, double start, double end){
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

int enemyCount(char axis, double start, double end){
	int cnt = 0;
	switch (axis)
	{
	case 'x':
		for (int i = 1; i <= PLAYER; i++){
			if (start < AteamEnamy.player[i].x&&AteamEnamy.player[i].x < end){
				cnt++;
			}
		}
		break;
	case 'y':
		for (int i = 1; i <= PLAYER; i++){
			if (start < AteamEnamy.player[i].y&&AteamEnamy.player[i].y < end){
				cnt++;
			}
		}
		break;
	default:
		cout << "enemyCount error\n";
	}
	return cnt;
}


void SampleA::strategy(){
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
	if ((this->teamStrategy == 0 || this->teamStrategy == 1) && ball.have >= 0){
		this->teamStrategy = 0;
	}
	if (ball.have < 0){
		this->teamStrategy = 1;
	}
	for (int n = 1; n <= PLAYER; n++){
		B_dist[n] = dist(this->player[n].x, this->player[n].y, ball.x, ball.y);	//ボールとの距離を測る
	}


	this->keeper();

	switch (this->teamStrategy){
	case 0://攻撃
		if (sw != 0){
			divide();
			sw = 0;
		}
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

		for (int n = 1; n < PLAYER; n++){
			switch (this->player[n].mode){
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
						min_dist = fmin(min_dist, dist(this->player[n].x, this->player[n].y, AteamEnamy.player[i].x, AteamEnamy.player[i].y));
					}
					if (min_dist>RANGE){
						if (this->player[n].x != 0){
							side = this->player[n].x / fabs(this->player[n].x);
						}
						this->player[n].move(side * 400, -750);
						if (this->player[n].re == 1){
							this->player[n].cnd = 2;
							this->player[n].re = 0;
						}
					}
					else{
						this->player[teamMode[2]].cnd = 1;
						this->player[n].pass(this->player[teamMode[2]]);
						if (this->player[n].re == 1){
							this->player[n].cnd = 4;
							this->player[n].re = 0;
						}
					}
					break;
				case 2:
					this->player[teamMode[3]].cnd = 2;
					this->player[n].pass(this->player[teamMode[3]]);
					if (this->player[n].re == 1){
						this->player[n].cnd = 3;
						this->player[n].re = 0;
					}
					break;
				case 3:

					break;
				case 4:
					this->player[n].move(-getPos(this->player[teamMode[2]],'x',-216,216) * 434, this->player[teamMode[2]].y - 300);
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
							min_dist = fmin(min_dist, dist(this->player[n].x, this->player[n].y, AteamEnamy.player[i].x, AteamEnamy.player[i].y));
						}
						if (min_dist>300){
							if (this->player[n].x != 0){
								side = this->player[n].x / fabs(this->player[n].x);
							}
							this->player[n].move(side * 600, -700);
							if (this->player[n].re == 1){
								this->player[n].re = 0;
							}
						}
						else{
							this->player[teamMode[2]].cnd = 1;
							this->player[n].pass(this->player[teamMode[2]]);
							if (this->player[n].re == 1){
								this->player[n].cnd = 4;
								this->player[n].re = 0;
							}
						}

					}
					else{
						/*this->player[n]=turn_P(this->player[n], this->player[teamMode[3]]);
						if (this->player[n].re == 1){
						this->player[n].re = 0;
						this->player[n].cnd = 3;
						}*/
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

			case 2:
				switch (this->player[n].cnd){
				case 0:
					this->player[n].move(-getPos(this->player[teamMode[1]],'x',-216,216) * 434, this->player[teamMode[1]].y - 300);
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
							min_dist = fmin(min_dist, dist(this->player[n].x, this->player[n].y, AteamEnamy.player[i].x, AteamEnamy.player[i].y));
						}
						if (min_dist>RANGE){
							if (this->player[n].x != 0){
								side = this->player[n].x / fabs(this->player[n].x);
							}
							this->player[n].move(getPos(this->player[teamMode[2]],'x',-216,216) * 600, -700);
							if (this->player[n].re == 1){
								this->player[n].cnd = 3;
								this->player[n].re = 0;
							}
						}
						else{
							this->player[teamMode[1]].cnd = 5;
							this->player[n].pass(this->player[teamMode[1]]);
							if (this->player[n].re == 1){
								this->player[n].cnd = 0;
								this->player[n].re = 0;
							}
						}
					}
					else{
						/*this->player[n]=turn_P(this->player[n], this->player[teamMode[3]]);
						if (this->player[n].re == 1){
						this->player[n].re = 0;
						this->player[n].cnd = 3;
						}*/
						/*this->player[n] = turn_G(this->player[n]);
						if (this->player[n].re == 1){
						this->player[n].cnd = 3;
						this->player[n].re = 0;
						}*/
						this->player[teamMode[3]].cnd = 2;
						this->player[n].pass(this->player[teamMode[3]]);
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
				case 2:
					this->player[n].turn_B();
					if (this->player[n].re == 1){
						this->player[n].re = 0;
					}
					if (this->player[n].have == 1){
						this->player[n].cnd = 3;
					}
					break;

				case 3:

					/*if (this->player[n].y > -600){
					this->player[n] = move(this->player[n], (root - 2) * 600, -700);
					if (this->player[n].re == 1){
					this->player[n].re = 0;
					}
					}
					else{*/
					/*this->player[n]=turn_P(this->player[n], this->player[teamMode[3]]);
					if (this->player[n].re == 1){
					this->player[n].re = 0;
					this->player[n].cnd = 3;
					}*/
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
						this->player[n].cnd = 5;
					}
					break;
				}
				break;
			}

		}


		break;
	case 1://守備
		if (sw != 1){
			for (int n = 1; n <= PLAYER; n++){
				B_dist[n] = dist(this->player[n].x, this->player[n].y, ball.x, ball.y);	//ボールとの距離を測る
			}
			
			for (int n = 1; n <= PLAYER; n++){
				if (fmin(fmin(fmin(B_dist[1], B_dist[2]), B_dist[3]), B_dist[4]) == B_dist[n]){	//人数が増えるときに書き直すところ
					this->player[n].mode = 1;		//ボールに一番近い人にmode1を振り分ける。
					teamMode[1] = n;
				}
			}

			this->player[teamMode[1]].mode = 1;
			int n = 1;
			for (int i = 1; i < PLAYER; i++){
				this->player[n].cnd = 0;
				if (i != teamMode[1]){
					List[n] = i;
					n++;
				}
			}
			if (this->player[List[1]].y == fmax(this->player[List[1]].y, this->player[List[2]].y)){
				teamMode[2] = List[1];
				this->player[List[1]].mode = 2;
				teamMode[3] = List[2];
				this->player[teamMode[3]].mode = 3;
			}
			else{
				teamMode[2] = List[2];
				this->player[List[1]].mode = 2;
				teamMode[3] = List[1];
				this->player[teamMode[3]].mode = 3;
			}
			sw = 1;
		}

		for (int n = 1; n <= PLAYER; n++){
			switch (this->player[n].mode){
			case 1:
				this->player[n].move(ball.x, ball.y);
				if (this->player[n].re == 1){
					this->player[n].re = 0;
				}
				break;
			case 2:
				this->player[n].move(ball.x, ball.y);
				if (this->player[n].re == 1){
					this->player[n].re = 0;
				}
				break;
			case 3:
				this->player[n].move(-fabs(ball.x) / ball.x * 300, -400);
				if (this->player[n].re == 1){
					this->player[n].re = 0;
				}
				break;
			}
		}


		break;


	case 2://スローイン
		if (this->player[1].cnd == 0){
			this->player[1].pass(this->player[2]);
			if (this->player[1].re == 1){
				this->player[1].re = 0;
				this->player[1].cnd = 0;
			}
		}
		if (this->player[2].have == 1){
			switch (this->player[2].cnd){
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
			}
		}

		break;
	case 3://コーナーキック
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
		break;
	case 4://ゴールキック
		if (this->player[4].cnd == 0){
			this->player[4].pass(this->player[3]);
			if (this->player[4].re == 1){
				this->player[4].re = 0;
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
				this->player[3].pass(this->player[2]);
				if (this->player[3].re == 1){
					this->player[3].re = 0;
					this->player[3].cnd = 0;
				}
				break;
			}
		}
		this->player[2].turn_B();
		if (this->player[2].have == 1){
			switch (this->player[2].cnd){
			case 0:
				this->player[2].turn(this->player[4]);
				if (this->player[2].re == 1){
					this->player[2].re = 0;
					this->player[2].cnd = 1;
				}
				break;
			case 1:
				this->player[2].pass(this->player[4]);
				if (this->player[2].re == 1){
					this->player[2].re = 0;
					this->player[2].cnd = 0;
				}
				break;
			}
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
				this->player[1].shoot();
				if (this->player[1].re == 1){
					this->player[1].re = 0;
					this->player[1].cnd = 0;
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