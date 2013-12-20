// Copyright (c) 2013 Shota Taniguchi
// 
// This software is released under the MIT License.
// 
// http://opensource.org/licenses/mit-license.php

#include<iostream>
#include<gl\glut.h>
#include<gl\GL.h>
#include<gl\GLU.h>

#include <opencv2/calib3d/calib3d.hpp>

#include "../ASIFT/ASIFT/Asift.h"
#include "MarkerLessAR.h"

#include"../MyLibs/OpenCVLibs/Image.h"
#include"glmain.h"

using namespace std;
using namespace pro;

/**
 * プロトタイプ宣言
 */
void main_Asift(int,char*[]);
int main_videoWriter(int argc, char *argv[]);
int main_videoReader(int argc,char *argv[]);
void main_resizeImage(int argc,char *argv[]);
void main_siftDotPrint(int argc,char *argv[]);
void main_centerLine(int argc,char *argv[]);
void main_help();

/**
 * MarkerLessAR
 * default
 */
void main_MarkerLessAR(int argc,char *argv[]){

	MarkerLessAR mlar;
	mlar.init(1);
	//mlar.run();
	mlar.setAxis();
}

/**
 * Asift main
 * -asift
 */
void main_Asift(int argc,char *argv[]){
	Asift asift;
	asift.init(1);
	asift.run();
}

/**
 * 動画書き込み
 * -vw
 */
int main_videoWriter(int argc, char *argv[])
{
	cv::VideoCapture cap(0);
	// 様々な設定...
	if(argc != 6){
		cout << "input << capture.avi,width,height,time" << endl; 
		return 0;
	}

	cout << "s:start q:quit" << endl;

	cv::Size cap_size(atoi(argv[3]), atoi(argv[4]));
	cap.set(CV_CAP_PROP_FRAME_WIDTH, cap_size.width);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, cap_size.height);
	// カメラがオープンできたかの確認
	if(!cap.isOpened()) return -1;

	// ビデオライタ
	int fps = 15;
	cv::VideoWriter writer(argv[2], CV_FOURCC('X','V','I','D'), fps, cap_size);

	cv::namedWindow("Capture", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
	cv::Mat frame;   

	int start = 0;
	pro::Timer timer;

	int time = atoi(argv[5]);

	while(1) {
		cap >> frame;  // キャプチャ
		// 様々な処理
		// ...
		if(start)
			writer << frame;

		cv::imshow(argv[2], frame);

		int key = cv::waitKey(30) ;

		// タイマー最大１０秒
		if(time!=0 && timer.getNow()>time/2*pro::Timer::PER_SEC){
			break;
		}

		// sキーで開始
		if(key == 's'){
			timer.start();
			cout << ((start==0)?"start":"stop") << endl;
			start = (start==1)?0:1;
		}
		// qキーで終了
		else if(key == 'q')
			break;
	}
	writer.release();

	cout << argv[2] << " end" << endl;

	return 0;
}

/**
 *2台カメラでステレオカメラ動画撮影
 * -vw2
 */
int main_videoWriter2(int argc,char *argv[]){

	cv::VideoCapture capLeft;
	cv::VideoCapture capRight;

	// 様々な設定...
	if(argc != 7){
		std::cout << "input << capLeft.avi capRight.avi width height time" << endl; 
		return 0;
	}

	std::cout << "s:start q:quit" << endl;

	cv::Size cap_size(atoi(argv[4]), atoi(argv[5]));
	pro::Image frameL,frameR;
	
	capLeft.set(CV_CAP_PROP_FRAME_WIDTH, cap_size.width);
	capLeft.set(CV_CAP_PROP_FRAME_HEIGHT, cap_size.height);
	//capLeft.open(0);
	cv::namedWindow("LeftSetting",1);
	while(1){
		// 左カメラ設定
		int key = cv::waitKey(30);
		// カメラがオープンできたかの確認
		if(capLeft.isOpened()){
			capLeft.read((cv::Mat&)frameL);
			try{
				frameL.imshow("LeftSetting",1);
			}catch(pro::Exception const &ex){
				std::cout << ex.what() << endl;
			}
			if(key==' '){
				break;
			}
		}
		if(key=='q') exit(1);
		//cout << key-'0' << endl;
		if((key-'0')>=0 && (key-'0')<=9){
			if(capLeft.isOpened()){
				capLeft.release();
			}
			capLeft.open(key-'0');
			capLeft.set(CV_CAP_PROP_FRAME_WIDTH, cap_size.width);
			capLeft.set(CV_CAP_PROP_FRAME_HEIGHT, cap_size.height);
		}
	}
	cv::destroyWindow("LeftSetting");

	capRight.set(CV_CAP_PROP_FRAME_WIDTH, cap_size.width);
	capRight.set(CV_CAP_PROP_FRAME_HEIGHT, cap_size.height);
	//capRight.open(0);
	cv::namedWindow("RightSetting",1);
	while(1){
		// 右カメラ設定
		int key = cv::waitKey(30);
		// カメラがオープンできたかの確認
		if(capRight.isOpened()){
			capRight.read((cv::Mat&)frameR);
			try{
				frameR.imshow("RightSetting",1);
			}catch(pro::Exception const &ex){
				std::cout << ex.what() << endl;
			}
			if(key==' '){
				break;
			}
		}
		if(key=='q') exit(1);
		//cout << key-'0' << endl;
		if((key-'0')>=0 && (key-'0')<=9){
			if(capRight.isOpened()){
				capRight.release();
			}
			capRight.open(key-'0');
			capRight.set(CV_CAP_PROP_FRAME_WIDTH, cap_size.width);
			capRight.set(CV_CAP_PROP_FRAME_HEIGHT, cap_size.height);
		}
	}
	cv::destroyWindow("RightSetting");

	// ビデオライタ
	int fps = 15;
	cv::VideoWriter writerLeft(argv[2], CV_FOURCC('X','V','I','D'), fps, cap_size);
	cv::VideoWriter writerRight(argv[3], CV_FOURCC('X','V','I','D'), fps, cap_size);

	//cv::namedWindow("CapLeft", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
	//cv::namedWindow("CapRight", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);

	cv::Mat frameLeft;   
	cv::Mat frameRight;   

	int start = 0;
	pro::Timer timer;

	int time = atoi(argv[5]);

	while(1) {

		capLeft >> frameLeft;  // キャプチャ
		capRight >> frameRight;  // キャプチャ

		// 様々な処理
		// ...
		if(start){
			writerLeft << frameLeft;
			writerRight << frameRight;
		}

		if(!frameLeft.empty())
			cv::imshow(argv[2], frameLeft);
		if(!frameRight.empty())
			cv::imshow(argv[3], frameRight);

		int key = cv::waitKey(30);

		// タイマー最大１０秒
		if(time!=0 && timer.getNow()>time/2*pro::Timer::PER_SEC){
			break;
		}

		// sキーで開始
		if(key == 's'){
			timer.start();
			std::cout << ((start==0)?"start":"stop") << endl;
			start = (start==1)?0:1;

		// qキーで終了
		}else if(key == 'q')
			break;
	}

	writerLeft.release();
	writerRight.release();

	return 0;
}

/**
 * 動画読み込み
 * -vr
 */
int main_videoReader(int argc,char *argv[]){
	if(argc!=4){
		cout << "input >> invideo.avi outvideo.avi" << endl;
		return 0;
	}

	cv::VideoCapture cap(argv[2]);
	// ファイルがオープンできたかの確認
	if(!cap.isOpened()) return -1;
	
	int width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	int height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	
	// ビデオライタ
	int fps = 15;
	cv::VideoWriter writer(argv[3], CV_FOURCC('X','V','I','D'), fps, cv::Size(800*2+20,600));
	
	cv::namedWindow(argv[2], CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
	cv::namedWindow(argv[3], CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
	while(1) {
		pro::Image frame;
		cap.read((cv::Mat&)frame);  // キャプチャ

		if(frame.empty()){
			cout << "empty" << endl;
			break;
		}
		
		frame.imshow(argv[2]);
		
		frame.line(cv::Point2f(0,0),cv::Point2f(width,height));
		frame.line(cv::Point2f(width,0),cv::Point2f(0,height));
		frame.horiconcat(frame,frame,20);

		// 様々な処理
		// ...
		frame.imshow(argv[3]);
		//

		writer << frame;

		if(cv::waitKey(30) >= 0) break;
	}
	return 0;
}

/**
 * 画像サイズ変更
 * -ire
 */
void main_resizeImage(int argc,char *argv[]){
	if(argc!=6){
		cout << "input >> input.png output.png width height" << endl;
		return;
	}
	int width = atoi(argv[4]);
	int height = atoi(argv[5]);
	pro::Image in(argv[2]),out;
	in.imshow("in",1);
	out.resize(in,cv::Size(width,height));
	out.imshow("out",1);
	out.save(argv[3]);
	cv::waitKey(0);
}

/**
 * カメラ撮影
 * -iw
 */
void main_imageWriter(int argc,char *argv[]){

	if(argc!=5){
		cout << "input >> capnum marker.png startNum" << endl;
		return;
	}

	cout << "撮影 : SPACE , 終了 : qキー" << endl;

	cv::VideoCapture cap(atoi(argv[2]));

	pro::Image frame;

	int count = atoi(argv[4]);
	
	while(1) {

		cap.read((cv::Mat&)frame);
		if(!frame.empty())
			frame.imshow(argv[3],1);

		int key = cv::waitKey(30);

		// space 撮影
		if(key == ' '){
			stringstream ss;
			ss<<pro::Dir::getStem(argv[3]) << std::setfill('0') << std::setw(3) << count++ << pro::Dir::getExtention(argv[3]);
			frame.save(ss.str());
			cout << ss.str() << " save." << endl;
		// qキーで終了
		}else if(key == 'q')
			break;
	}
	return;
}

/**
 * 画像撮影2つカメラ
 * -iw2
 */
void main_imageWriter2(int argc,char *argv[]){
	cv::VideoCapture capLeft;
	cv::VideoCapture capRight;
	//cv::VideoCapture capLeft(0);
	//cv::VideoCapture capRight(0);

	// 様々な設定...
	if(argc != 6){
		std::cout << "input << imgLeft.png imgRight.png width height" << endl; 
		return;
	}

	std::cout << "s:start q:quit" << endl;

	cv::Size cap_size(atoi(argv[4]), atoi(argv[5]));
	pro::Image frameL,frameR;
	
	capLeft.set(CV_CAP_PROP_FRAME_WIDTH, cap_size.width);
	capLeft.set(CV_CAP_PROP_FRAME_HEIGHT, cap_size.height);
	cv::namedWindow("LeftSetting",1);
	//capLeft.open(0);
	while(1){
		// 左カメラ設定
		int key = cv::waitKey(30);
		// カメラがオープンできたかの確認
		if(capLeft.isOpened()){
			capLeft.read((cv::Mat&)frameL);
			try{
				frameL.imshow("LeftSetting",1);
			}catch(pro::Exception const &ex){
				std::cout << ex.what() << endl;
			}
			if(key==' '){
				break;
			}
		}
		if(key=='q') exit(1);
		//cout << key-'0' << endl;
		if((key-'0')>=0 && (key-'0')<=9){
			if(capLeft.isOpened()){
				capLeft.release();
			}
			capLeft.open(key-'0');
			capLeft.set(CV_CAP_PROP_FRAME_WIDTH, cap_size.width);
			capLeft.set(CV_CAP_PROP_FRAME_HEIGHT, cap_size.height);
		}
	}
	cv::destroyWindow("LeftSetting");

	capRight.set(CV_CAP_PROP_FRAME_WIDTH, cap_size.width);
	capRight.set(CV_CAP_PROP_FRAME_HEIGHT, cap_size.height);
	//capRight.open(0);
	cv::namedWindow("RightSetting",1);
	while(1){
		// 右カメラ設定
		int key = cv::waitKey(30);
		// カメラがオープンできたかの確認
		if(capRight.isOpened()){
			capRight.read((cv::Mat&)frameR);
			try{
				frameR.imshow("RightSetting",1);
			}catch(pro::Exception const &ex){
				std::cout << ex.what() << endl;
			}
			if(key==' '){
				break;
			}
		}
		if(key=='q') exit(1);
		//cout << key-'0' << endl;
		if((key-'0')>=0 && (key-'0')<=9){
			if(capRight.isOpened()){
				capRight.release();
			}
			capRight.open(key-'0');
			capRight.set(CV_CAP_PROP_FRAME_WIDTH, cap_size.width);
			capRight.set(CV_CAP_PROP_FRAME_HEIGHT, cap_size.height);
		}
	}
	cv::destroyWindow("RightSetting");

	pro::Image frameLeft;
	pro::Image frameRight;

	int start = 0;
	pro::Timer timer;

	int time = atoi(argv[5]);

	while(1) {

		capLeft.read((cv::Mat&)frameLeft);  // キャプチャ
		capRight.read((cv::Mat&)frameRight);  // キャプチャ

		if(!frameLeft.empty())
			frameLeft.imshow(argv[2],1);
		if(!frameRight.empty())
			frameRight.imshow(argv[3],1);

		int key = cv::waitKey(30);

		// space 撮影
		if(key == ' '){
			frameLeft.save(argv[2]);
			frameRight.save(argv[3]);
		// qキーで終了
		}else if(key == 'q')
			break;
	}

	return;
}

/**
 * 画像読み込み
 * -ir
 */
void main_imageReader(int argc,char *argv[]){

}

/**
 * SIFTまたはASIFTのキーポイントを点で描写
 * -isp
 */
void main_siftDotPrint(int argc,char *argv[]){

	if(argc != 6 && argc!=7){
		cout << "input >> input.png output.png keys.txt flag=0(0:sift,1:asift) tilt" << endl;
		return;
	}

	string input = argv[2];
	string output = argv[3];
	string keys_txt = argv[4];
	int flag = atoi(argv[5]);

	//AsiftKeypoints keys;
	Asift asift;
	asift.init(1);
	asift.initImages();
	Image in_img,out_img;
	// 画像読み込み
	in_img.load(input);
	out_img.load(input);

	// Asiftに画像設置
	asift.setImage(in_img,Asift::IMAGE_ID_BASE);

	// 読み込みiniファイル設定
	ptree pt;
	read_ini("Asift.ini", pt);

	// asiftとsiftの判定
	if(flag == 0){
		asift.baseKeys = AsiftKeypoints(1);
	}else{		
		if(argc!=7){
			cout << "set the tilt value." << endl;
			exit(EXIT_FAILURE);
		}
		int tilt = atoi(argv[6]);
		asift.baseKeys = AsiftKeypoints(tilt);

	}

	// キーポイント算出
	asift.computeKeyPoints(Asift::IMAGE_ID_BASE);

	// キーポイントのドットを描写
	for (int tt = 0; tt < (int) asift.baseKeys.keys.size(); tt++)
	{
		for (int rr = 0; rr < (int) asift.baseKeys.keys[tt].size(); rr++)
		{
			keypointslist::iterator ptr = asift.baseKeys.keys[tt][rr].begin();
			for(int i=0; i < (int) asift.baseKeys.keys[tt][rr].size(); i++, ptr++)	
			{
				cv::Scalar color;
				if(tt == 0){
					color = cv::Scalar(0,0,255);
				}else{
					color = cv::Scalar(255,0,0);
				}
				// 中心点の描写
				out_img.circle(cv::Point2f(ptr->x,ptr->y),1,color);
				// スケールの描写
				out_img.circle(cv::Point2f(ptr->x,ptr->y),ptr->scale,color,1);
				// 特徴点のオリエンテーション方向を描写
				if(ptr->angle>=0){
					cv::Point pt2(ptr->x + cos(ptr->angle)*ptr->scale, ptr->y + sin(ptr->angle)*ptr->scale);
					out_img.line(cv::Point2f(ptr->x,ptr->y),pt2,color,1);
				}			
			}
		}	
	}

	// キーポイントのテキスト出力
	asift.baseKeys.output(keys_txt);
		
	// 描写画像の保存
	out_img.save(output);
}

/**
 * センターラインの検出
 * -cl
 */
void main_centerLine(int argc,char *argv[]){
	
	Asift asift;
	asift.init(1);
	asift.initImages();
	asift.baseKeys = AsiftKeypoints(1);

	if(argc!=5){
		cout << "input >> in.png out.png distance" << endl;
		return;
	}
	
	string input = argv[2];
	string output = argv[3];

	double distance = atoi(argv[4]);
	
	pro::Image in_img,out_img;
	in_img.load(input);
	out_img.load(input);

	double center_x,center_y;
	center_x = in_img.size().width/2;
	center_y = in_img.size().height/2;

	out_img.line(cv::Point2f(0,center_y),cv::Point2f(out_img.size().width,center_y),cv::Scalar(255,0,0),1);
	out_img.line(cv::Point2f(center_x,0),cv::Point2f(center_x,out_img.size().height),cv::Scalar(255,0,0),1);

	asift.computeKeyPoints(Asift::IMAGE_ID_BASE);
	std::cout << asift.baseKeys.getNum() << endl;

	out_img.imshow(output);

	//asift.baseKeys.filterRectangle(cv::Point2f(0,0),cv::Point2f(out_img.size().width,out_img.size().height));
	asift.setFilterRectanglePoints(in_img);
	asift.fileterRun(asift.baseKeys);
	std::cout << asift.baseKeys.getNum() << endl;

	//asift.baseKeys.draw(out_img);

	for (int tt = 0; tt < (int) asift.baseKeys.keys.size(); tt++)
	{
		for (int rr = 0; rr < (int) asift.baseKeys.keys[tt].size(); rr++)
		{
			keypointslist::iterator ptr = asift.baseKeys.keys[tt][rr].begin();
			for(int i=0; i < (int) asift.baseKeys.keys[tt][rr].size(); i++, ptr++)	
			{
				if(distance >= abs(ptr->x-center_x) || distance >= abs(ptr->y - center_y)){
					cv::Scalar color;
					if(tt == 0){
						color = cv::Scalar(0,0,255);
					}else{
						color = cv::Scalar(255,0,0);
					}
					// 中心点の描写
					out_img.circle(cv::Point2f(ptr->x,ptr->y),1,color);
					// スケールの描写
					out_img.circle(cv::Point2f(ptr->x,ptr->y),ptr->scale,color,1);
					// 特徴点のオリエンテーション方向を描写
					if(ptr->angle>=0){
						cv::Point pt2(ptr->x + cos(ptr->angle)*ptr->scale, ptr->y + sin(ptr->angle)*ptr->scale);
						out_img.line(cv::Point2f(ptr->x,ptr->y),pt2,color,1);
					}
				}
			}
		}	
	}

	out_img.save(output);

	out_img.imshow(output);
	cv::waitKey(0);
}

/**
 * カメラキャリブレーション
 * -calibrate
 */
int main_calibrate(int argc,char *argv[])
{

	if(argc!=4){
		std::cout << "input >> capnum=0 capName=cap0" << std::endl;
	}
	
	// カメラ番号
	int cameraNum = atoi(argv[2]);
	string cameraName = argv[3];

	// 撮影回数
	const int number_of_patterns = 10;

	// チェッカーパターンの交点の数
	const cv::Size pattern_size(13, 9);

	// 格子の長さ [mm]
	const int length = 19.538;

	// 交点の画像座標
	std::vector<std::vector<cv::Point2f>> image_points(number_of_patterns);

	// 交点のワールド座標
	std::vector<std::vector<cv::Point3f>> world_points(number_of_patterns);

	// 
	cv::TermCriteria criteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1);

	// カメラの接続を確認します
	cv::VideoCapture capture(cameraNum); 

	// カメラが未接続ならば，強制終了します
	if (!capture.isOpened()) {
		return -1;
	}

	// カメラ画像
	cv::Mat camera_image;

	// グレースケール画像
	cv::Mat grayscale_image;

	// カメラ画像表示ウィンドウを作ります
	cv::namedWindow("Camera Image");

	// グレースケール画像表示ウィンドウを作ります
	cv::namedWindow("Grayscale Image");

	int i = 0;
	while (i < number_of_patterns) {
		// カメラ画像を取得します
		capture >> camera_image;

		// カメラ画像をグレースケール画像へ変換します
		cv::cvtColor(camera_image, grayscale_image, CV_BGR2GRAY);

		// グレースケール画像を表示します
		cv::imshow("Grascale Image", grayscale_image);

		// 1[ms]キー入力がなければ，カメラ画像の取得をやり直します
		if (cv::waitKey(1) < 0) {
			continue;
		}

		// チェッカーパターンの交点を検出します
		if (!cv::findChessboardCorners(grayscale_image, pattern_size, image_points[i])) {
			// 交点検出に失敗したので，やり直します
			continue;
		}

		// 交点をサブピクセル精度に修正します
		cv::cornerSubPix(
			grayscale_image,	// 画像
			image_points[i],	// 交点
			cv::Size(11, 11),	// 
			cv::Size(-1, -1),	// 
			criteria);			// 

		// 修正後の交点をカメラ画像へ描画します
		cv::drawChessboardCorners(
			camera_image,				// カメラ画像
			pattern_size,				// チェッカーパターンの大きさ
			cv::Mat(image_points[i]),	// 交点
			true);

		// カメラ画像を表示します
		cv::imshow("Camera Image", camera_image);

		// 
		i++;

		// 
		std::cout << i << "回目の検出ができました．" << std::endl;
	}

	// 交点のワールド座標を計算します
	for (int i = 0; i < number_of_patterns; i++) {
		for (int j = 0; j < pattern_size.area(); j++) {
			world_points[i].push_back(
				cv::Point3f(
					static_cast<float>(j % pattern_size.width * length),
					static_cast<float>(j / pattern_size.width * length),
					0.0));
		}
	}

	// 内部パラメータ行列
	cv::Mat camera_matrix;

	// 歪み係数ベクトル
	cv::Mat distortions;

	// 回転ベクトル
	std::vector<cv::Mat> rotation_vectors;

	// 並進ベクトル
	std::vector<cv::Mat> translation_vectors;

	// キャリブレーションを行います
	cv::calibrateCamera(
		world_points,			// 交点のワールド座標
		image_points,			// 交点の画像座標
		camera_image.size(),	// カメラ画像の大きさ
		camera_matrix,			// 内部パラメータ行列
		distortions,			// 歪み係数ベクトル
		rotation_vectors,		// 回転ベクトル
		translation_vectors);	// 並進ベクトル

	// 内部パラメータ行列を表示します
	std::cout << camera_matrix << std::endl;

	// ファイル出力
	//cv::FileStorage cvfs("CameraCalibrate.xml",CV_STORAGE_WRITE);
	cv::FileStorage cvfs("CameraCalibrate.xml",CV_STORAGE_APPEND);
	cv::write(cvfs,cameraName,camera_matrix);

	// http://www18.atwiki.jp/cvlec/pages/19.html
	// http://d.hatena.ne.jp/fous/20090909/1252495733#20090909f1

	// 正常終了します
	return 0;
}

/**
 * マーカー作成
 * -mc
 */
void main_markerCreate(int argc,char *argv[]){

	if(argc!=4 && argc!=5 && argc!=6){
		cout << "input >> marker.png titls rectFilterFlag=1 imageShow=1" << endl;
		return;
	}

	string markerName = argv[2];
	int tilts = atoi(argv[3]);
	
	int rectFlag = 1;
	if(argc==5)
		rectFlag = atoi(argv[4]);

	int showFlag = 1;
	if(argc==6)
		showFlag = atoi(argv[5]);

	Asift asift;
	asift.init(1);
	asift.markerCreate(markerName,tilts,rectFlag,showFlag);
}

/**
 * キャリブレーション読み込み
 * -rcab
 */
void main_readCalibrate(int argc,char *argv[]){
    cv::Mat cm;
 
    cv::FileStorage cvfs("CameraCalibrate.xml", CV_STORAGE_READ);
    cv::FileNode node(cvfs.fs, NULL);

    cv::read(node["Camera0"], cm);

    std::cout << cm << std::endl;
}

/**
 * ヘルプ表示 
 * -h
 */
void main_help(){
	std::cout << "default : MarkerLessAR" << endl;
	std::cout << "-asift : Asift Run" << endl;
	std::cout << "-vw : video writer" << endl;
	std::cout << "-vw2 : video writer 2 view" << endl;
	std::cout << "-vr : video reader" << endl;
	std::cout << "-iw2 : image writer 2 view" << endl;
	std::cout << "-ire : Image resize" << endl;
	std::cout << "-isp: Sift or Asift Keypoints Dot Print int Image" << endl;
	std::cout << "-cl : center line" << endl;
	std::cout << "-calibrate : calibrate Camera." << endl;
	std::cout << "-mc : marker create" << endl;
	std::cout << "-rcab : main_readCalibrate" << endl;
	std::cout << "-h : help show" << endl;
}

/**
 * テスト関数
 * -test
 */
void main_test(int argc,char *argv[]){
	AsiftMatchings asiftM;
	asiftM.input("marker002.png_LR.txt");
	asiftM.output("marker002.png_LR__.txt");
}

/**
 * メイン関数
 */
void main(int argc,char *argv[]){
	if(argc==1 || argv[1][0]!='-'){
		main_MarkerLessAR(argc,argv);
	}else if(argc>1){
		string option = string(argv[1]);
		if(option=="-asift"){
			main_Asift(argc,argv);
		}else if(option=="-vw"){
			main_videoWriter(argc,argv);
		}else if(option=="-vw2"){
			main_videoWriter2(argc,argv);
		}else if(option=="-vr"){
			main_videoReader(argc,argv);
		}else if(option=="-iw"){
			main_imageWriter(argc,argv);
		}else if(option=="-iw2"){
			main_imageWriter2(argc,argv);
		}else if(option=="-ir"){
			main_imageReader(argc,argv);
		}else if(option=="-ire"){
			main_resizeImage(argc,argv);
		}else if(option=="-cl"){
			main_centerLine(argc,argv);
		}else if(option=="-isp"){
			main_siftDotPrint(argc,argv);
		}else if(option=="-calibrate"){
			main_calibrate(argc,argv);
		}else if(option=="-mc"){
			main_markerCreate(argc,argv);
		}else if(option=="-rcab"){
			main_readCalibrate(argc,argv);
		}else if(option=="-gl"){
			glmain(argc,argv);
		}else if(option=="-test"){
			main_test(argc,argv);
		}else if(option=="-h"){
			main_help();
		}
	}else{
		std::cout << "not command." << endl;
		std::cout << "help is -h option." << endl;
	}
}

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
