/*--------------------------------------------------------------------------------------------------------------
*	Name:		Draw.cpp
*	Author:		R.Imai
*	Created:	2015 / 09 / 26
*	Last Date:	2015 / 09 / 26
*	Note:
*
*--------------------------------------------------------------------------------------------------------------*/
#include"AIsoccer.h"

Draw::Draw(){
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

Draw::~Draw(){

}

void Draw::circle(double x, double y, double r){
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

void Draw::player(Player people,double r,double g,double b){
	double s = P / 18;
	int i;
	i = 0;

	glColor3f(r, g, b);		
	glPushMatrix();
	glTranslatef(people.x, people.y, 0);
	glRotatef(people.ang, 0, 0, 1.0);
	glBegin(GL_POLYGON);
	while (i < 31){
		glVertex2d(40 * cos(s*(i + 3)), 40 * sin(s*(i + 3)));
		i = i + 1;
	}
	glEnd();
	glPopMatrix();
}

void Draw::Ball(){
	glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslatef(ball.x, ball.y, 0);
	circle(0.0, 0.0, 20);
	glPopMatrix();
}
