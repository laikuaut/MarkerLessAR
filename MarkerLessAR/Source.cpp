// Copyright (c) 2013 Shota Taniguchi
// 
// This software is released under the MIT License.
// 
// http://opensource.org/licenses/mit-license.php

#include<iostream>
#include"../MyLibs/OpenCVLibs/Image.h"

using namespace std;
using namespace pro;	

int main(void){
	cv::VideoCapture cap(0);
	// 様々な設定...
	cv::Size cap_size(640, 480);
	cap.set(CV_CAP_PROP_FRAME_WIDTH, cap_size.width);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, cap_size.height);
	// カメラがオープンできたかの確認
	if(!cap.isOpened()) return -1;

	// ビデオライタ
	int fps = 15;
	cv::VideoWriter writer("capture.avi", CV_FOURCC('X','V','I','D'), fps, cap_size);

	//cv::namedWindow("Capture", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
	//cv::moveWindow("Capture",100,100);
	//cv::Mat frame;
	
	Image frame;
	int x=0,y=0,w=0,h=0;
	frame.imshow("Capture");
	frame.moveWindow(100,100);
	
	while(1) {
		cap.read(frame);
		//cap >> frame;  // キャプチャ
		// 様々な処理
		// ...
		writer.write(frame);
		//writer << frame;
		frame.imshow("Capture");
		if(cv::waitKey(30) >= 0) break;
		frame.getWindowRect(x,y,w,h);
		cout << x << "," << y << "," << w << "," << h << endl;
	}
}
