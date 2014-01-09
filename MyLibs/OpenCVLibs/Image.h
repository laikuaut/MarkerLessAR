// Copyright (c) 2013 Shota Taniguchi
// 
// This software is released under the MIT License.
// 
// http://opensource.org/licenses/mit-license.php

#pragma once

#include"..\Core\Core.h"
#include"opencv2\core\core.hpp"
#include"opencv2\highgui\highgui.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include<string>
#include <Windows.h>

class PRO_EXPORTS cv::Mat;

namespace pro{

class PRO_EXPORTS Image
{
public:
	
	static const int _8UC1 = CV_8UC1;
	static const int _32FC1 = CV_32FC1;
	static const int _8UC3 = CV_8UC3;
	static const int _32FC3 = CV_32FC3;

private:

	cv::Mat img;
	int w,h,ch;
	std::string winName;
	int type;

public:

	Dir path;

private:

	void w_h_reset();

public:
	
	Image(void);
	Image(int w,int h,int type=_8UC3);
	Image(string name,int type=_8UC3);
	Image(int type);
	
	void init(int w,int h,int type=_8UC3);

	void release();
	void clone(const Image& src);
	void swap(Image& src);
	void resize(const Image& src,int interpolation=1);
	void resize(const Image& src,double fx,double fy,int interpolation=1);
	void resize(const Image& src,cv::Size size,int interpolation=1);
	void rotation(const Image& src,cv::Point2f center,double angle);

	void horiconcat(const Image &src1,const Image &src2,int band_w=0,cv::Scalar color=cv::Scalar::all(0));
	void vertconcat(const Image &src1,const Image &src2,int band_w=0,cv::Scalar color=cv::Scalar::all(0));
	
	void grayeScale(const Image& src);
	void grayToColor(const Image& src);
	void binarization(const Image& src);
	void adaptiveBinarization(const Image& src);
	void oneColor(cv::Scalar scal);
	// hueÇÃâ∫å¿ hueÇÃè„å¿(0Å`360) saturationÅiç ìxÅjÇÃâ∫å¿(0Å`100) valueÅiñæìxÅjÇÃâ∫å¿(0Å`100)
	void hsvColorExtraction(const Image& src,int low_hue,int up_hue,int low_saturation,int low_value);
	
	void bitwiseAnd(const Image& src1,const Image& src2);
	void bitwiseOr(const Image& src1,const Image& src2);
	void bitwiseXor(const Image& src1,const Image& src2);
	void bitwiseNot(const Image& src);

	void reversal(const Image& src);

	void load(string name);
	void load(Dir path,string name);
	void save(string name);
	void save(Dir path,string name);

	bool empty();

	void circle(cv::Point2f center,int radius,cv::Scalar scal = cv::Scalar(0,0,0),int thickness=-1);
	void rectangle(cv::Point2f pt1,cv::Point2f pt2,cv::Scalar scal = cv::Scalar(0,0,0),int thickness=1);
	void rectangle(cv::Point2f center,int w,int h,cv::Scalar scal = cv::Scalar(0,0,0),int thickness=1);
	void rectangle(cv::Point2f center,int w,int h,int angle,cv::Scalar scal = cv::Scalar(0,0,0),int thickness=1);
	void line(cv::Point2f pt1,cv::Point2f pt2,cv::Scalar scal = cv::Scalar(0,0,0),int thickness=1);
	void triangle(cv::Point2f pt1,cv::Point2f pt2,cv::Point2f pt3,cv::Scalar scal = cv::Scalar(0,0,0),int thickness=1);

	pro::Image imshow(string windowName=string(),int flag=0);
	void moveWindow(int x,int y);
	void resizeWindow(int w,int h);
	void getWindowPos(int &x,int &y);
	void getWindowSize(int &w,int &h);
	void getWindowRect(int &x,int &y,int &w,int &h);

	cv::Size size();

	unsigned char* getU8Data();
	void setU8Data(unsigned char* cudata,int w,int h,int ch);
	void setU8Data(vector<unsigned char> cudata,int w,int h,int ch);
	//float* getF32Data();

	//operator cv::Mat &();
	operator const cv::Mat &(); 
	operator const unsigned char* ();
	operator unsigned char* ();
	operator const float* ();
	operator float* ();
	operator cv::Size ();


};

}
