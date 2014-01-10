#include"glmain.h"

/*
 *	main.cpp
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <iostream>

#include <gl/glut.h>
#include <gl\GL.h>

#include "../MarkerLessAR/MarkerLessAR.h"
#include"../MyLibs/OpenCVLibs/Image.h"
//#include "my_opencv.h"

using namespace std;

/*
 *	OpenGL�����������܂��D
 */
void initialize_opengl(int&, char* [], int, int);

/*
 *	���������������܂��D
 */
void initialize_light(void);

/*
 *	�ގ������������܂��D
 */
void initialize_material(void);

/*
 *	�E�B���h�E�`��֐��ł��D
 */
void display_function(void);

/*
 *	�E�B���h�E�T�C�Y�ύX�֐��ł��D
 */
void reshape_function(int, int);

/*
 *	�L�[���͊֐��ł��D
 */
void keyboard_function(unsigned char, int, int);

/*
 *	�}�E�X���͊֐��ł��D
 */
void mouse_function(int, int, int, int);

/*
 * �������_�[
 */
void render_axes(float);
void render_cuboid(double length_x,double length_y,double length_z);

// �J�����摜
cv::Mat camera_image;

// �J�����摜�̃R�s�[
cv::Mat copy_image;

// �}�[�J���XAR
MarkerLessAR glmlar;

// ���f���r���[�s��
GLdouble modelMat[4*4];

void setModelMat(double x1,double x2,double x3,
				 double y1,double y2,double y3,
				 double z1,double z2,double z3,
				 double t1,double t2,double t3){
	
	modelMat[0]=x1;
	modelMat[1]=x2;
	modelMat[2]=x3;
	modelMat[3]=0;

	modelMat[4]=y1;
	modelMat[5]=y2;
	modelMat[6]=y3;
	modelMat[7]=0;

	modelMat[8]=z1;
	modelMat[9]=z2;
	modelMat[10]=z3;
	modelMat[11]=0;

	modelMat[12]=t1;
	modelMat[13]=t2;
	modelMat[14]=t3;
	modelMat[15]=1;

}

GLdouble* getModelMat(double x1,double x2,double x3,
				 double y1,double y2,double y3,
				 double z1,double z2,double z3,
				 double t1,double t2,double t3){
	
	GLdouble modelMat[16];

	modelMat[0]=x1;
	modelMat[1]=x2;
	modelMat[2]=x3;
	modelMat[3]=0;

	modelMat[4]=y1;
	modelMat[5]=y2;
	modelMat[6]=y3;
	modelMat[7]=0;

	modelMat[8]=z1;
	modelMat[9]=z2;
	modelMat[10]=z3;
	modelMat[11]=0;

	modelMat[12]=t1;
	modelMat[13]=t2;
	modelMat[14]=t3;
	modelMat[15]=1;

	return modelMat;
}



/*
 *	main�֐��ł��D
 */
void glmain(int argc,char *argv[]){

	// MarkerLessAR�̏�����
	glmlar.init(1);
	glmlar.initAsift();
	//if
	//glmlar.run();
	//else
	glmlar.setImages();

	// OpenGL�\���p�摜
	camera_image = glmlar.asiftLeft.imgInput;

	// ���s��ǂݍ���
	glmlar.setAxis();
	glmlar.setPersMat(100,glmlar.tAxis[2]+200);
	
	for(int i=0;i<16;i++){
		modelMat[i]=0;
	}
	
	modelMat[0]=glmlar.xAxis[0];
	modelMat[1]=glmlar.xAxis[1];
	modelMat[2]=glmlar.xAxis[2];
	modelMat[3]=0;

	//modelMat[0]=2;
	//modelMat[1]=0;
	//modelMat[2]=0;
	//modelMat[3]=0;

	//modelMat[0]=glmlar.xAxis[0];
	//modelMat[1]=0;
	//modelMat[2]=0;
	//modelMat[3]=0;
	
	modelMat[4]=glmlar.yAxis[0];
	modelMat[5]=glmlar.yAxis[1];
	modelMat[6]=glmlar.yAxis[2];
	modelMat[7]=0;

	//modelMat[4]=0;
	//modelMat[5]=2;
	//modelMat[6]=0;
	//modelMat[7]=0;

	//modelMat[4]=0;
	//modelMat[5]=glmlar.yAxis[1];
	//modelMat[6]=0;
	//modelMat[7]=0;
	
	modelMat[8]=glmlar.zAxis[0];
	modelMat[9]=glmlar.zAxis[1];
	modelMat[10]=glmlar.zAxis[2];
	modelMat[11]=0;

	//modelMat[8]=0;
	//modelMat[9]=0;
	//modelMat[10]=2;
	//modelMat[11]=0;

	//modelMat[8]=0;
	//modelMat[9]=0;
	//modelMat[10]=glmlar.zAxis[2];
	//modelMat[11]=0;

	//modelMat[12]=-glmlar.tAxis[0];
	//modelMat[13]=-glmlar.tAxis[1];
	//modelMat[14]=-glmlar.tAxis[2];
	//modelMat[15]=1;

	modelMat[12]=0.0f;
	modelMat[13]=0.0f;
	modelMat[14]=0.0f;
	modelMat[15]=1;

	//setModelMat(1/sqrt(2),0,0,0,2,0,0,0,2,0,0,0);

	//for(int i=0;i<3;i++){
	//	modelMat[0+4*i] = glmlar.xAxis[i];
	//	modelMat[1+4*i] = glmlar.yAxis[i];
	//	modelMat[2+4*i] = glmlar.zAxis[i];
	//	//modelMat[3+4*i] = glmlar.tAxis[i];
	//	modelMat[3+4*i] = 0;
	//}
	//for(int i=0;i<3;i++){
	//	modelMat[4*3+i] = 0;
	//}
	//modelMat[4*3+3] = 1;

	// �J�����摜��OpenGL�̌`���ɕϊ����܂�
	cv::cvtColor(camera_image, copy_image, CV_BGR2RGB);
	cv::flip(copy_image, copy_image, 0);

	// OpenGL�����������܂�
	initialize_opengl(argc, argv, copy_image.cols, copy_image.rows);

	// �C�x���g���󂯕t���܂�
	glutMainLoop();

	// ����I�����܂�
	return ;
}

/*
 *	OpenGL�����������܂��D
 */
void initialize_opengl(int& argc, char* argv[], int width, int height)
{
	// GLUT�����������܂�
	glutInit(&argc, argv);

	// �E�B���h�E�̈ʒu��ݒ肵�܂�
	glutInitWindowPosition(0, 0);

	// �E�B���h�E�̑傫����ݒ肵�܂�
	glutInitWindowSize(width, height);

	// �\�����[�h��ݒ肵�܂�
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA);

	// �E�B���h�E�����܂�
	glutCreateWindow(argv[0]);
	
	// �E�B���h�E�`��֐���ݒ肵�܂�
	glutDisplayFunc(display_function);
	
	// �E�B���h�E�T�C�Y�ύX�֐���ݒ肵�܂�
	glutReshapeFunc(reshape_function);

	// �L�[���͊֐���ݒ肵�܂�
	glutKeyboardFunc(keyboard_function);

	// �}�E�X���͊֐���ݒ肵�܂�
	glutMouseFunc(mouse_function);

	// �w�i�F��ݒ肵�܂�
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// �f�v�X�o�b�t�@��L���ɂ��܂�
	glEnable(GL_DEPTH_TEST);

	// ���������������܂�
	initialize_light();

	// �ގ������������܂�
	initialize_material();
}

/*
 *	���������������܂��D
 */
void initialize_light(void)
{
	// ����������L���ɂ��܂�
	//glEnable(GL_LIGHTING);

	// ����0�Ԃ�L���ɂ��܂�
	//glEnable(GL_LIGHT0);
}

/*
 *	�ގ������������܂��D
 */
void initialize_material(void)
{
	float ambient[] = {0.1, 0.1, 0.1, 1.0};
	float diffuse[] = {0.0, 0.8, 0.5, 1.0};
	float specular[] = {0.8, 0.8, 0.8, 1.0};
	float shininess = 128.0;

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}

/*
 *	�E�B���h�E�`��֐��ł��D
 */
void display_function(void)
{
	// �J���[�o�b�t�@�����������܂�
	//glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// �ˉe�s������������܂�
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// ���f���r���[�s������������܂�
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// �J�����摜��w�i�ɏ������݂܂�
	glRasterPos2i(-1, -1);
	glDrawPixels(copy_image.cols, copy_image.rows, GL_RGB, GL_UNSIGNED_BYTE, copy_image.data);

	// �ˉe�s��ƃ��f���r���[�s����Đݒ肵�܂�
	reshape_function(copy_image.cols, copy_image.rows);

	// �f�v�X�o�b�t�@�����������܂�
	glClear(GL_DEPTH_BUFFER_BIT);

	// ���f���r���[�s��̃R�s�[���X�^�b�N�ɕۑ����܂�
	glPushMatrix();

	// ���_�Ǝ�����ݒ肵�܂�
	// �{�Ԃł́C�R�����g�ɂ��܂�
	//gluLookAt(
	//	2.0, 3.0 ,1.5,
	//	0.0, 0.0, 0.0,
	//	0.0, 2.0, 0.0);

	//glPushMatrix();

	////y rotate
	//glMultMatrixd(getModelMat(
	//	 cos(30.0 * PI/180.0), 0.0,  -1 * sin(30.0 * PI/180.0),
	//	                  0.0, 1.0,                        0.0,
	//	 sin(30.0 * PI/180.0), 0.0,       cos(30.0 * PI/180.0),
	//			       	  0.0, 0.0,                        0.0
	//)); 

	//glMultMatrixd(getModelMat(
	//	 0.00245, 0.6006,-0.7994,-0.974336,0.178846,0.1313,0.2218,0.75599,0.61585,-1.0,-1.0,1.0
	//));  

	//glMultMatrixd(getModelMat(
	//	 0.00245, 0.6006,-0.7994,-0.974336,0.178846,0.1313,0.2218,0.75599,0.61585,-11.708,2.953,277.390
	//));

	//glMultMatrixd(modelMat);
	//glTranslated(0.0f,0.0f,-glmlar.tAxis[2]);
	//render_axes(500);

	//// �����̂�`�悵�܂�
	////glutSolidCube(50);
	//render_cuboid(50,50,50);

	//glPopMatrix();

	// ���f���r���[�s��̃R�s�[�𕜌����܂�
	glPopMatrix();

	

	glPushMatrix();
	glMultMatrixd(getModelMat(
		 glmlar.xAxis[0], glmlar.xAxis[1],glmlar.xAxis[2],
		 glmlar.yAxis[0], glmlar.yAxis[1],glmlar.yAxis[2],
		 glmlar.zAxis[0], glmlar.zAxis[1],glmlar.zAxis[2],
		 0.0, -glmlar.tAxis[1],-glmlar.tAxis[2]
		));
	//glTranslated(glmlar.tAxis[0],glmlar.tAxis[1],10.0f);
	render_axes(500);
	render_cuboid(50,50,50);
	glPopMatrix();

	vector<cv::Point3f> pt3s;
	pt3s = glmlar.inputWorldPoints();
	glColor3f(0.0f,1.0f,1.0f);
	// �_�`��
	for(int i=0;i<pt3s.size();i++){
		glPushMatrix();

		glMultMatrixd(getModelMat(
		 glmlar.xAxis[0], glmlar.xAxis[1],glmlar.xAxis[2],
		 glmlar.yAxis[0], glmlar.yAxis[1],glmlar.yAxis[2],
		 glmlar.zAxis[0], glmlar.zAxis[1],glmlar.zAxis[2],
		 pt3s[i].x,pt3s[i].y,-pt3s[i].z
		 //0.0f,0.0f,0.0f
		));
		//glTranslated(pt3s[i].x,pt3s[i].y,-pt3s[i].z);
		glutSolidCube(1);

		cout << pt3s[i] << endl;

		glPopMatrix();
	}

	// OpenGL�̖��߂����s���܂�
	glFlush();
}

/*
 *	�E�B���h�E�T�C�Y�ύX�֐��ł��D
 *
 *	@param width �E�B���h�E�̕�
 *	@param height �E�B���h�E�̍���
 */
void reshape_function(int width, int height)
{
	// �r���[�|�[�g��ݒ肵�܂�
	glViewport(0, 0, width, height);

	// �ˉe�s������������܂�
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// �����ˉe�s���ݒ肵�܂�
	// �{�Ԃł́C�J���������p�����[�^��p����glFrustum�֐��֕ύX���܂�
	//gluPerspective(45.0,(double)width / (double)height,1.0,400.0);
	glFrustum(glmlar.persL,glmlar.persR,glmlar.persB,glmlar.persT,glmlar.persN,glmlar.persF);
	//glFrustum(0 , 2 , 4 , 0 , 1 , 100);

	// ���f���r���[�s������������܂�
	glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();
	//glLoadMatrixd(modelMat);
	glLoadIdentity();
	//glMultMatrixd(modelMat);

	////z rotate
	//glMultMatrixd(getModelMat(
	//	 cos(90.0 * PI/180.0), sin(90.0 * PI/180.0), 0.0,
	//	-sin(90.0 * PI/180.0), cos(90.0 * PI/180.0), 0.0,
	//	                  0.0,                  0.0, 1.0,
	//			     	  0.0,                  0.0, 0.0
	//));  
	
	//y rotate
	//glMultMatrixd(getModelMat(
	//	 cos(30.0 * PI/180.0), 0.0,  -1 * sin(30.0 * PI/180.0),
	//	                  0.0, 1.0,                        0.0,
	//	 sin(30.0 * PI/180.0), 0.0,       cos(30.0 * PI/180.0),
	//			       	  0.0, 0.0,                        0.0
	//)); 

	//x rotate
	//glMultMatrixd(getModelMat(
	//	 1.0, 0.0,					0.0,
	//	 0.0, cos(30.0 * PI/180.0), -1 * sin(30.0 * PI/180.0),
	//	 0.0, sin(30.0 * PI/180.0), cos(30.0 * PI/180.0),
	//	 0.0, 0.0, -10.0
	//)); 

	////scale
	//glMultMatrixd(getModelMat(
	//	 1.0, 0.0, 0.0,
	//	 0.0, 1.0, 0.0,
	//	 0.0, 0.0, 2.0,
	//	 0.0, 0.0, 0.0
	//));  
	
	////default
	//glMultMatrixd(getModelMat(
	//	 1.0, 0.0, 0.0,
	//	 0.0, 1.0, 0.0,
	//	 0.0, 0.0, 1.0,
	//	 0.0, 10.0, -100.0
	//));  

	//glMultMatrixd(getModelMat(
	//	 0.00245, 0.6006,-0.7994,-0.974336,0.178846,0.1313,0.2218,0.75599,0.61585,-11.708,2.953,277.390
	//));

	//glMultMatrixd(getModelMat(
	//	 0.00245, 0.6006,-0.7994,-0.974336,0.178846,0.1313,0.2218,0.75599,0.61585,0.0,0.0,-10.0
	//));  

}

/*
 *	�L�[���͊֐��ł��D
 *
 *	@param key �L�[�̎��
 *	@param x ���͎���x���W
 *	@param y ���͎���y���W
 */
void keyboard_function(unsigned char key, int x, int y)
{
	switch (key) {
		// ESC�L�[
		case 0x1b:
			// �����I�����܂�
			exit(0);

		default:
			break;
	}
}

/*
 *	�}�E�X���͊֐��ł��D
 *
 *	@param button �{�^���̎��
 *	@param state �{�^���̏��
 *	@param x ���͎���x���W
 *	@param y ���͎���y���W
 */
void mouse_function(int button, int state, int x, int y)
{
	switch (button) {
		case GLUT_LEFT_BUTTON:
			if (state == GLUT_UP) {
				printf("The left button was pressed at (%d, %d).\n", x, y);
			} else {
				
			}
			break;

		default:
			break;
	}
}

void render_axes(float length){
	glColor3f(1.0f,0.0f,0.0f);
	glBegin(GL_LINES);
		glVertex3f(-length,0.0f,0.0f);
		glVertex3f(length,0.0f,0.0f);
	glEnd();

	glColor3f(0.0f,1.0f,0.0f);
	glBegin(GL_LINES);
		glVertex3f(0.0f,-length,0.0f);
		glVertex3f(0.0f,length,0.0f);
	glEnd();

	glColor3f(0.0f,0.0f,1.0f);
	glBegin(GL_LINES);
		glVertex3f(0.0f,0.0f,-length);
		glVertex3f(0.0f,0.0f,length);
	glEnd();

	//glBegin(GL_LINES);
	//	glVertex3f(0.0f,0.0f,0.0f);
	//	glVertex3f(length,0.0f,0.0f);
	//glEnd();

	//glColor3f(0.0f,1.0f,0.0f);
	//glBegin(GL_LINES);
	//	glVertex3f(0.0f,0.0f,0.0f);
	//	glVertex3f(0.0f,length,0.0f);
	//glEnd();

	//glColor3f(0.0f,0.0f,1.0f);
	//glBegin(GL_LINES);
	//	glVertex3f(0.0f,0.0f,0.0f);
	//	glVertex3f(0.0f,0.0f,length);
	//glEnd();
}


void render_cuboid(double length_x,double length_y,double length_z){
	glLineWidth(2.0f);
	glColor3f(1.0f,1.0f,0.0f);
	glBegin(GL_LINE_LOOP);
		glVertex3f((GLfloat)length_x/2,-(GLfloat)length_y/2,(GLfloat)length_z/2);
		glVertex3f((GLfloat)length_x/2,-(GLfloat)length_y/2,-(GLfloat)length_z/2);
		glVertex3f((GLfloat)length_x/2,(GLfloat)length_y/2,-(GLfloat)length_z/2);
		glVertex3f((GLfloat)length_x/2,(GLfloat)length_y/2,(GLfloat)length_z/2);
	glEnd();

	//glColor3f(0.5f,1.0f,0.5f);
	glBegin(GL_LINE_LOOP);
		glVertex3f(-(GLfloat)length_x/2,(GLfloat)length_y/2,(GLfloat)length_z/2);
		glVertex3f(-(GLfloat)length_x/2,(GLfloat)length_y/2,-(GLfloat)length_z/2);
		glVertex3f(-(GLfloat)length_x/2,-(GLfloat)length_y/2,-(GLfloat)length_z/2);
		glVertex3f(-(GLfloat)length_x/2,-(GLfloat)length_y/2,(GLfloat)length_z/2);
	glEnd();

	//glColor3f(0.5f,0.5f,1.0f);
	glBegin(GL_LINE_LOOP);
		glVertex3f((GLfloat)length_x/2,(GLfloat)length_y/2,(GLfloat)length_z/2);
		glVertex3f((GLfloat)length_x/2,(GLfloat)length_y/2,-(GLfloat)length_z/2);
		glVertex3f(-(GLfloat)length_x/2,(GLfloat)length_y/2,-(GLfloat)length_z/2);
		glVertex3f(-(GLfloat)length_x/2,(GLfloat)length_y/2,(GLfloat)length_z/2);
	glEnd();

	//glColor3f(0.5f,1.0f,1.0f);
	glBegin(GL_LINE_LOOP);
		glVertex3f(-(GLfloat)length_x/2,-(GLfloat)length_y/2,(GLfloat)length_z/2);
		glVertex3f(-(GLfloat)length_x/2,-(GLfloat)length_y/2,-(GLfloat)length_z/2);
		glVertex3f((GLfloat)length_x/2,-(GLfloat)length_y/2,-(GLfloat)length_z/2);
		glVertex3f((GLfloat)length_x/2,-(GLfloat)length_y/2,(GLfloat)length_z/2);
	glEnd();

	//glColor3f(1.0f,1.0f,0.5f);
	glBegin(GL_LINE_LOOP);
		glVertex3f(-(GLfloat)length_x/2,(GLfloat)length_y/2,(GLfloat)length_z/2);
		glVertex3f(-(GLfloat)length_x/2,-(GLfloat)length_y/2,(GLfloat)length_z/2);
		glVertex3f((GLfloat)length_x/2,-(GLfloat)length_y/2,(GLfloat)length_z/2);
		glVertex3f((GLfloat)length_x/2,(GLfloat)length_y/2,(GLfloat)length_z/2);
	glEnd();

	//glColor3f(0.5f,0.5f,0.5f);
	glBegin(GL_LINE_LOOP);
		glVertex3f((GLfloat)length_x/2,(GLfloat)length_y/2,-(GLfloat)length_z/2);
		glVertex3f((GLfloat)length_x/2,-(GLfloat)length_y/2,-(GLfloat)length_z/2);
		glVertex3f(-(GLfloat)length_x/2,-(GLfloat)length_y/2,-(GLfloat)length_z/2);
		glVertex3f(-(GLfloat)length_x/2,(GLfloat)length_y/2,-(GLfloat)length_z/2);
	glEnd();

}