#include"AIsoccer.h"



double dist(double x1, double y1, double x2, double y2){//(x1,y1)と(x2,y2)二点間の距離を測る関数
	double re;
	re = (x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2);
	re = pow(re, 0.5);
	return re;
}