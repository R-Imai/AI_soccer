#include"AIsoccer.h"



double dist(double x1, double y1, double x2, double y2){//(x1,y1)��(x2,y2)��_�Ԃ̋����𑪂�֐�
	double re;
	re = (x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2);
	re = pow(re, 0.5);
	return re;
}