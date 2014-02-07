#include"glmain.h"

/**
 * OpenGLでの描写main
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
/*
 * キューブック(枠線のみ)レンダー
 */
void render_cuboid(double length_x,double length_y,double length_z);

// カメラ画像
cv::Mat camera_image;

// カメラ画像のコピー
cv::Mat copy_image;

// マーカレスAR
MarkerLessAR glmlar;

// モデルビュー行列
GLdouble modelMat[4*4];

// 初期化フラグ
int initFlag=0;


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

GLdouble* getModelMat(double x1,double x2,double x3,double x4,
				 double y1,double y2,double y3,double y4,
				 double z1,double z2,double z3,double z4,
				 double t1,double t2,double t3,double t4){
	
	GLdouble modelMat[16];

	modelMat[0]=x1;
	modelMat[1]=x2;
	modelMat[2]=x3;
	modelMat[3]=x4;

	modelMat[4]=y1;
	modelMat[5]=y2;
	modelMat[6]=y3;
	modelMat[7]=y4;

	modelMat[8]=z1;
	modelMat[9]=z2;
	modelMat[10]=z3;
	modelMat[11]=z4;

	modelMat[12]=t1;
	modelMat[13]=t2;
	modelMat[14]=t3;
	modelMat[15]=t4;

	return modelMat;
}

void saveImage( const unsigned int imageWidth, const unsigned int imageHeight )
{
  const unsigned int channnelNum = 3; // RGBなら3, RGBAなら4
  void* dataBuffer = NULL;
  dataBuffer = ( GLubyte* )malloc( imageWidth * imageHeight * channnelNum );

  // 読み取るOpneGLのバッファを指定 GL_FRONT:フロントバッファ　GL_BACK:バックバッファ
  glReadBuffer( GL_BACK );

  // OpenGLで画面に描画されている内容をバッファに格納
  glReadPixels(
	0,                 //読み取る領域の左下隅のx座標
	0,                 //読み取る領域の左下隅のy座標 //0 or getCurrentWidth() - 1
	imageWidth,             //読み取る領域の幅
	imageHeight,            //読み取る領域の高さ
	GL_BGR_EXT, //it means GL_BGR,           //取得したい色情報の形式
	GL_UNSIGNED_BYTE,  //読み取ったデータを保存する配列の型
	dataBuffer      //ビットマップのピクセルデータ（実際にはバイト配列）へのポインタ
	);

  GLubyte* p = static_cast<GLubyte*>( dataBuffer );
  std::string fname = "W_"+glmlar.getLeftImgName();
  IplImage* outImage = cvCreateImage( cvSize( imageWidth, imageHeight ), IPL_DEPTH_8U, 3 );

  for ( unsigned int j = 0; j < imageHeight; ++ j )
  {
    for ( unsigned int i = 0; i < imageWidth; ++i )
    {
      outImage->imageData[ ( imageHeight - j - 1 ) * outImage->widthStep + i * 3 + 0 ] = *p;
      outImage->imageData[ ( imageHeight - j - 1 ) * outImage->widthStep + i * 3 + 1 ] = *( p + 1 );
      outImage->imageData[ ( imageHeight - j - 1 ) * outImage->widthStep + i * 3 + 2 ] = *( p + 2 );
      p += 3;
    }
  }

  cvSaveImage( fname.c_str(), outImage );
}

/*
 *	main関数です．
 */
void glmain(int argc,char *argv[],string *strs){

	// MarkerLessARの初期化
	if(argc==4){
		if(strs==nullptr)
			glmlar.init(1,argv[2],argv[3]);
		else
			glmlar.init(1,strs[0],strs[1]);
	}else{
		glmlar.init(1);
	}
	glmlar.initAsift();
	//if
	//glmlar.run();
	//else
	glmlar.setImages();

	// OpenGL表示用画像
	camera_image = glmlar.asiftLeft.imgInput;

	// 軸行列読み込み
	if(!glmlar.setAxis()){
		cout << "not Marker" << endl;
		std::string fname = "W_"+glmlar.getLeftImgName();
		cv::imwrite(fname.c_str(),camera_image);
		return;
		//exit(1);
	}
	if(-glmlar.tAxis[2]<1){
		glmlar.setPersMat(1,-glmlar.tAxis[2]+100);
	}else{
		glmlar.setPersMat(-glmlar.tAxis[2]-100,-glmlar.tAxis[2]+300);
	}

	for(int i=0;i<16;i++){
		modelMat[i]=0;
	}

	// カメラ画像をOpenGLの形式に変換します
	cv::cvtColor(camera_image, copy_image, CV_BGR2RGB);
	cv::flip(copy_image, copy_image, 0);

	// OpenGLを初期化します
	initialize_opengl(argc, argv, copy_image.cols, copy_image.rows);

	// イベントを受け付けます
	//glutMainLoop();

	// 正常終了します
	return ;
}

/*
 *	OpenGLを初期化します．
 */
void initialize_opengl(int& argc, char* argv[], int width, int height)
{
	// GLUTを初期化します
	if(initFlag==0){
		glutInit(&argc, argv);
		initFlag++;
	}
	// ウィンドウの位置を設定します
	glutInitWindowPosition(0, 0);

	// ウィンドウの大きさを設定します
	glutInitWindowSize(width, height);

	// 表示モードを設定します
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA);

	// ウィンドウを作ります
	int id = glutCreateWindow(argv[0]);
	
	// ウィンドウ描画関数を設定します
	//glutDisplayFunc(display_function);
	
	// ウィンドウサイズ変更関数を設定します
	//glutReshapeFunc(reshape_function);

	// キー入力関数を設定します
	//glutKeyboardFunc(keyboard_function);

	// マウス入力関数を設定します
	//glutMouseFunc(mouse_function);

	// 背景色を設定します
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// デプスバッファを有効にします
	glEnable(GL_DEPTH_TEST);

	// 光源を初期化します
	initialize_light();

	// 材質を初期化します
	initialize_material();

	// 単独実行
	display_function();

	glutDestroyWindow(id);
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
	//glPushMatrix();

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
	//glPopMatrix();

	

	glPushMatrix();
	glMultMatrixd(getModelMat(
		 glmlar.xAxis[0], glmlar.xAxis[1],glmlar.xAxis[2],
		 glmlar.yAxis[0], glmlar.yAxis[1],glmlar.yAxis[2],
		 glmlar.zAxis[0], glmlar.zAxis[1],glmlar.zAxis[2],
		 glmlar.tAxis[0], glmlar.tAxis[1],glmlar.tAxis[2]
		 //0.0f,0.0f,glmlar.tAxis[2]
		));
	// 平面代わりに表示
	//render_plane(150);

	glTranslated(0.0f,0.0f,25.0f);
	//glTranslated(0.0f,0.0f,0.0f);
	render_axes(500);
	render_cuboid(50,50,50);
	glPopMatrix();
	
	

	// X軸の三次元点描写
	vector<cv::Point3f> pt3s_xAxis;
	pt3s_xAxis = glmlar.inputXAxisWorldPoints();
	glColor3f(0.0f,1.0f,1.0f);
	cout << pt3s_xAxis.size() << endl;
	for(int i=0;i<pt3s_xAxis.size();i++){
		glPushMatrix();

		glMultMatrixd(getModelMat(
		 glmlar.xAxis[0], glmlar.xAxis[1],glmlar.xAxis[2],
		 glmlar.yAxis[0], glmlar.yAxis[1],glmlar.yAxis[2],
		 glmlar.zAxis[0], glmlar.zAxis[1],glmlar.zAxis[2],
		 pt3s_xAxis[i].x,pt3s_xAxis[i].y,pt3s_xAxis[i].z
		 //0.0f,0.0f,pt3s[i].z
		));
		//glTranslated(-pt3s[i].x,-pt3s[i].y,0.0f);
		glutSolidCube(2.0);

		//cout << pt3s[i] << endl;

		glPopMatrix();
	}
	
	// 3次元点描写
	vector<cv::Point3f> pt3s;
	pt3s = glmlar.inputWorldPoints();
	glColor3f(1.0f,0.0f,0.0f);
	for(int i=0;i<pt3s.size();i++){
		glPushMatrix();

		glMultMatrixd(getModelMat(
		 glmlar.xAxis[0], glmlar.xAxis[1],glmlar.xAxis[2],
		 glmlar.yAxis[0], glmlar.yAxis[1],glmlar.yAxis[2],
		 glmlar.zAxis[0], glmlar.zAxis[1],glmlar.zAxis[2],
		 pt3s[i].x,pt3s[i].y,pt3s[i].z
		 //0.0f,0.0f,pt3s[i].z
		));
		//glTranslated(-pt3s[i].x,-pt3s[i].y,0.0f);
		glutSolidCube(2);

		//cout << pt3s[i] << endl;

		glPopMatrix();
	}

	// OpenGLの命令を実行します
	glFlush();

	saveImage(copy_image.cols, copy_image.rows);
	//exit(1);
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
	// 論文 P51「6.2.6 内部パラメータを考慮した透視射影」
	glMultMatrixd(getModelMat(
		2*glmlar.persAu/glmlar.persW, 0, 0, 0,
		0, 2*glmlar.persAv/glmlar.persH, 0, 0,
		(glmlar.persW-2*glmlar.persU0)/glmlar.persW, (glmlar.persH-2*glmlar.persV0)/glmlar.persH, -(glmlar.persF+glmlar.persN)/(glmlar.persF-glmlar.persN), -1,
		0, 0, -(2*glmlar.persF*glmlar.persN)/(glmlar.persF-glmlar.persN), 0
		));

	// モデルビュー行列を初期化します
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


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

	glBegin(GL_LINE_LOOP);
		glVertex3f(-(GLfloat)length_x/2,(GLfloat)length_y/2,(GLfloat)length_z/2);
		glVertex3f(-(GLfloat)length_x/2,(GLfloat)length_y/2,-(GLfloat)length_z/2);
		glVertex3f(-(GLfloat)length_x/2,-(GLfloat)length_y/2,-(GLfloat)length_z/2);
		glVertex3f(-(GLfloat)length_x/2,-(GLfloat)length_y/2,(GLfloat)length_z/2);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f((GLfloat)length_x/2,(GLfloat)length_y/2,(GLfloat)length_z/2);
		glVertex3f((GLfloat)length_x/2,(GLfloat)length_y/2,-(GLfloat)length_z/2);
		glVertex3f(-(GLfloat)length_x/2,(GLfloat)length_y/2,-(GLfloat)length_z/2);
		glVertex3f(-(GLfloat)length_x/2,(GLfloat)length_y/2,(GLfloat)length_z/2);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(-(GLfloat)length_x/2,-(GLfloat)length_y/2,(GLfloat)length_z/2);
		glVertex3f(-(GLfloat)length_x/2,-(GLfloat)length_y/2,-(GLfloat)length_z/2);
		glVertex3f((GLfloat)length_x/2,-(GLfloat)length_y/2,-(GLfloat)length_z/2);
		glVertex3f((GLfloat)length_x/2,-(GLfloat)length_y/2,(GLfloat)length_z/2);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(-(GLfloat)length_x/2,(GLfloat)length_y/2,(GLfloat)length_z/2);
		glVertex3f(-(GLfloat)length_x/2,-(GLfloat)length_y/2,(GLfloat)length_z/2);
		glVertex3f((GLfloat)length_x/2,-(GLfloat)length_y/2,(GLfloat)length_z/2);
		glVertex3f((GLfloat)length_x/2,(GLfloat)length_y/2,(GLfloat)length_z/2);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f((GLfloat)length_x/2,(GLfloat)length_y/2,-(GLfloat)length_z/2);
		glVertex3f((GLfloat)length_x/2,-(GLfloat)length_y/2,-(GLfloat)length_z/2);
		glVertex3f(-(GLfloat)length_x/2,-(GLfloat)length_y/2,-(GLfloat)length_z/2);
		glVertex3f(-(GLfloat)length_x/2,(GLfloat)length_y/2,-(GLfloat)length_z/2);
	glEnd();

}