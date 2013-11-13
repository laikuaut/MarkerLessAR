// Copyright (c) 2013 Shota Taniguchi
// 
// This software is released under the MIT License.
// 
// http://opensource.org/licenses/mit-license.php

#include<iostream>
#include"../MyLibs/OpenCVLibs/Image.h"
#include<gl\glut.h>

using namespace std;
using namespace pro;

int m_ID;
Image img;
//cv::Mat img;
cv::Size size;
int m_w,m_h;
double m_wr,m_hr;
cv::VideoCapture cap(0);

#define MAXSIZE 1024

GLuint texture[2];

//————— 画像を読み込み//

void wininit(const int iw, const int ih, const char* name){
	m_w = iw, m_h = ih;
	m_wr = static_cast<double>(iw) / static_cast<double>(MAXSIZE);
	m_hr = static_cast<double>(ih) / static_cast<double>(MAXSIZE);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(iw, ih);
	m_ID = glutCreateWindow(name);
}

void setTex(const int texturenum){
	texture[0] = texturenum;

	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, MAXSIZE, MAXSIZE, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, 0);

}

void draw(const unsigned char* data){
	//glutSetWindow(m_ID);

	glBindTexture(GL_TEXTURE_2D, texture[0]);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	// グレースケール
	glTexImage2D(GL_TEXTURE_2D , 0 , GL_DEPTH_COMPONENT, m_w,m_h,0,GL_DEPTH_COMPONENT,GL_UNSIGNED_BYTE,data);	
	// RGB
	//glTexImage2D(GL_TEXTURE_2D , 0 , GL_RGB, m_w,m_h,0,GL_BGR_EXT,GL_UNSIGNED_BYTE,data);	

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);

	glTexCoord2d(1.0, 0.0);		glVertex2d(1, 1);
	glTexCoord2d(1.0, 1.0);		glVertex2d(1, -1);
	glTexCoord2d(0.0, 1.0);		glVertex2d(-1, -1);
	glTexCoord2d(0.0, 0.0);		glVertex2d(-1, 1);

	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void display(){
	cap.read(img);
	img.grayeScale(img);
	draw(img);
	glutSwapBuffers();
}

// for glut
void idle()
{
	glutPostRedisplay();
}

// for glut
void reshape(const int w, const int h)
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void glInit(){
	cap.read(img);
	//img.load("霧島　千代子.jpg");
	img.grayeScale(img);
	size = img.size();
	wininit(size.width,size.height,"MarkerLessAR");
	//wininit(640, 480,"MarkerLessAR");

	glutDisplayFunc(display);

	//glGenTextures(2, texture);
	//setTex(texture[0]);

}

int main(int argc, char **argv)

{
	//img=cv::imread("霧島　千代子.jpg");
	//img.load("霧島　千代子.jpg");

	float * iarr1;
    size_t w1, h1;
	pro::Image img1(pro::Image::_32FC3);
	try{
		img1.load("霧島　千代子.jpg");
	}catch(pro::Exception &e){
		e.showError();
        std::cerr << "Unable to load image file " << argv[1] << std::endl;
		return 1;
	}
	w1 = img1.size().width;
	h1 = img1.size().height;
	iarr1 = img1.getF32Data();
	


	// 様々な設定...
	cv::Size cap_size(640, 480);
	cap.set(CV_CAP_PROP_FRAME_WIDTH, cap_size.width);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, cap_size.height);
	
	if(!cap.isOpened()) return -1;

	glutInit(&argc,argv);

	glInit();

	/**
	 *  GLUTのウィンドウの大きさが変ったときや、移動したときに
	 *  呼び出されるコールバック関数を登録する。
	 */
	glutReshapeFunc(reshape);	// 

	// 処理するべきイベントが何もない状態の時に発行され続けます。
	glutIdleFunc(idle);
	glutDisplayFunc(display);	// メインループ

	glutMainLoop();

	return 0;

}

//int main(void){
//	cv::VideoCapture cap(0);
//	// 様々な設定...
//	cv::Size cap_size(640, 480);
//	cap.set(CV_CAP_PROP_FRAME_WIDTH, cap_size.width);
//	cap.set(CV_CAP_PROP_FRAME_HEIGHT, cap_size.height);
//	// カメラがオープンできたかの確認
//	if(!cap.isOpened()) return -1;
//
//	// ビデオライタ
//	int fps = 15;
//	cv::VideoWriter writer("capture.avi", CV_FOURCC('X','V','I','D'), fps, cap_size);
//
//	//cv::namedWindow("Capture", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
//	//cv::moveWindow("Capture",100,100);
//	//cv::Mat frame;
//	
//	Image frame;
//	int x=0,y=0,w=0,h=0;
//	frame.imshow("Capture");
//	frame.moveWindow(100,100);
//	
//	while(1) {
//		cap.read(frame);
//		//cap >> frame;  // キャプチャ
//		// 様々な処理
//		// ...
//		writer.write(frame);
//		//writer << frame;
//		frame.imshow("Capture");
//		if(cv::waitKey(30) >= 0) break;
//		frame.getWindowRect(x,y,w,h);
//		cout << x << "," << y << "," << w << "," << h << endl;
//	}
//}
