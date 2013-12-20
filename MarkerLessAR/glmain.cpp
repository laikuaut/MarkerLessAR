#include"glmain.h"

/*
 *	main.cpp
 */

#include <stdio.h>
#include <stdlib.h>

#include <gl/glut.h>

#include"../MyLibs/OpenCVLibs/Image.h"
//#include "my_opencv.h"

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

// カメラ画像
cv::Mat camera_image;

// カメラ画像のコピー
cv::Mat copy_image;

/*
 *	main関数です．
 */
void glmain(int argc,char *argv[]){
	// カメラ画像を読み込みます
	camera_image = cv::imread("./cat.jpg");

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
	glClearColor(1.0, 1.0, 0.0, 1.0);

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
	glEnable(GL_LIGHTING);

	// 光源0番を有効にします
	glEnable(GL_LIGHT0);
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
	glClear(GL_COLOR_BUFFER_BIT);

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
	gluLookAt(
		2.0, 2.0, 1.5,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0);

	// 立方体を描画します
	glutSolidCube(1.0);

	// モデルビュー行列のコピーを復元します
	glPopMatrix();

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
	gluPerspective(
		45.0,
		(double)width / (double)height,
		1.0,
		100.0);

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