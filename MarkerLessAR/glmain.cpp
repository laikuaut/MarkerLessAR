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
 *	OpenGLを初期化します．
 */
void initialize_opengl(int&, char* [], int, int);

/*
 *	光源を初期化します．
 */
void initialize_light(void);

/*
 *	材質を初期化します．
 */
void initialize_material(void);

/*
 *	ウィンドウ描画関数です．
 */
void display_function(void);

/*
 *	ウィンドウサイズ変更関数です．
 */
void reshape_function(int, int);

/*
 *	キー入力関数です．
 */
void keyboard_function(unsigned char, int, int);

/*
 *	マウス入力関数です．
 */
void mouse_function(int, int, int, int);

/*
 * 軸レンダー
 */
void render_axes(float);
void render_cuboid(double length_x,double length_y,double length_z);

// カメラ画像
cv::Mat camera_image;

// カメラ画像のコピー
cv::Mat copy_image;

// マーカレスAR
MarkerLessAR glmlar;

// モデルビュー行列
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
 *	main関数です．
 */
void glmain(int argc,char *argv[]){

	// MarkerLessARの初期化
	glmlar.init(1);
	glmlar.initAsift();
	//if
	//glmlar.run();
	//else
	glmlar.setImages();

	// OpenGL表示用画像
	camera_image = glmlar.asiftLeft.imgInput;

	// 軸行列読み込み
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

	// カメラ画像をOpenGLの形式に変換します
	cv::cvtColor(camera_image, copy_image, CV_BGR2RGB);
	cv::flip(copy_image, copy_image, 0);

	// OpenGLを初期化します
	initialize_opengl(argc, argv, copy_image.cols, copy_image.rows);

	// イベントを受け付けます
	glutMainLoop();

	// 正常終了します
	return ;
}

/*
 *	OpenGLを初期化します．
 */
void initialize_opengl(int& argc, char* argv[], int width, int height)
{
	// GLUTを初期化します
	glutInit(&argc, argv);

	// ウィンドウの位置を設定します
	glutInitWindowPosition(0, 0);

	// ウィンドウの大きさを設定します
	glutInitWindowSize(width, height);

	// 表示モードを設定します
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA);

	// ウィンドウを作ります
	glutCreateWindow(argv[0]);
	
	// ウィンドウ描画関数を設定します
	glutDisplayFunc(display_function);
	
	// ウィンドウサイズ変更関数を設定します
	glutReshapeFunc(reshape_function);

	// キー入力関数を設定します
	glutKeyboardFunc(keyboard_function);

	// マウス入力関数を設定します
	glutMouseFunc(mouse_function);

	// 背景色を設定します
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// デプスバッファを有効にします
	glEnable(GL_DEPTH_TEST);

	// 光源を初期化します
	initialize_light();

	// 材質を初期化します
	initialize_material();
}

/*
 *	光源を初期化します．
 */
void initialize_light(void)
{
	// 光源処理を有効にします
	//glEnable(GL_LIGHTING);

	// 光源0番を有効にします
	//glEnable(GL_LIGHT0);
}

/*
 *	材質を初期化します．
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
 *	ウィンドウ描画関数です．
 */
void display_function(void)
{
	// カラーバッファを初期化します
	//glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 射影行列を初期化します
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// モデルビュー行列を初期化します
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// カメラ画像を背景に書き込みます
	glRasterPos2i(-1, -1);
	glDrawPixels(copy_image.cols, copy_image.rows, GL_RGB, GL_UNSIGNED_BYTE, copy_image.data);

	// 射影行列とモデルビュー行列を再設定します
	reshape_function(copy_image.cols, copy_image.rows);

	// デプスバッファを初期化します
	glClear(GL_DEPTH_BUFFER_BIT);

	// モデルビュー行列のコピーをスタックに保存します
	glPushMatrix();

	// 視点と視線を設定します
	// 本番では，コメントにします
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

	//// 立方体を描画します
	////glutSolidCube(50);
	//render_cuboid(50,50,50);

	//glPopMatrix();

	// モデルビュー行列のコピーを復元します
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
	// 点描写
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

	// OpenGLの命令を実行します
	glFlush();
}

/*
 *	ウィンドウサイズ変更関数です．
 *
 *	@param width ウィンドウの幅
 *	@param height ウィンドウの高さ
 */
void reshape_function(int width, int height)
{
	// ビューポートを設定します
	glViewport(0, 0, width, height);

	// 射影行列を初期化します
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// 透視射影行列を設定します
	// 本番では，カメラ内部パラメータを用いたglFrustum関数へ変更します
	//gluPerspective(45.0,(double)width / (double)height,1.0,400.0);
	glFrustum(glmlar.persL,glmlar.persR,glmlar.persB,glmlar.persT,glmlar.persN,glmlar.persF);
	//glFrustum(0 , 2 , 4 , 0 , 1 , 100);

	// モデルビュー行列を初期化します
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
 *	キー入力関数です．
 *
 *	@param key キーの種類
 *	@param x 入力時のx座標
 *	@param y 入力時のy座標
 */
void keyboard_function(unsigned char key, int x, int y)
{
	switch (key) {
		// ESCキー
		case 0x1b:
			// 強制終了します
			exit(0);

		default:
			break;
	}
}

/*
 *	マウス入力関数です．
 *
 *	@param button ボタンの種類
 *	@param state ボタンの状態
 *	@param x 入力時のx座標
 *	@param y 入力時のy座標
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