/*
	calc.cpp
*/
#include"AIsoccer.h"



double dist(double x1, double y1, double x2, double y2){//(x1,y1)‚Æ(x2,y2)“ñ“_ŠÔ‚Ì‹——£‚ğ‘ª‚éŠÖ”
	double re;
	re = (x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2);
	re = pow(re, 0.5);
	return re;
}


double calcAng(double x1, double y1, double x2, double y2){
	return 180 + atan2(y1 - y2, x1 - x2) * 180 / P;
}
double calcAng(Player player, double x, double y){
	return calcAng(player.x, player.y, x, y);
}
double calcAng(Player player1, Player player2){
	return calcAng(player1.x, player1.y, player2.x, player2.y);
}
double calcAng(Player player, Ball ball){
	return calcAng(player.x, player.y, ball.x, ball.y);
}