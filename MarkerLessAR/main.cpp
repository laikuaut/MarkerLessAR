// Copyright (c) 2013 Shota Taniguchi
// 
// This software is released under the MIT License.
// 
// http://opensource.org/licenses/mit-license.php

#include<iostream>
#include<gl\glut.h>
#include<gl\GL.h>
#include<gl\GLU.h>

#include "../ASIFT/ASIFT/Asift.h"

#include"../MyLibs/OpenCVLibs/Image.h"

using namespace std;
using namespace pro;

/**
 * メイン関数
 */
void main(){
	Asift asift;
	asift.init(1);
	asift.run();
}

/**
 * 動画書き込み
 */
//int main(int argc, char *argv[])
//{
//	cv::VideoCapture cap(0);
//	// 様々な設定...
//	cv::Size cap_size(800, 600);
//	cap.set(CV_CAP_PROP_FRAME_WIDTH, cap_size.width);
//	cap.set(CV_CAP_PROP_FRAME_HEIGHT, cap_size.height);
//	// カメラがオープンできたかの確認
//	if(!cap.isOpened()) return -1;
//
//	// ビデオライタ
//	int fps = 15;
//	cv::VideoWriter writer("capture.avi", CV_FOURCC('X','V','I','D'), fps, cap_size);
//
//	cv::namedWindow("Capture", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
//	cv::Mat frame;   
//
//	int start = 0;
//	while(1) {
//		cap >> frame;  // キャプチャ
//		// 様々な処理
//		// ...
//		if(start)
//			writer << frame;
//		cv::imshow("Capture", frame);
//
//		int key = cv::waitKey(30) ;
//		if(key == 's')
//			start = 1;
//		else if(key == 'q')
//			break;
//	}
//	return 0;
//}

/**
 * 動画読み込み
 */
//int main(){
//	cv::VideoCapture cap("capture.avi");
//	// ファイルがオープンできたかの確認
//	if(!cap.isOpened()) return -1;
//	
//	int width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
//	int height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
//	
//	// ビデオライタ
//	int fps = 15;
//	cv::VideoWriter writer("capture1.avi", CV_FOURCC('X','V','I','D'), fps, cv::Size(width,height));
//
//	cv::namedWindow("Capture", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
//	while(1) {
//		pro::Image frame;
//		cap.read((cv::Mat&)frame);  // キャプチャ
//
//		if(frame.empty()){
//			cout << "empty" << endl;
//			break;
//		}
//		
//		frame.line(cv::Point2f(0,0),cv::Point2f(width,height));
//		frame.line(cv::Point2f(width,0),cv::Point2f(0,height));
//
//		// 様々な処理
//		// ...
//		frame.imshow("Capture");
//		//
//
//		writer << frame;
//
//		if(cv::waitKey(30) >= 0) break;
//	}
//	return 0;
//}


//int m_ID;
//Image img;
////cv::Mat img;
//cv::Size size;
//int m_w,m_h;
//double m_wr,m_hr;
//cv::VideoCapture cap(1);
//
//#define MAXSIZE 1024
//
//GLuint texture[1];
//
////————— 画像を読み込み//
//
//void wininit(const int iw, const int ih, const char* name){
//	m_w = iw, m_h = ih;
//	m_wr = static_cast<double>(iw) / static_cast<double>(MAXSIZE);
//	m_hr = static_cast<double>(ih) / static_cast<double>(MAXSIZE);
//
//	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
//	glutInitWindowPosition(100,100);
//	glutInitWindowSize(iw, ih);
//	m_ID = glutCreateWindow(name);
//}
//
//void setTex(){
//	glGenTextures(1, texture);
//	//texture[0] = texture[];
//
//	glBindTexture(GL_TEXTURE_2D, texture[0]);
//	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
//
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, MAXSIZE, MAXSIZE, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, 0);
//	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, MAXSIZE, MAXSIZE, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
//
//}
//
//void draw(const unsigned char* data){
//	glutSetWindow(m_ID);
//
//	glBindTexture(GL_TEXTURE_2D, texture[0]);
//
//	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//
//	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_w, m_h, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);
//	//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_w, m_h, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, data);
//
//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
//
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	glEnable(GL_TEXTURE_2D);
//	glBegin(GL_QUADS);
//
//	glTexCoord2d(m_wr, 0.0);		glVertex2d(1, 1);
//	glTexCoord2d(m_wr, m_hr);		glVertex2d(1, -1);
//	glTexCoord2d(0.0 , m_hr);		glVertex2d(-1, -1);
//	glTexCoord2d(0.0 , 0.0);		glVertex2d(-1, 1);
//
//	//glTexCoord2d(1.0, 0.0);		glVertex2d(1, 1);
//	//glTexCoord2d(1.0, 1.0);		glVertex2d(1, -1);
//	//glTexCoord2d(0.0 , 1.0);		glVertex2d(-1, -1);
//	//glTexCoord2d(0.0 , 0.0);		glVertex2d(-1, 1);
//
//	glEnd();
//	glDisable(GL_TEXTURE_2D);
//}
//
//void display(){
//	cap.read(img);
//	img.grayeScale(img);
//	pro::Image image("adam1.png");
//	image.grayeScale(image);
//	img.horiconcat(img,image,20);
//	img.imshow("test");
//	//img.grayeScale(img);
//	//draw(img);
//	glutSwapBuffers();
//}
//
//// for glut
//void idle()
//{
//	glutPostRedisplay();
//}
//
//// for glut
//void reshape(const int w, const int h)
//{
//	glViewport(0, 0, w, h);
//
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//}
//
//void glInit(){
//	cap.read(img);
//	//img.load("adam2.png");
//	//img.horiconcat(img,img,20);
//
//	//img.grayeScale(img);
//	//img.imshow("test",1);
//	size = img.size();
//	wininit(size.width,size.height,"MarkerLessAR");
//	//wininit(640, 480,"MarkerLessAR");
//	
//	
//	setTex();
//
//}

//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/nonfree/features2d.hpp>
//
//int
//main(int argc, char *argv[])
//{
//  cv::Mat img = cv::imread("./adam1.png", 1);
//  if(img.empty()) return -1; 
//
//  cv::Mat gray_img;
//  cv::cvtColor(img, gray_img, CV_BGR2GRAY);
//  cv::normalize(gray_img, gray_img, 0, 255, cv::NORM_MINMAX);
//
//  std::vector<cv::KeyPoint> keypoints;
//  std::vector<cv::KeyPoint>::iterator itk;
//  cv::Mat descriptors;
//
//  //
//  // threshold=4500
//  cv::SurfFeatureDetector detector(4500);
//  detector.detect(gray_img, keypoints);
//  // SURF に基づくディスクリプタ抽出器
//  cv::SurfDescriptorExtractor extractor;
//  cv::Scalar color(100,255,50);
//  extractor.compute(gray_img, keypoints, descriptors);
//
//  // 64次元の特徴量 x keypoint数
//  for(int i=0; i<descriptors.rows; ++i) {
//    cv::Mat d(descriptors, cv::Rect(0,i,descriptors.cols,1));
//    std::cout << i << ": " << d << std::endl;
//  }
//}

//int main(int argc, char **argv)
//
//{


	//// 様々な設定...
	//cv::Size cap_size(640, 480);
	//cap.set(CV_CAP_PROP_FRAME_WIDTH, cap_size.width);
	//cap.set(CV_CAP_PROP_FRAME_HEIGHT, cap_size.height);
	//
	//if(!cap.isOpened()) return -1;

	//glutInit(&argc,argv);

	//glInit();

	///**
	// *  GLUTのウィンドウの大きさが変ったときや、移動したときに
	// *  呼び出されるコールバック関数を登録する。
	// */
	//glutReshapeFunc(reshape);

	//// 処理するべきイベントが何もない状態の時に発行され続けます。
	//glutIdleFunc(idle);

	//glutDisplayFunc(display);	// メインループ

	//glutMainLoop();

	//return 0;

//}

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
