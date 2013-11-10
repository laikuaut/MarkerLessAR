#pragma once

#include"..\Core\Core.h"
#include<opencv2\core\core.hpp>
#include"Image.h"
#include"Labeling.h"
#include"MouseEvent.h"

namespace pro{

class PRO_EXPORTS LabelingCenter : public MouseEvent
{
private:

	Labeling labels;
	vector<cv::Point2f> centers;

	// mouseRangeFilter用の変数
	cv::Point2f mousePt[2];
	int ptCount;
	Image dst;

public:
	LabelingCenter(void);
	~LabelingCenter(void);

	// 初期化
	void init(cv::Size size);
	// 重心計算
	void setCenter(const unsigned char* bins);

	// 円形以外の重心除外
	void circleFilter(double per,double radius_per=1);
	// サイズに合わない重心除外
	void sizeFilter(int min,int max);
	// 矩形範囲外除外
	void rangeRectangleFilter(cv::Point2f pt1,cv::Point2f pt2);
	// フィルタによって削除した点を戻す
	void filterReset();
	
	
	void onMouse_impl(int event,int x,int y,int flag);
	// マウスで指定して矩形フィルターをかける
	void mouseRangeFilter(Image& img);

	void draw(Image& img,short flag=1,cv::Scalar scal = cv::Scalar::all(0));

	void outputCenter();

	cv::Point operator() (const int num);

public:
	int num;
	vector<int> sizes;
	vector<short> flags;
};

}