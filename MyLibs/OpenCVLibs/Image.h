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

class PRO_EXPORTS cv::Mat;

namespace pro{

class PRO_EXPORTS Image
{

private:

	cv::Mat img;
	int w,h;

public:

	Dir path;

private:

	void w_h_reset();

public:
	
	Image(void);
	
	void init(int w,int h);

	void release();
	void clone(const Image& src);
	void swap(Image& src);
	void resize(const Image& src);
	void resize(const Image& src,double fx,double fy);
	void resize(const Image& src,cv::Size size);
	void rotation(const Image& src,cv::Point2f center,double angle);
	
	void grayeScale(const Image& src);
	void binarization(const Image& src);
	void adaptiveBinarization(const Image& src);
	void oneColor(cv::Scalar scal);
	// hueの下限 hueの上限(0～360) saturation（彩度）の下限(0～100) value（明度）の下限(0～100)
	void hsvColorExtraction(const Image& src,int low_hue,int up_hue,int low_saturation,int low_value);
	
	void bitwiseAnd(const Image& src1,const Image& src2);
	void bitwiseOr(const Image& src1,const Image& src2);
	void bitwiseXor(const Image& src1,const Image& src2);
	void bitwiseNot(const Image& src);

	void reversal(const Image& src);

	void load(string name);
	void save(string name);

	bool empty();

	void circle(cv::Point2f center,int radius,cv::Scalar scal = cv::Scalar(0,0,0));
	void rectangle(cv::Point2f pt1,cv::Point2f pt2,cv::Scalar scal = cv::Scalar(0,0,0),int thickness=1);
	void rectangle(cv::Point2f center,int w,int h,cv::Scalar scal = cv::Scalar(0,0,0),int thickness=1);
	void rectangle(cv::Point2f center,int w,int h,int angle,cv::Scalar scal = cv::Scalar(0,0,0),int thickness=1);
	void line(cv::Point2f pt1,cv::Point2f pt2,cv::Scalar scal = cv::Scalar(0,0,0),int thickness=1);
	void triangle(cv::Point2f pt1,cv::Point2f pt2,cv::Point2f pt3,cv::Scalar scal = cv::Scalar(0,0,0),int thickness=1);

	void imshow(string windowName);

	cv::Size size();

	operator cv::Mat &(); 
	operator const unsigned char* ();
	operator unsigned char* ();
	operator cv::Size ();


};

}
