/*--------------------------------------------------------------------------------------------------------------
*	Name:		Main.cpp
*	Author:		R.Imai
*	Created:	2015 / 09 / 25
*	Last Date:	2016 / 01 / 01
*	Note:
*
*--------------------------------------------------------------------------------------------------------------*/
#include"AIsoccer.h"
using namespace std;


Ball ball;
R_Imai A(1);
SampleB B(-1);

int score_A = 0, score_B = 0;

void display(void){
	glClear(GL_COLOR_BUFFER_BIT);

	Draw draw;

	//debug_disp();

	draw.player(A.player[1], 1.0, 0, 0);
	draw.player(A.player[2], 1.0, 0.3, 0);
	draw.player(A.player[3], 1.0, 0.6, 0);
	draw.player(A.player[4], 1.0, 0.9, 0);

	draw.player(B.player[1], 0, 0, 1.0);
	draw.player(B.player[2], 0, 0.3, 1.0);
	draw.player(B.player[3], 0, 0.6, 1.0);
	draw.player(B.player[4], 0, 0.9, 1.0);

	draw.Ball();



	glutSwapBuffers();
}




void simu(void)
{
	Ground ground;

	A.update();
	B.update();

	ball.update();

	
	ground.player_wall();
	ground.crash();
	//ground.goal();
	ground.wall();

	A.strategy();
	B.strategy();
	
	glutPostRedisplay();//�ĕ`��

}



void init(void)
{
	glClearColor(g_R, g_G, g_B, 0.0);			//�E�B���h�E�S�̂̐F
	glShadeModel(GL_FLAT);					//�Z�W
	A.offence_init();
	//A.defence_init();
	B.defence_init();

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

void mouse(int button, int state, int x, int y){		//(�{�^��,����������������,�}�E�X�|�C���^��x���W,�}�E�X�|�C���^��y���W)
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN){
			glutIdleFunc(simu);
			ball.x = (x - 300)*2.4;
			ball.y = -(y - 400)*2.4;
			ball.vx = 0;
			ball.vy = 0;
			ball.have = 0;
			A.click_init();
			B.click_init();
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

void keyboard(unsigned char key, int x, int y)			//(�����ꂽ�{�^��,�}�E�X�|�C���^��x���W,�}�E�X�|�C���^��y���W)
{
	if (key == '\x1b') exit(0);

	if (key == 's'){
		glutIdleFunc(simu);
	}
	if (key == 'a') glutIdleFunc(NULL);
}

int main(int argc, char *argv[])
{

	glutInit(&argc, argv);							//������
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600, 800);					//�E�B���h�E�̑傫��
	glutInitWindowPosition(700, 0);					//�E�B���h�E�̈ʒu
	glutCreateWindow(argv[0]);						//����
	init();
	glutReshapeFunc(reshape);							//�T�C�Y�ύX���̌Ăяo���֐�
	glutKeyboardFunc(keyboard);						//�L�[�{�[�h�֐��̎Q��
	glutMouseFunc(mouse);								//�}�E�X�֐��̎Q��
	glutDisplayFunc(display);							//�f�B�X�v���C�֐��̎Q��
	glutMainLoop();									//�������[�v
	return 0;
}



