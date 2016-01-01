#include"SampleB.h"
#include"R_Imai.h"
SampleB::SampleB(int side) :Team(side){
}

void SampleB::defence_init(){
	/*this->player[1].set(500, this->side * 200, -side * 90);
	this->player[2].set(-500, this->side * 200, -side * 90);
	this->player[3].set(0, this->side * 400, -side * 90);
	this->player[4].set(0, this->side * 800, -side * 90);*/
	int n = 1;
	this->player[1].x = -400;
	this->player[1].y = -500;
	this->player[1].ang = 90;
	this->player[2].x = 350;
	this->player[2].y = -200;
	this->player[2].ang = 90;
	this->player[3].x = -100;
	this->player[3].y = -450;
	this->player[3].ang = 90;
	this->player[4].x = 0;
	this->player[4].y = -800;
	this->player[4].ang = 90;


	while (n <= PLAYER){
		this->player[n].v = 0;
		this->player[n].vang = 0;
		this->player[n].cnd = 0;
		this->player[n].mode = 0;
		n = n + 1;
	}
}
void SampleB::lineover_init(){
	int side;
	if (ball.x > 0){
		side = 1;
	}
	else{
		side = -1;
	}

	int n = 1;
	while (n <= 4){
		this->player[n].cnd = 0;
		this->player[n].mode = 0;
		n++;
	}

	switch (ball.state)
	{
	case -3:
		//自分のチームのコーナーキック
		ball.x = side * 630;
		ball.y = 905;
		this->player[1].x = side * 670;
		this->player[1].y = 945;
		this->player[1].ang = -90 - side * 45;

		this->player[2].x = side * 400;
		this->player[2].y = 500;
		this->player[2].ang = -180 + atan2(A.player[2].y - ball.y, A.player[2].x - ball.x) * 180 / P;

		this->player[3].x = -side * 100;
		this->player[3].y = 550;
		this->player[3].ang = -180 + atan2(A.player[3].y - ball.y, A.player[3].x - ball.x) * 180 / P;

		this->player[4].x = side*(-400);
		this->player[4].y = 700;
		this->player[4].ang = -180 + atan2(A.player[4].y - ball.y, A.player[4].x - ball.x) * 180 / P;
		break;
	case -2:
		//自分のチームのゴールキック
		ball.x = 0;
		ball.y = -800;
		this->player[1].x = 0;
		this->player[1].y = -855;
		this->player[1].ang = 90;

		this->player[2].x = 300;
		this->player[2].y = -600;
		this->player[2].ang = -180 + atan2(A.player[2].y - ball.y, A.player[2].x - ball.x) * 180 / P;
		this->player[3].x = -300;
		this->player[3].y = -600;
		this->player[3].ang = -180 + atan2(A.player[3].y - ball.y, A.player[3].x - ball.x) * 180 / P;
		this->player[4].x = 0;
		this->player[4].y = 400;
		this->player[4].ang = -180 + atan2(A.player[4].y - ball.y, A.player[4].x - ball.x) * 180 / P;
		break;
	case -1:
		//自分のチームのスローイン
		this->player[1].x = ball.x + side * 55;
		this->player[1].y = ball.y;
		this->player[1].ang = A.player[1].ang = 90 + side * 90;
		if (ball.y < -800){//自ゴールに近い
			this->player[2].x = side * 800;
			this->player[2].y = ball.y + 300;
			this->player[2].ang = +180 + atan2(A.player[2].y - ball.y, A.player[2].x - ball.x) * 180 / P;
			this->player[3].x = -side * 300;
			this->player[3].y = ball.y + 150;
			this->player[3].ang = +180 + atan2(A.player[3].y - ball.y, A.player[3].x - ball.x) * 180 / P;
			this->player[4].x = 0;
			this->player[4].y = -800;
			this->player[4].ang = +180 + atan2(A.player[4].y - ball.y, A.player[4].x - ball.x) * 180 / P;

		}
		else if (ball.y > 500){//敵ゴールに近い
			this->player[2].x = side * 200;
			this->player[2].y = ball.y - 200;
			this->player[2].ang = 180 + atan2(A.player[2].y - ball.y, A.player[2].x - ball.x) * 180 / P;
			this->player[3].x = -side * 300;
			this->player[3].y = ball.y - 50;
			this->player[3].ang = 180 + atan2(A.player[3].y - ball.y, A.player[3].x - ball.x) * 180 / P;
			this->player[4].x = 0;
			this->player[4].y = -300;
			this->player[4].ang = 180 + atan2(A.player[4].y - ball.y, A.player[4].x - ball.x) * 180 / P;

		}
		else{//その他
			this->player[2].x = side * 400;
			this->player[2].y = ball.y - 100;
			this->player[2].ang = 180 + atan2(A.player[2].y - ball.y, A.player[2].x - ball.x) * 180 / P;
			this->player[3].x = side * 200;
			this->player[3].y = ball.y + 400;
			this->player[3].ang = 180 + atan2(A.player[3].y - ball.y, A.player[3].x - ball.x) * 180 / P;
			this->player[4].x = -side * 300;
			this->player[4].y = ball.y + 200;
			this->player[4].ang = 180 + atan2(A.player[4].y - ball.y, A.player[4].x - ball.x) * 180 / P;
		}

		break;
	case 1:
		this->player[1].x = side * 200;
		this->player[1].y = ball.y - 200;
		this->player[1].ang = 180 + atan2(A.player[2].y - ball.y, A.player[2].x - ball.x) * 180 / P;
		this->player[2].x = side * 200;
		this->player[2].y = ball.y - 200;
		this->player[2].ang = 180 + atan2(A.player[2].y - ball.y, A.player[2].x - ball.x) * 180 / P;
		this->player[3].x = -side * 300;
		this->player[3].y = ball.y - 50;
		this->player[3].ang = 180 + atan2(A.player[3].y - ball.y, A.player[3].x - ball.x) * 180 / P;
		this->player[4].x = 0;
		this->player[4].y = -300;
		this->player[4].ang = 180 + atan2(A.player[4].y - ball.y, A.player[4].x - ball.x) * 180 / P;
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

void SampleB::offence_init(){
	this->player[1].set(-50, this->side * 50, -side * 45);
	this->player[2].set(100, this->side * 100, -side * 135);
	this->player[3].set(-400, this->side * 45, -side * 90);
	this->player[4].set(0, this->side * 400, -side * 90);
}

void SampleB::keeper(){
	double B4_X = pow(10000 / (1 + pow((ball.y + 905) / ball.x, 2)), 0.5);
	if (ball.x < 0 && B4_X>0){
		B4_X = -B4_X;
	}
	if (this->player[4].cnd == 0){
		this->player[4].move(B4_X, ((ball.y + 905) / ball.x)*B4_X - 905);
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
		this->player[4].pass(this->player[1]);
	}
	if (dist(this->player[4].x, this->player[4].y, B4_X, ((ball.y + 905) / ball.x)*B4_X - 905) > 10 && this->player[4].have == 0 && this->player[4].cnd != 0){
		this->player[4].cnd = 0;
	}
}

void SampleB::click_init(){
	this->teamStrategy = 0;
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


void SampleB::strategy(){
	double B3_X = pow(240000 / (1 + pow((ball.y + 905) / ball.x, 2)), 0.5);
	if (ball.x < 0 && B3_X>0){
		B3_X = -B3_X;
	}

	if (this->teamStrategy == 0 || this->teamStrategy == 1){
		if (ball.have >= 0){
			this->teamStrategy = 0;
		}
		else if (ball.have < 0){
			this->teamStrategy = 1;
		}
	}
	switch (this->teamStrategy)
	{
	case 0:
	case 1:
		

		this->keeper();

		if (this->player[1].have == 0){
			this->player[1].move(ball.x, ball.y);
		}
		if (this->player[1].have == 1){
			this->player[1].v = 0;
			//this->player[1] = B_turn_G(this->player[1]);
			this->player[1].cnd = 1;
			if (this->player[1].re == 1){
				this->player[1].cnd = 1;
				this->player[1].re = 0;
			}
		}
		if (this->player[1].cnd == 1){
			//this->player[1] = shoot(this->player[1]);
			this->player[1].pass(this->player[2]);
		}
		if (this->player[1].re == 1){
			this->player[1].cnd = 0;
			this->player[1].re = 0;
		}

		if (this->player[2].cnd == 0){
			//this->player[2] = move(this->player[2], (A.player[Atean_mode[3]].x + A.player[Atean_mode[2]].x) / 2, (A.player[Atean_mode[3]].y + A.player[Atean_mode[2]].y) / 2);
			this->player[2].move(400, 600);
			if (this->player[2].re == 1){
				this->player[2].cnd = 1;
				this->player[2].re = 0;
			}
		}
		if (this->player[2].cnd == 1){
			this->player[2].turn_B();
			//this->player[2] = turn_P(this->player[2],this->player[1]);
			if (this->player[2].re == 1){
				this->player[2].re = 0;
				//this->player[2].cnd = 2;
			}
		}
		if (this->player[2].have == 1){
			//this->player[2] = pass(this->player[2], this->player[1]);
			this->player[2].turn_G();
			if (this->player[2].re == 1){
				this->player[2].cnd = 3;
				this->player[2].re = 0;
			}

			if (this->player[2].cnd == 3){
				this->player[2].shoot();
				//cout << "player2.have:" << this->player[2].have<<"\n";
				this->player[2].cnd = 1;
			}
		}
		if (this->player[2].have == 0 && this->player[2].cnd == 2){
			this->player[2].cnd = 0;
		}
		

		if (ball.y >= 0){
			if (this->player[3].cnd == 0){
				this->player[3].move(B3_X, ((ball.y + 905) / ball.x)*B3_X - 905);
				if (this->player[3].re == 1){
					this->player[3].cnd = 1;
					this->player[3].re = 0;
				}
			}
			if (this->player[3].cnd == 1){
				this->player[3].turn_B();
			}
			if (this->player[3].have == 1){
				this->player[3].cnd = 2;
			}
			if (this->player[3].cnd == 2){
				this->player[3].pass(this->player[1]);
			}
			if (dist(this->player[3].x, this->player[3].y, B3_X, ((ball.y + 905) / ball.x)*B3_X - 905) > 10 && this->player[3].have == 0 && this->player[3].cnd != 0){
				this->player[3].cnd = 0;
			}
		}
		else{
			if (this->player[3].have == 0){
				this->player[3].move(ball.x, ball.y);
			}
			if (this->player[3].have == 1){
				this->player[3].v = 0;
				this->player[3].turn((atan2(905 - this->player[3].y, -this->player[3].x) * 180 / P));
				if (this->player[3].re == 1){
					this->player[3].cnd = 1;
					this->player[3].re = 0;
				}
			}
			if (this->player[3].cnd == 1){
				this->player[3].pass(this->player[2]);
				if (this->player[3].re == 1){
					this->player[3].cnd = 2;
					this->player[3].re = 0;
				}
			}
		}
		break;
	}
}


