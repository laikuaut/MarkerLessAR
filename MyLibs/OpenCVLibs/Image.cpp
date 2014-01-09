// Copyright (c) 2013 Shota Taniguchi
// 
// This software is released under the MIT License.
// 
// http://opensource.org/licenses/mit-license.php

#include "Image.h"

namespace pro{

Image::Image(void) : path(Dir()){
	init(100,100);
}

Image::Image(int width,int height,int type) : path(Dir()){
	init(width,height,type);
}

Image::Image(string name,int type) : path(Dir()){
	init(100,100,type);
	load(name);
}

Image::Image(int type) : path(Dir()){
	init(100,100,type);
}

void Image::init(int width,int height,int type){
	w = width;
	h = height;
	this->type = type;
	img = cv::Mat(cv::Size(w,h),type);
	ch = img.channels();
	winName = string();
}

void Image::w_h_reset(){
	w = img.size().width;
	h = img.size().height;
	ch = img.channels();
}

void Image::release(){
	if(!img.empty()){
		img.release();
	}
}

void Image::clone(const Image& src){
	release();
	w = src.w;
	h = src.h;
	img = src.img.clone();
	img.data;
}

void Image::swap(Image& src){
	Image tmp;
	tmp.img = img.clone();
	tmp.w = w;
	tmp.h = h;
	img = src.img.clone();
	w = src.w , h = src.h;
	src.clone(tmp);	
}

void Image::resize(const Image& src,int interpolation){
	cv::resize(src.img,img,cv::Size(w,h),0,0,interpolation);
}
void Image::resize(const Image& src,double fx,double fy,int interpolation){
	cv::resize(src.img,img,cv::Size(),fx,fy,interpolation);
	w_h_reset();
}
void Image::resize(const Image& src,cv::Size size,int interpolation){
	cv::resize(src.img,img,size,0,0,interpolation);
	w_h_reset();
}

void Image::rotation(const Image& src,cv::Point2f center,double angle){
	// 以上の条件から2次元の回転行列を計算
	float scale = 1.0;
	const cv::Mat affine_matrix = cv::getRotationMatrix2D( center, angle, scale );
	cv::warpAffine(src.img, img, affine_matrix, src.img.size());
}

void Image::horiconcat(const Image& src1,const Image& src2,int band_w,cv::Scalar color){
	if(band_w<=0){
		cv::hconcat(src1.img,src2.img,img);
	}else{
		Image band(band_w,src1.img.size().height,img.type());
		band.oneColor(color);
		try{
			cv::hconcat(src1.img,band.img,band.img);
			cv::hconcat(band.img,src2.img,img);
		}catch(cv::Exception ex){
			std::cout << ex.what() << std::endl;
			std::cout << "size not fit" << std::endl;
			exit(1);
		}
	}
	w_h_reset();
}

void Image::vertconcat(const Image& src1,const Image& src2,int band_w,cv::Scalar color){
	if(band_w<=0){
		cv::vconcat(src1.img,src2.img,img);
	}else{
		Image band(src1.img.size().width,band_w,img.type());
		band.oneColor(color);
		try{
			cv::vconcat(src1.img,band.img,band.img);
			cv::vconcat(band.img,src2.img,img);
		}catch(cv::Exception ex){
			std::cout << ex.what() << std::endl;
			std::cout << "size not fit" << std::endl;
			exit(1);
		}
	}
	w_h_reset();
}

void Image::grayeScale(const Image& src){
	cv::cvtColor(src.img, img, CV_BGR2GRAY);
	w_h_reset();
}

void Image::grayToColor(const Image& src){
	cv::cvtColor(src.img,img,CV_GRAY2BGR);
	w_h_reset();
}

void Image::binarization(const Image& src){
	cv::threshold(src.img, img, 0, 255, cv::THRESH_BINARY_INV|cv::THRESH_OTSU);
	w_h_reset();
}

void Image::adaptiveBinarization(const Image& src){
	cv::adaptiveThreshold(src.img, img, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 7, 8);
	w_h_reset();
}

void Image::oneColor(cv::Scalar scal){
	img = cv::Mat(cv::Size(w, h), type, scal);
}

void Image::hsvColorExtraction(const Image& src,int low_hue,int up_hue,int low_saturation,int low_value){
    cv::Mat hsv, hue, hue1, hue2, saturation, value, hue_saturation;          //変数確保
	cv::cvtColor(src.img, hsv, CV_BGR2HSV);               // RGB（BGR）画像をHSV画像に変換する（frame → hsv）
	//赤い領域を取得
	// hsvをチャンネル毎にsinglechannelsというMat型の配列に分解して格納する。その結果、singlechannels[0]がHue, singlechannels[1]がSaturation, singlechannels[2]がValueの情報を持つ。
	std::vector<cv::Mat> singlechannels;//Matクラスのベクトルとしてsinglechannelsを定義
	cv::split(hsv, singlechannels);//hsvをsinglechannelsに分解([0]:h, [1]:s,[2]:v)

	//それぞれのチャンネルことに閾値を設定して二値化
	cv::threshold(singlechannels[0], hue1, low_hue, 255, CV_THRESH_BINARY);                 // singlechannels[0]をLOW_HUEを閾値処理して、LOW_HUE以上の部分が255,それ以下の部分が0になるように、hue1に格納する。
	cv::threshold(singlechannels[0], hue2, up_hue, 255, CV_THRESH_BINARY_INV);              // singlechannels[0]をUP_HUEを閾値処理して、UP_HUE以上の部分が0,それ以下の部分が255になるように、hue2に格納する。
	cv::threshold(singlechannels[1], saturation, low_saturation, 255, CV_THRESH_BINARY);    //彩度LOW_SATURATION以上
	cv::threshold(singlechannels[2], value, low_value, 255, CV_THRESH_BINARY);              //明度LOW_VALUE以上

	//条件を満たした領域をoutに設定
	cv::bitwise_and(hue1, hue2, hue);                                                       // hue1とhue2のbitごとのandをとる→hue
	cv::bitwise_and(hue, saturation, hue_saturation);                                       // hueとsaturationのbitごとのandをとる→hue_saturation
	cv::bitwise_and(hue_saturation, value, img);  
}

void Image::bitwiseAnd(const Image& src1,const Image& src2){
	cv::bitwise_and(src1.img,src2.img,img);
	w_h_reset();
}
void Image::bitwiseOr(const Image& src1,const Image& src2){
	cv::bitwise_or(src1.img,src2.img,img);
	w_h_reset();
}
void Image::bitwiseXor(const Image& src1,const Image& src2){
	cv::bitwise_xor(src1.img,src2.img,img);
	w_h_reset();
}
void Image::bitwiseNot(const Image& src){
	cv::bitwise_not(src.img,img);
	w_h_reset();
}

void Image::reversal(const Image& src){
	img = ~src.img;
	w_h_reset();
}

void Image::load(string name){
	try{
		if(!path.isExist(name))
			throw FileException(FileException::NOT_EXIST,path.pwd(name),"Image.cpp","Image::Load()",__LINE__);
		else if(path.isDirectory(name))
			throw FileException(FileException::DIRECTORY,path.pwd(name),"Image.cpp","Image::Load()",__LINE__);
		else{
			img = cv::imread(path.pwd(name), 1);
			w = img.size().width;
			h = img.size().height;
		}
	}catch(const FileException& e){
		e.showError();
		exit(EXIT_FAILURE);
	}
}

void Image::load(Dir path,string name){
	try{
		if(!path.isExist(name))
			throw FileException(FileException::NOT_EXIST,path.pwd(name),"Image.cpp","Image::Load()",__LINE__);
		else if(path.isDirectory(name))
			throw FileException(FileException::DIRECTORY,path.pwd(name),"Image.cpp","Image::Load()",__LINE__);
		else{
			img = cv::imread(path.pwd(name), 1);
			w = img.size().width;
			h = img.size().height;
		}
	}catch(const FileException& e){
		e.showError();
		exit(EXIT_FAILURE);
	}
}

void Image::save(string name){
	vector<int> params;
	params = vector<int>(2);
	// JPEG圧縮パラメータ
	params[0] = CV_IMWRITE_JPEG_QUALITY;
	params[1] = 95;
	cv::imwrite(path.pwd(name), img , params);
}

void Image::save(Dir path,string name){
	vector<int> params;
	params = vector<int>(2);
	// JPEG圧縮パラメータ
	params[0] = CV_IMWRITE_JPEG_QUALITY;
	params[1] = 95;
	cv::imwrite(path.pwd(name), img , params);
}

bool Image::empty(){
	return img.empty();
}

void Image::circle(cv::Point2f center,int radius,cv::Scalar scal,int thickness){
	cv::circle(img,center,radius,scal, thickness, CV_AA);
}

void Image::rectangle(cv::Point2f pt1,cv::Point2f pt2,cv::Scalar scal,int thickness){
	cv::rectangle(img,pt1,pt2,scal,thickness,8);
}
void Image::rectangle(cv::Point2f center,int w,int h,cv::Scalar scal,int thickness){
	cv::rectangle(img,cv::Point2f(center.x-w/2,center.y-h/2),cv::Point2f(center.x+w/2,center.y+h/2),scal,thickness,8);
}

void Image::rectangle(cv::Point2f center,int w,int h,int angle,cv::Scalar scal,int thickness){
	cv::Point pt1[1][4];
	int npt[] = {4};
	//double c=cos(angle*CV_PI/180),s=sin(angle*CV_PI/180);
	const cv::Point *ppt[1] = {pt1[0]};
	pt1[0][0] = cv::Point2f(center+Calc::PointRotate(w/2,h/2,angle));
	pt1[0][1] = cv::Point2f(center+Calc::PointRotate(w/2,-h/2,angle));
	pt1[0][2] = cv::Point2f(center+Calc::PointRotate(-w/2,-h/2,angle));
	pt1[0][3] = cv::Point2f(center+Calc::PointRotate(-w/2,h/2,angle));
	cv::polylines(img,ppt,npt,1,true,scal,thickness);
}

void Image::line(cv::Point2f pt1,cv::Point2f pt2,cv::Scalar scal,int thickness){
	cv::line(img,pt1,pt2,scal,thickness,8);
}

void Image::triangle(cv::Point2f pt1,cv::Point2f pt2,cv::Point2f pt3,cv::Scalar scal,int thickness){
	cv::line(img,pt1,pt2,scal,thickness,8);
	cv::line(img,pt2,pt3,scal,thickness,8);
	cv::line(img,pt3,pt1,scal,thickness,8);
}

pro::Image Image::imshow(string windowName,int flag){
	if(windowName.empty() && winName.empty()){
		throw Exception("ウィンドウを作成していません。","Image.cpp","Image::imshow(string,int)",__LINE__);
		return NULL;
	}else if(windowName != winName){
		winName = windowName;
		cv::namedWindow(winName,flag);
	}
	if(!img.empty()){
		cv::imshow(winName,img);
	}else{
		throw Exception("画像が読み込まれていません。","Image.cpp","Image::imshow(string,int)",__LINE__);
		return NULL;
	}
	return *this;
}

void Image::moveWindow(int x,int y){
	cv::moveWindow(winName,x,y);
}

void Image::resizeWindow(int w,int h){
	cv::resizeWindow(winName,w,h);
}

void Image::getWindowPos(int &x,int &y){
	RECT lpRect;
	HWND hWnd = (HWND)cvGetWindowHandle(winName.c_str());
	if(hWnd == NULL) return;
	if(GetWindowRect(hWnd,(LPRECT)&lpRect)==0) return;
	x = lpRect.left;
	y = lpRect.top;
}

void Image::getWindowSize(int &w,int &h){
	RECT lpRect;
	HWND hWnd = (HWND)cvGetWindowHandle(winName.c_str());
	if(hWnd == NULL) return;
	if(GetWindowRect(hWnd,(LPRECT)&lpRect)==0) return;
	w = lpRect.right-lpRect.left;
	h = lpRect.bottom-lpRect.top;
}

void Image::getWindowRect(int &x,int &y,int &w,int &h){
	RECT lpRect;
	HWND hWnd = (HWND)cvGetWindowHandle(winName.c_str());
	if(hWnd == NULL) return;
	if(GetWindowRect(hWnd,(LPRECT)&lpRect)==0) return;
	x = lpRect.left;
	y = lpRect.top;
	w = lpRect.right-lpRect.left;
	h = lpRect.bottom-lpRect.top;
}

cv::Size Image::size(){
	w_h_reset();
	return cv::Size(w,h);
}

unsigned char* Image::getU8Data(){
	unsigned char* udata = new unsigned char[img.cols*img.rows*img.channels()];
	//std::cout << img.channels() << std::endl;
	for(int i=0;i<img.rows;i++){
		for(int j=0;j<img.cols;j++){
			for(int k=0;k<img.channels();k++){
				udata[i*img.cols+j*img.channels()+k] = img.data[i*img.cols+j*img.channels()+k];
			}
		}
	}
	return udata;
}

void Image::setU8Data(unsigned char* cudata,int w,int h,int ch){
	init(w,h,img.type());
	//std::cout << ch << "," << img.channels() << std::endl;
	for(int i=0;i<h;i++){
		for(int j=0;j<w;j++){
			for(int k=0;k<ch;k++){
				img.data[w*i+j*ch+k] = cudata[w*i+j*ch+k];
			}
		}
	}
}

void Image::setU8Data(vector<unsigned char> cudata,int w,int h,int ch){
	init(w,h,img.type());
	for(int i=0;i<h;i++){
		for(int j=0;j<w;j++){
			for(int k=0;k<ch;k++){
				img.data[w*i+j*ch+k] = cudata[w*i+j*ch+k];
			}
		}
	}
	cudata.clear();
}

//float* Image::getF32Data(){
//	return (float*)img.data;
//}

//Image::operator cv::Mat &(){
//	return img;
//}

Image::operator const cv::Mat &(){
	return img;
}

Image::operator unsigned char *(){
	return img.data;
}
Image::operator const unsigned char *(){
	return img.data;
}

Image::operator const float* (){
	return (float*)img.data;
}

Image::operator float* (){
	return (float*)img.data;
}

Image::operator cv::Size (){
	return cv::Size(w,h);
}



}